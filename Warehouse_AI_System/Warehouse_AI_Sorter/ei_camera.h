#ifndef _EI_CAMERA_H_
#define _EI_CAMERA_H_

/* Includes ---------------------------------------------------------------- */
#include <dylan42415-project-1_inferencing.h>
#include <Arduino_OV767X.h>

#include <stdint.h>
#include <stdlib.h>

/* Constant variables ------------------------------------------------------- */
#define EI_CAMERA_RAW_FRAME_BUFFER_COLS     160
#define EI_CAMERA_RAW_FRAME_BUFFER_ROWS     120

#define DWORD_ALIGN_PTR(a)   ((a & 0x3) ?(((uintptr_t)a + 0x4) & ~(uintptr_t)0x3) : a)

/* Edge Impulse ------------------------------------------------------------- */
#define CAMERA_SIGNAL_TIMEOUT 2000000UL

class OV7675 : public OV767X {
    public:
        int begin(int resolution, int format, int fps);
        bool readFrame(void* buffer);

    private:
        int vsyncPin;
        int hrefPin;
        int pclkPin;
        int xclkPin;

        volatile uint32_t* vsyncPort;
        uint32_t vsyncMask;
        volatile uint32_t* hrefPort;
        uint32_t hrefMask;
        volatile uint32_t* pclkPort;
        uint32_t pclkMask;

        uint16_t width;
        uint16_t height;
        uint8_t bytes_per_pixel;
        uint16_t bytes_per_row;
        uint8_t buf_rows;
        uint16_t buf_size;
        uint8_t resize_height;
        uint8_t *raw_buf;
        void *buf_mem;
        uint8_t *intrp_buf;
        uint8_t *buf_limit;

        bool readBuf();
        int allocate_scratch_buffs();
        int deallocate_scratch_buffs();
};

typedef struct {
	size_t width;
	size_t height;
} ei_device_resize_resolutions_t;

int ei_get_serial_available(void) {
    return Serial.available();
}

char ei_get_serial_byte(void) {
    return Serial.read();
}

/* Private variables ------------------------------------------------------- */
static OV7675 Cam;
static bool is_initialised = false;

/*
** @brief points to the output of the capture
*/
static uint8_t *ei_camera_capture_out = NULL;
uint32_t resize_col_sz;
uint32_t resize_row_sz;
bool do_resize = false;
bool do_crop = false;

static bool debug_nn = false;

/* Function definitions ------------------------------------------------------- */
bool ei_camera_init(void);
void ei_camera_deinit(void);
bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf);
int calculate_resize_dimensions(uint32_t out_width, uint32_t out_height, uint32_t *resize_col_sz, uint32_t *resize_row_sz, bool *do_resize);
void resizeImage(int srcWidth, int srcHeight, uint8_t *srcImage, int dstWidth, int dstHeight, uint8_t *dstImage, int iBpp);
void cropImage(int srcWidth, int srcHeight, uint8_t *srcImage, int startX, int startY, int dstWidth, int dstHeight, uint8_t *dstImage, int iBpp);

int calculate_resize_dimensions(uint32_t out_width, uint32_t out_height, uint32_t *resize_col_sz, uint32_t *resize_row_sz, bool *do_resize)
{
    size_t list_size = 2;
    const ei_device_resize_resolutions_t list[list_size] = { {42,32}, {128,96} };

    *resize_col_sz = EI_CAMERA_RAW_FRAME_BUFFER_COLS;
    *resize_row_sz = EI_CAMERA_RAW_FRAME_BUFFER_ROWS;
    *do_resize = false;

    for (size_t ix = 0; ix < list_size; ix++) {
        if ((out_width <= list[ix].width) && (out_height <= list[ix].height)) {
            *resize_col_sz = list[ix].width;
            *resize_row_sz = list[ix].height;
            *do_resize = true;
            break;
        }
    }

    return 0;
}

/**
 * @brief   Setup image sensor & start streaming (Pure Stock Official Implementation)
 */
bool ei_camera_init(void) {
    if (is_initialised) return true;

    if (!Cam.begin(QQVGA, RGB565, 1)) {
        ei_printf("ERR: Failed to initialize camera\r\n");
        return false;
    }
    is_initialised = true;

    return true;
}

void ei_camera_deinit(void) {
    if (is_initialised) {
        Cam.end();
        is_initialised = false;
    }
}

bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf)
{
    if (!is_initialised) {
        ei_printf("ERR: Camera is not initialized\r\n");
        return false;
    }

    if (!out_buf) {
        ei_printf("ERR: invalid parameters\r\n");
        return false;
    }

    int res = calculate_resize_dimensions(img_width, img_height, &resize_col_sz, &resize_row_sz, &do_resize);
    if (res) {
        ei_printf("ERR: Failed to calculate resize dimensions (%d)\r\n", res);
        return false;
    }

    if ((img_width != resize_col_sz) || (img_height != resize_row_sz)) {
        do_crop = true;
    }

    if (!Cam.readFrame(out_buf)) {
        ei_printf("ERR: Camera readFrame failed\r\n");
        return false;
    }

    if (do_crop) {
        uint32_t crop_col_sz = img_width;
        uint32_t crop_row_sz = img_height;
        uint32_t crop_row_start = (resize_row_sz - img_height) / 2;
        uint32_t crop_col_start = (resize_col_sz - img_width) / 2;

        cropImage(resize_col_sz, resize_row_sz,
                out_buf,
                crop_col_start, crop_row_start,
                crop_col_sz, crop_row_sz,
                out_buf,
                16);
    }

    ei_camera_capture_out = out_buf;
    return true;
}

/**
 * @brief      Convert RGB565 raw camera buffer to RGB888 for DSP inference
 */
int ei_camera_cutout_get_data(size_t offset, size_t length, float *out_ptr) {
    size_t pixel_ix = offset * 2;
    size_t bytes_left = length;
    size_t out_ptr_ix = 0;

    while (bytes_left != 0) {
        uint16_t pixel = (ei_camera_capture_out[pixel_ix] << 8) | ei_camera_capture_out[pixel_ix+1];
        uint8_t r, g, b;
        r = ((pixel >> 11) & 0x1f) << 3;
        g = ((pixel >> 5) & 0x3f) << 2;
        b = (pixel & 0x1f) << 3;

        float pixel_f = (r << 16) + (g << 8) + b;
        out_ptr[out_ptr_ix] = pixel_f;

        out_ptr_ix++;
        pixel_ix += 2;
        bytes_left--;
    }

    return 0;
}

#ifdef __ARM_FEATURE_SIMD32
#include <device.h>
#endif
#define FRAC_BITS 14
#define FRAC_VAL (1<<FRAC_BITS)
#define FRAC_MASK (FRAC_VAL - 1)

void resizeImage(int srcWidth, int srcHeight, uint8_t *srcImage, int dstWidth, int dstHeight, uint8_t *dstImage, int iBpp)
{
    uint32_t src_x_accum, src_y_accum;
    uint32_t x_frac, nx_frac, y_frac, ny_frac;
    int x, y, ty, tx;

    if (iBpp != 8 && iBpp != 16)
        return;
    src_y_accum = FRAC_VAL/2;
    const uint32_t src_x_frac = (srcWidth * FRAC_VAL) / dstWidth;
    const uint32_t src_y_frac = (srcHeight * FRAC_VAL) / dstHeight;
    const uint32_t r_mask = 0xf800f800;
    const uint32_t g_mask = 0x07e007e0;
    const uint32_t b_mask = 0x001f001f;
    uint8_t *s, *d;
    uint16_t *s16, *d16;
    uint32_t x_frac2, y_frac2;
    for (y=0; y < dstHeight; y++) {
        ty = src_y_accum >> FRAC_BITS;
        y_frac = src_y_accum & FRAC_MASK;
        src_y_accum += src_y_frac;
        ny_frac = FRAC_VAL - y_frac;
        y_frac2 = ny_frac | (y_frac << 16);
        s = &srcImage[ty * srcWidth];
        s16 = (uint16_t *)&srcImage[ty * srcWidth * 2];
        d = &dstImage[y * dstWidth];
        d16 = (uint16_t *)&dstImage[y * dstWidth * 2];
        src_x_accum = FRAC_VAL/2;
        if (iBpp == 8) {
            for (x=0; x < dstWidth; x++) {
                uint32_t tx, p00,p01,p10,p11;
                tx = src_x_accum >> FRAC_BITS;
                x_frac = src_x_accum & FRAC_MASK;
                nx_frac = FRAC_VAL - x_frac;
                x_frac2 = nx_frac | (x_frac << 16);
                src_x_accum += src_x_frac;
                p00 = s[tx]; p10 = s[tx+1];
                p01 = s[tx+srcWidth]; p11 = s[tx+srcWidth+1];
                p00 = ((p00 * nx_frac) + (p10 * x_frac) + FRAC_VAL/2) >> FRAC_BITS;
                p01 = ((p01 * nx_frac) + (p11 * x_frac) + FRAC_VAL/2) >> FRAC_BITS;
                p00 = ((p00 * ny_frac) + (p01 * y_frac) + FRAC_VAL/2) >> FRAC_BITS;
                *d++ = (uint8_t)p00;
            }
        } else {
            for (x=0; x < dstWidth; x++) {
                uint32_t tx, p00,p01,p10,p11;
                uint32_t r00, r01, r10, r11, g00, g01, g10, g11, b00, b01, b10, b11;
                tx = src_x_accum >> FRAC_BITS;
                x_frac = src_x_accum & FRAC_MASK;
                nx_frac = FRAC_VAL - x_frac;
                x_frac2 = nx_frac | (x_frac << 16);
                src_x_accum += src_x_frac;
                p00 = __builtin_bswap16(s16[tx]); p10 = __builtin_bswap16(s16[tx+1]);
                p01 = __builtin_bswap16(s16[tx+srcWidth]); p11 = __builtin_bswap16(s16[tx+srcWidth+1]);
                r00 = (p00 & r_mask) >> 1; g00 = p00 & g_mask; b00 = p00 & b_mask;
                r10 = (p10 & r_mask) >> 1; g10 = p10 & g_mask; b10 = p10 & b_mask;
                r01 = (p01 & r_mask) >> 1; g01 = p01 & g_mask; b01 = p01 & b_mask;
                r11 = (p11 & r_mask) >> 1; g11 = p11 & g_mask; b11 = p11 & b_mask;
                r00 = ((r00 * nx_frac) + (r10 * x_frac) + FRAC_VAL/2) >> FRAC_BITS;
                r01 = ((r01 * nx_frac) + (r11 * x_frac) + FRAC_VAL/2) >> FRAC_BITS;
                r00 = ((r00 * ny_frac) + (r01 * y_frac) + FRAC_VAL/2) >> FRAC_BITS;
                g00 = ((g00 * nx_frac) + (g10 * x_frac) + FRAC_VAL/2) >> FRAC_BITS;
                g01 = ((g01 * nx_frac) + (g11 * x_frac) + FRAC_VAL/2) >> FRAC_BITS;
                g00 = ((g00 * ny_frac) + (g01 * y_frac) + FRAC_VAL/2) >> FRAC_BITS;
                b00 = ((b00 * nx_frac) + (b10 * x_frac) + FRAC_VAL/2) >> FRAC_BITS;
                b01 = ((b01 * nx_frac) + (b11 * x_frac) + FRAC_VAL/2) >> FRAC_BITS;
                b00 = ((b00 * ny_frac) + (b01 * y_frac) + FRAC_VAL/2) >> FRAC_BITS;
                r00 = (r00 << 1) & r_mask;
                g00 = g00 & g_mask;
                b00 = b00 & b_mask;
                p00 = (r00 | g00 | b00);
                *d16++ = (uint16_t)__builtin_bswap16(p00);
            }
        }
    }
}

void cropImage(int srcWidth, int srcHeight, uint8_t *srcImage, int startX, int startY, int dstWidth, int dstHeight, uint8_t *dstImage, int iBpp)
{
    uint32_t *s32, *d32;
    int x, y;

    if (startX < 0 || startX >= srcWidth || startY < 0 || startY >= srcHeight || (startX + dstWidth) > srcWidth || (startY + dstHeight) > srcHeight)
       return;
    if (iBpp != 8 && iBpp != 16)
       return;

    if (iBpp == 8) {
      uint8_t *s, *d;
      for (y=0; y<dstHeight; y++) {
        s = &srcImage[srcWidth * (y + startY) + startX];
        d = &dstImage[(dstWidth * y)];
        x = 0;
        for (; x<dstWidth; x++) {
          *d++ = *s++;
        }
      }
    } else {
      uint16_t *s, *d;
      for (y=0; y<dstHeight; y++) {
        s = (uint16_t *)&srcImage[2 * srcWidth * (y + startY) + startX * 2];
        d = (uint16_t *)&dstImage[(dstWidth * y * 2)];
        x = 0;
        for (; x<dstWidth; x++) {
          *d++ = *s++;
        }
      }
    }
}

#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_CAMERA
#error "Invalid model for current sensor"
#endif

#include <Arduino.h>
#include <Wire.h>

#define digitalPinToBitMask(P) (1 << (digitalPinToPinName(P) % 32))
#define portInputRegister(P) ((P == 0) ? &NRF_P0->IN : &NRF_P1->IN)

int OV7675::begin(int resolution, int format, int fps)
{
    pinMode(OV7670_VSYNC, INPUT);
    pinMode(OV7670_HREF, INPUT);
    pinMode(OV7670_PLK, INPUT);
    pinMode(OV7670_XCLK, OUTPUT);

    vsyncPort = portInputRegister(digitalPinToPort(OV7670_VSYNC));
    vsyncMask = digitalPinToBitMask(OV7670_VSYNC);
    hrefPort = portInputRegister(digitalPinToPort(OV7670_HREF));
    hrefMask = digitalPinToBitMask(OV7670_HREF);
    pclkPort = portInputRegister(digitalPinToPort(OV7670_PLK));
    pclkMask = digitalPinToBitMask(OV7670_PLK);

    bool ret = OV767X::begin(VGA, format, fps);
    width = OV767X::width();
    height = OV767X::height();
    bytes_per_pixel = OV767X::bytesPerPixel();
    bytes_per_row = width * bytes_per_pixel;
    resize_height = 2;

    buf_mem = NULL;
    raw_buf = NULL;
    intrp_buf = NULL;

    return ret;
}

int OV7675::allocate_scratch_buffs()
{
    buf_rows = height / resize_row_sz * resize_height;
    buf_size = bytes_per_row * buf_rows;

    buf_mem = ei_malloc(buf_size);
    if(buf_mem == NULL) {
        ei_printf("failed to create buf_mem\r\n");
        return -1;
    }
    raw_buf = (uint8_t *)DWORD_ALIGN_PTR((uintptr_t)buf_mem);
    return 0;
}

int OV7675::deallocate_scratch_buffs()
{
    ei_free(buf_mem);
    buf_mem = NULL;
    return 0;
}

bool OV7675::readFrame(void* buffer)
{
    if (allocate_scratch_buffs() != 0 || raw_buf == NULL) {
        return false;
    }

    uint8_t* out = (uint8_t*)buffer;
    noInterrupts();

    // Falling edge indicates start of frame with timeout protection
    uint32_t to = 5000000UL;
    while ((*vsyncPort & vsyncMask) == 0) {
        if (--to == 0) {
            interrupts();
            deallocate_scratch_buffs();
            return false;
        }
    }
    to = 5000000UL;
    while ((*vsyncPort & vsyncMask) != 0) {
        if (--to == 0) {
            interrupts();
            deallocate_scratch_buffs();
            return false;
        }
    }

    int out_row = 0;
    for (int raw_height = 0; raw_height < height; raw_height += buf_rows) {
        if (!readBuf()) {
            interrupts();
            deallocate_scratch_buffs();
            return false;
        }

        resizeImage(width, buf_rows,
                    raw_buf,
                    resize_col_sz, resize_height,
                    &(out[out_row]),
                    16);

        out_row += resize_col_sz * resize_height * bytes_per_pixel;
    }

    interrupts();
    deallocate_scratch_buffs();
    return true;
}

bool OV7675::readBuf()
{
    int offset = 0;
    uint32_t ulPin = 33;
    NRF_GPIO_Type * port;
    port = nrf_gpio_pin_port_decode(&ulPin);

    for (int i = 0; i < buf_rows; i++) {
        uint32_t to = CAMERA_SIGNAL_TIMEOUT;
        while ((*hrefPort & hrefMask) == 0) {
            if (--to == 0) return false;
        }

        for (int col = 0; col < bytes_per_row; col++) {
            to = CAMERA_SIGNAL_TIMEOUT;
            while ((*pclkPort & pclkMask) != 0) {
                if (--to == 0) return false;
            }

            uint32_t in = port->IN;
            in >>= 2;
            in &= 0x3f03;
            in |= (in >> 6);

            raw_buf[offset++] = in;

            to = CAMERA_SIGNAL_TIMEOUT;
            while ((*pclkPort & pclkMask) == 0) {
                if (--to == 0) return false;
            }
        }

        to = CAMERA_SIGNAL_TIMEOUT;
        while ((*hrefPort & hrefMask) != 0) {
            if (--to == 0) return false;
        }
    }
    return true;
}

#endif // _EI_CAMERA_H_

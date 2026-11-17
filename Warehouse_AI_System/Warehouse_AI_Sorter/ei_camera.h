#ifndef _EI_CAMERA_H_
#define _EI_CAMERA_H_

#include <Arduino.h>
#include <Arduino_OV767X.h>
#include <dylan42415-project-1_inferencing.h>

// Dimensions for camera raw capture (Native QQVGA Grayscale)
#define CAMERA_RAW_WIDTH   160
#define CAMERA_RAW_HEIGHT  120

#define DWORD_ALIGN_PTR(a) ((a & 0x3) ? (((uintptr_t)a + 0x4) & ~(uintptr_t)0x3) : a)

// State variables
static bool is_initialised = false;
static uint8_t *ei_camera_capture_out = NULL;
static uint8_t raw_grayscale_buffer[CAMERA_RAW_WIDTH * CAMERA_RAW_HEIGHT]; // 19,200 bytes static buffer

// Fixed-point scaling helpers for bilinear interpolation (8-bit grayscale)
#define FRAC_BITS 14
#define FRAC_VAL (1 << FRAC_BITS)
#define FRAC_MASK (FRAC_VAL - 1)

// Fast bilinear image resize for 8-bit grayscale pixels
inline void resizeGrayscaleImage(int srcWidth, int srcHeight, uint8_t *srcImage, 
                                 int dstWidth, int dstHeight, uint8_t *dstImage) {
    uint32_t src_x_accum, src_y_accum;
    uint32_t x_frac, nx_frac, y_frac, ny_frac;
    int x, y, ty;

    src_y_accum = FRAC_VAL / 2;
    const uint32_t src_x_frac = (srcWidth * FRAC_VAL) / dstWidth;
    const uint32_t src_y_frac = (srcHeight * FRAC_VAL) / dstHeight;

    for (y = 0; y < dstHeight; y++) {
        ty = src_y_accum >> FRAC_BITS;
        y_frac = src_y_accum & FRAC_MASK;
        src_y_accum += src_y_frac;
        ny_frac = FRAC_VAL - y_frac;

        uint8_t *s = &srcImage[ty * srcWidth];
        uint8_t *d = &dstImage[y * dstWidth];
        src_x_accum = FRAC_VAL / 2;

        for (x = 0; x < dstWidth; x++) {
            uint32_t tx = src_x_accum >> FRAC_BITS;
            x_frac = src_x_accum & FRAC_MASK;
            nx_frac = FRAC_VAL - x_frac;
            src_x_accum += src_x_frac;

            uint32_t p00 = s[tx];
            uint32_t p10 = s[tx + 1];
            uint32_t p01 = s[tx + srcWidth];
            uint32_t p11 = s[tx + srcWidth + 1];

            uint32_t top = ((p00 * nx_frac) + (p10 * x_frac) + FRAC_VAL / 2) >> FRAC_BITS;
            uint32_t bot = ((p01 * nx_frac) + (p11 * x_frac) + FRAC_VAL / 2) >> FRAC_BITS;
            *d++ = (uint8_t)(((top * ny_frac) + (bot * y_frac) + FRAC_VAL / 2) >> FRAC_BITS);
        }
    }
}

// Initialize OV7675 camera in Native GRAYSCALE mode
inline bool ei_camera_init(void) {
    if (is_initialised) return true;

    // Initialize hardware camera in QQVGA (160x120) GRAYSCALE mode (1 byte/pixel)
    if (!Camera.begin(QQVGA, GRAYSCALE, 1)) {
        return false;
    }
    is_initialised = true;
    return true;
}

// De-initialize camera
inline void ei_camera_deinit(void) {
    if (is_initialised) {
        Camera.end();
        is_initialised = false;
    }
}

// Capture frame and resize directly to target dimensions (96x96 grayscale)
inline bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf) {
    if (!is_initialised) {
        if (!ei_camera_init()) return false;
    }
    if (!out_buf) return false;

    // Read full 160x120 8-bit grayscale frame from hardware camera
    Camera.readFrame(raw_grayscale_buffer);

    // Rescale down to target (96x96) grayscale using optimized bilinear interpolation
    resizeGrayscaleImage(CAMERA_RAW_WIDTH, CAMERA_RAW_HEIGHT, raw_grayscale_buffer,
                         img_width, img_height, out_buf);

    ei_camera_capture_out = out_buf;
    return true;
}

// Edge Impulse signal callback: converts 8-bit grayscale pixel to normalized RGB float for DSP
inline int ei_camera_cutout_get_data(size_t offset, size_t length, float *out_ptr) {
    for (size_t i = 0; i < length; i++) {
        uint8_t gray = ei_camera_capture_out[offset + i];
        // Package 8-bit grayscale into 32-bit packed float (R=gray, G=gray, B=gray)
        // Edge Impulse extract_image_features normalizes: (0.299*R + 0.587*G + 0.114*B)/255 = gray/255.0f
        out_ptr[i] = (float)((gray << 16) | (gray << 8) | gray);
    }
    return 0;
}

#endif // _EI_CAMERA_H_

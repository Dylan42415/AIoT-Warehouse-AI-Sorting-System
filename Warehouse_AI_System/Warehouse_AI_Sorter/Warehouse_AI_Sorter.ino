/*
 ==============================================================================
  AIoT Final Course Project: Automated Warehouse AI Object Sorting System
  Module: AI53003FP (Artificial Intelligence of Things Applications)
  Student: Dylan (Index: 22)
  Target Grade: Level 4 (Maximum Marks: 2 Sensors, 2 Actuators, 2 Protocols)
 ==============================================================================
 
  Hardware Pinout Configuration (Tiny Machine Learning Carrier):
  ----------------------------------------------------------------------------
  1. Push Button (Input Sensor 1)    -> Carrier PB1 (Pin 13, Lab 13 Interrupt) / Serial 's'
  2. Proximity Sensor (Input Sensor 2)-> Onboard APDS-9960 (I2C: Wire1, Lab 03 & 12)
  3. Conveyor LED (Output Actuator 1) -> Onboard RGB LEDs (Green = Moving, Red = Stop/Scan)
  4. Servo Motor (Output Actuator 2) -> Grove J3 (Pin 12) & J2 (Pin 11) (Sorts item: 180 deg for 5s)
  5. Hardware UART Serial Gateway    -> Grove J1: A6 (TX) / A7 (RX) to Raspberry Pi (Lab 07)
  6. Edge Impulse TinyML Vision      -> OV7675 Camera (MobileNetV1 96x96 0.25 Grayscale int8, Lab 15)
  7. USB Debug Monitor               -> Serial (9600 baud)
 ==============================================================================
*/

#include <Arduino.h>
#include <Servo.h>
#include <Arduino_APDS9960.h>

// Edge Impulse Camera & Machine Learning Inferencing Headers
#include "ei_camera.h"

// ==============================================================================
// 1. PIN & HARDWARE DEFINITIONS (Grounded in Lab 11, 12 & 13)
// ==============================================================================
#define BUTTON_CARRIER      13          // Push button PB1 on TinyML Carrier (Lab 13 Interrupt)
#define SERVO_PIN_PRIMARY   12          // Grove port J3: Pin D12 (TinyML Carrier Standard)
#define SERVO_PIN_SECONDARY 11          // Grove port J2: Pin D11 (Lecturer Lab 11 & 12 Reference)

// Volatile flag for hardware button interrupt (Lab 13)
volatile bool buttonTriggered = false;

void button_interrupt() {
  buttonTriggered = true;
}

// Check if push button PB1 was pressed (interrupt or direct digitalRead)
bool isButtonPressed() {
  if (buttonTriggered) {
    buttonTriggered = false;
    return true;
  }
  if (digitalRead(BUTTON_CARRIER) == LOW) {
    return true;
  }
  return false;
}

// Define UART object for communication with Raspberry Pi via J1 (A6 = TX, A7 = RX, Lab 07)
UART mySerial(A6, A7, NC, NC);

// Servo instances for dual-port support (operates on both J2/D11 and J3/D12)
Servo servoPrimary;
Servo servoSecondary;

// Slew-rate limited servo movement: prevents sudden inrush current and brownout resets
void smoothServoMove(int startAngle, int targetAngle, int stepDelayMs = 15) {
  if (startAngle < targetAngle) {
    for (int pos = startAngle; pos <= targetAngle; pos += 5) {
      servoPrimary.write(pos);
      servoSecondary.write(pos);
      delay(stepDelayMs);
    }
  } else {
    for (int pos = startAngle; pos >= targetAngle; pos -= 5) {
      servoPrimary.write(pos);
      servoSecondary.write(pos);
      delay(stepDelayMs);
    }
  }
}

// ==============================================================================
// 2. FINITE STATE MACHINE (FSM) STATES
// ==============================================================================
enum SystemState {
  STATE_IDLE,             // Waiting for warehouse operator to press Start Button (PB1 / 's')
  STATE_CONVEYOR_RUNNING, // Conveyor belt moving (Green LED ON), polling APDS proximity
  STATE_OBJECT_DETECTED,  // Object arrived at scan zone (Conveyor stops, Red LED ON)
  STATE_INFERENCE,        // AI Model analyzes the object (Mouse vs Background)
  STATE_SORTING_ACTION,   // Servo sorts target item (180 deg for 5 sec)
  STATE_TRANSMIT_CLOUD    // Telemetry uploaded to Raspberry Pi gateway (UART -> ThingSpeak)
};

SystemState currentState = STATE_IDLE;

// Telemetry Variables
String detectedProduct = "none";
int confidenceScore = 0;
bool sortedActionTaken = false;
bool cameraAvailable = false;
bool calibrationMode = false;

// Proximity threshold (APDS-9960: 0 = closest, 255 = far, Lab 03 & Lab 12)
const int PROXIMITY_THRESHOLD = 40; 

// Conveyor timeout tracker at file scope
unsigned long conveyorStartTime = 0;

// Helper to compute grayscale luma from a 16-bit RGB565 pixel
static inline uint8_t rgb565ToLuma(uint8_t hi, uint8_t lo) {
  uint16_t pixel = ((uint16_t)hi << 8) | lo;
  uint8_t r = ((pixel >> 11) & 0x1f) << 3;
  uint8_t g = ((pixel >> 5) & 0x3f) << 2;
  uint8_t b = (pixel & 0x1f) << 3;
  return (uint8_t)((r * 299UL + g * 587UL + b * 114UL) / 1000UL);
}

// ==============================================================================
// LIVE CAMERA CALIBRATION, 24x24 ASCII VIEWER & LUMA DIAGNOSTIC ('c' / 'x')
// ==============================================================================
void runCalibrationTick() {
  if (!cameraAvailable && !ei_camera_init()) {
    Serial.println("[CAL] Error: OV7675 camera not detected.");
    return;
  }
  cameraAvailable = true;

  uint32_t resize_col_sz;
  uint32_t resize_row_sz;
  bool do_resize = false;
  calculate_resize_dimensions(EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT, &resize_col_sz, &resize_row_sz, &do_resize);

  void *snapshot_mem = ei_malloc((resize_col_sz * resize_row_sz * 2) + 8);
  if (snapshot_mem == NULL) {
    Serial.println("[CAL] Error: Snapshot buffer memory allocation failed.");
    return;
  }
  uint8_t *snapshot_buf = (uint8_t *)DWORD_ALIGN_PTR((uintptr_t)snapshot_mem);

  if (!ei_camera_capture(EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT, snapshot_buf)) {
    Serial.println("[CAL] Error: Camera frame capture timed out or failed.");
    ei_free(snapshot_mem);
    return;
  }

  // Print 24x24 ASCII preview of the 96x96 frame so user can visually align the mouse
  const char *asciiRamp = " .:-=+*#%@";
  Serial.println("\n  +------------------------+ (24x24 Live Camera View)");
  for (uint32_t y = 0; y < EI_CLASSIFIER_INPUT_HEIGHT; y += 4) {
    Serial.print("  |");
    for (uint32_t x = 0; x < EI_CLASSIFIER_INPUT_WIDTH; x += 4) {
      uint32_t idx = (y * EI_CLASSIFIER_INPUT_WIDTH + x) * 2;
      uint8_t luma = rgb565ToLuma(snapshot_buf[idx], snapshot_buf[idx + 1]);
      uint8_t rampIdx = (uint8_t)((luma * 9UL) / 255UL);
      Serial.print(asciiRamp[rampIdx]);
    }
    Serial.println("|");
  }
  Serial.println("  +------------------------+");

  // Calculate Grayscale Luminance statistics across 96x96 RGB565 frame
  uint32_t totalPixels = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
  uint32_t lumaSum = 0;
  uint8_t minLuma = 255;
  uint8_t maxLuma = 0;

  for (uint32_t i = 0; i < totalPixels; i++) {
    uint8_t luma = rgb565ToLuma(snapshot_buf[i * 2], snapshot_buf[i * 2 + 1]);
    lumaSum += luma;
    if (luma < minLuma) minLuma = luma;
    if (luma > maxLuma) maxLuma = luma;
  }
  uint8_t avgLuma = (uint8_t)(lumaSum / totalPixels);

  // Run TinyML Inference
  ei::signal_t signal;
  signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
  signal.get_data = &ei_camera_cutout_get_data;

  ei_impulse_result_t result = { 0 };
  EI_IMPULSE_ERROR ei_error = run_classifier(&signal, &result, false);

  float mouseProb = 0.0f;
  float bgProb = 0.0f;

  if (ei_error == EI_IMPULSE_OK) {
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
      if (strcmp(ei_classifier_inferencing_categories[i], "mouse") == 0) {
        mouseProb = result.classification[i].value * 100.0f;
      } else if (strcmp(ei_classifier_inferencing_categories[i], "background") == 0) {
        bgProb = result.classification[i].value * 100.0f;
      }
    }
  }

  Serial.print("[CAL] Luma avg:");
  Serial.print(avgLuma);
  Serial.print(" (min:");
  Serial.print(minLuma);
  Serial.print(", max:");
  Serial.print(maxLuma);
  Serial.print(") | mouse: ");
  Serial.print(mouseProb, 1);
  Serial.print("% | background: ");
  Serial.print(bgProb, 1);
  Serial.println("%");

  ei_free(snapshot_mem);
}

// ==============================================================================
// 3. SETUP FUNCTION
// ==============================================================================
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(BUTTON_CARRIER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_CARRIER), button_interrupt, FALLING);

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  int apdsRetries = 0;
  while (!APDS.begin() && apdsRetries < 3) {
    Serial.println("⚠️ Warning: APDS-9960 retry initialization...");
    delay(400);
    apdsRetries++;
  }
  if (apdsRetries >= 3) {
    Serial.println("❌ Error: Failed to initialize APDS-9960 Proximity Sensor! Check I2C bus.");
  } else {
    Serial.println("✅ APDS-9960 Proximity Sensor Initialized.");
  }

  Serial.println("[INIT] Initializing OV7675 Camera (Native Grayscale mode)...");
  if (ei_camera_init()) {
    cameraAvailable = true;
    Serial.println("✅ OV7675 Camera Initialized in Native Grayscale Mode.");
  } else {
    cameraAvailable = false;
    Serial.println("⚠️ Notice: Camera not detected. Fallback interactive mode enabled.");
  }

  Serial.println("==================================================");
  Serial.println("  WAREHOUSE AI CONVEYOR SORTING SYSTEM READY!    ");
  Serial.println("  Level 4 Architecture Initialized (Grayscale).  ");
  Serial.println("  Controls:");
  Serial.println("    • Press PB1 or type 's': Start Conveyor / Trigger Scan");
  Serial.println("    • Type 'm': Instant Mouse Demo (180 deg Servo for 5s)");
  Serial.println("    • Type 'b': Instant Background Demo (Servo Idle at 0 deg)");
  Serial.println("    • Type 'c': Enter Live Camera Calibration + ASCII View");
  Serial.println("    • Type 'x': Exit Live Camera Calibration");
  Serial.println("==================================================");
}

// ==============================================================================
// 4. MAIN LOOP - FINITE STATE MACHINE
// ==============================================================================
void loop() {

  if (Serial.available()) {
    char c = Serial.read();

    if (c == '\r' || c == '\n') {
      return;
    }
    
    if (c == 'm' || c == 'M') {
      Serial.println("\n[DEMO] >>> INJECTING MOUSE TARGET DETECTION (96%) <<<");
      detectedProduct = "mouse";
      confidenceScore = 96;
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDR, LOW);
      currentState = STATE_SORTING_ACTION;
      return;
    }
    
    if (c == 'b' || c == 'B') {
      Serial.println("\n[DEMO] >>> INJECTING BACKGROUND NON-TARGET (98%) <<<");
      detectedProduct = "background";
      confidenceScore = 98;
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDR, LOW);
      currentState = STATE_SORTING_ACTION;
      return;
    }

    if (c == 'c' || c == 'C') {
      if (currentState == STATE_IDLE || currentState == STATE_CONVEYOR_RUNNING) {
        calibrationMode = true;
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDR, LOW);
        digitalWrite(LEDB, HIGH);
        Serial.println("\n[CAL] === ENTERING LIVE CAMERA CALIBRATION MODE ===");
        Serial.println("[CAL] Streaming 24x24 ASCII preview + Luma + TinyML confidence.");
        Serial.println("[CAL] Type 'x' to exit calibration or 's' to start conveyor sorting.");
        return;
      }
    }

    if (c == 'x' || c == 'X') {
      if (calibrationMode) {
        calibrationMode = false;
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDG, HIGH);
        Serial.println("\n[CAL] === EXITED CAMERA CALIBRATION MODE (Back to IDLE) ===");
        currentState = STATE_IDLE;
        return;
      }
    }
    
    if (c == 's' || c == 'S' || c == ' ') {
      if (calibrationMode) {
        calibrationMode = false;
        Serial.println("\n[CAL] Exited calibration mode to run conveyor sorting.");
      }
      if (currentState == STATE_IDLE) {
        Serial.println("\n[START] Conveyor Belt Running (Green LED ON)...");
        digitalWrite(LEDG, LOW);
        digitalWrite(LEDR, HIGH);
        conveyorStartTime = millis();
        sortedActionTaken = false;
        buttonTriggered = false;
        currentState = STATE_CONVEYOR_RUNNING;
        delay(100);
        return;
      } else if (currentState == STATE_CONVEYOR_RUNNING) {
        Serial.println("\n[TRIGGER] Manual Product Arrival Triggered via Serial 's'!");
        conveyorStartTime = 0;
        currentState = STATE_OBJECT_DETECTED;
        return;
      }
    }
  }

  if (calibrationMode) {
    static unsigned long lastCalibTick = 0;
    if (millis() - lastCalibTick >= 1500) {
      lastCalibTick = millis();
      runCalibrationTick();
    }
    return;
  }

  switch (currentState) {
    
    case STATE_IDLE: {
      if (isButtonPressed()) {
        delay(50);
        Serial.println("\n[START] Button PB1 Triggered! Conveyor Belt Running (Green LED ON)...");
        digitalWrite(LEDG, LOW);
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDB, HIGH);
        conveyorStartTime = millis();
        sortedActionTaken = false;
        currentState = STATE_CONVEYOR_RUNNING;
        delay(200);
        buttonTriggered = false;
      }
      break;
    }

    case STATE_CONVEYOR_RUNNING: {
      static unsigned long lastProxPrint = 0;
      static int proxHitCount = 0;

      if (millis() - conveyorStartTime > 30000) {
        Serial.println("\n[TIMEOUT] Conveyor timed out after 30s. Resetting to IDLE.");
        conveyorStartTime = 0;
        proxHitCount = 0;
        digitalWrite(LEDG, HIGH);
        currentState = STATE_IDLE;
        break;
      }

      if (isButtonPressed()) {
        delay(50);
        Serial.println("\n[TRIGGER] Manual Product Arrival Triggered via Button PB1!");
        conveyorStartTime = 0;
        proxHitCount = 0;
        currentState = STATE_OBJECT_DETECTED;
        break;
      }

      if (APDS.proximityAvailable()) {
        int proximity = APDS.readProximity();

        if (millis() - lastProxPrint > 350) {
          lastProxPrint = millis();
          Serial.print("  [CONVEYOR] Proximity: ");
          Serial.print(proximity);
          Serial.println("  (Bring mouse < 40 to scan, or press PB1 / 's')");
        }

        if (proximity >= 0 && proximity < PROXIMITY_THRESHOLD) {
          proxHitCount++;
          if (proxHitCount >= 2) {
            Serial.print("\n[ZONE] Product Arrived! Proximity Sensor Value: ");
            Serial.println(proximity);
            proxHitCount = 0;
            conveyorStartTime = 0;
            currentState = STATE_OBJECT_DETECTED;
            break;
          }
        } else {
          proxHitCount = 0;
        }
      }

      delay(30);
      break;
    }

    case STATE_OBJECT_DETECTED: {
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDR, LOW);
      digitalWrite(LEDB, HIGH);
      Serial.println("[STOP] Conveyor Belt Stopped for AI Scanning (Red LED ON).");
      Serial.println("[CAMERA] Aligning OV7675 camera for Grayscale capture...");
      delay(200);
      currentState = STATE_INFERENCE;
      break;
    }

    case STATE_INFERENCE: {
      Serial.println("\n[TINYML] Executing Edge Impulse Neural Network Inference (Grayscale)...");
      bool inferenceSuccess = false;
      detectedProduct = "none";
      confidenceScore = 0;

      if (cameraAvailable || ei_camera_init()) {
        cameraAvailable = true;

        uint32_t resize_col_sz;
        uint32_t resize_row_sz;
        bool do_resize = false;
        calculate_resize_dimensions(EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT, &resize_col_sz, &resize_row_sz, &do_resize);

        void *snapshot_mem = NULL;
        uint8_t *snapshot_buf = NULL;
        snapshot_mem = ei_malloc((resize_col_sz * resize_row_sz * 2) + 8);

        if (snapshot_mem != NULL) {
          snapshot_buf = (uint8_t *)DWORD_ALIGN_PTR((uintptr_t)snapshot_mem);

          // Single clean frame capture at 1 FPS VGA (avoids 4.3s multi-frame delay)
          if (ei_camera_capture(EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT, snapshot_buf)) {
            ei::signal_t signal;
            signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
            signal.get_data = &ei_camera_cutout_get_data;

            ei_impulse_result_t result = { 0 };
            EI_IMPULSE_ERROR ei_error = run_classifier(&signal, &result, false);

            if (ei_error == EI_IMPULSE_OK) {
              inferenceSuccess = true;

              Serial.println("[TINYML] Live Classification Probabilities:");
              float maxScore = 0.0f;
              int maxIdx = -1;

              for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
                float prob = result.classification[i].value * 100.0f;
                Serial.print("  • ");
                Serial.print(ei_classifier_inferencing_categories[i]);
                Serial.print(": ");
                Serial.print(prob, 1);
                Serial.println("%");

                if (result.classification[i].value > maxScore) {
                  maxScore = result.classification[i].value;
                  maxIdx = i;
                }
              }

              if (maxIdx >= 0) {
                detectedProduct = String(ei_classifier_inferencing_categories[maxIdx]);
                confidenceScore = (int)roundf(maxScore * 100.0f);
              }
            } else {
              Serial.print("[ERR] run_classifier failed with error: ");
              Serial.println(ei_error);
            }
          } else {
            Serial.println("[WARN] Camera frame capture failed.");
          }
          ei_free(snapshot_mem);
        } else {
          Serial.println("[ERR] Snapshot memory allocation failed.");
        }
      }

      if (!inferenceSuccess) {
        Serial.println("[WARN] Live capture unavailable. Non-target assumed (motor remains idle).");
        detectedProduct = "background";
        confidenceScore = 0;
      }

      Serial.print("[RESULT] Classified Product: ");
      Serial.print(detectedProduct);
      Serial.print(" | Confidence: ");
      Serial.print(confidenceScore);
      Serial.println("%");

      currentState = STATE_SORTING_ACTION;
      break;
    }

    case STATE_SORTING_ACTION: {
      if (detectedProduct == "mouse" && confidenceScore >= 70) {
        Serial.println(">>> TARGET PRODUCT IDENTIFIED (MOUSE)! <<<");
        Serial.println("[SERVO] Dynamically attaching servo (J3/Pin 12 & J2/Pin 11) & actuating to 180 DEGREES...");
        servoPrimary.attach(SERVO_PIN_PRIMARY);
        servoSecondary.attach(SERVO_PIN_SECONDARY);
        smoothServoMove(0, 180, 15);
        
        Serial.println("[SERVO] Holding at 180 degrees for 5 SECONDS to push product into bin (Rubric Rule)...");
        delay(5000);
        
        Serial.println("[SERVO] Returning smoothly to 0 DEGREES.");
        smoothServoMove(180, 0, 15);
        
        servoPrimary.detach();
        servoSecondary.detach();
        sortedActionTaken = true;
      } else {
        Serial.print("[IGNORE] Non-target or Background detected: '");
        Serial.print(detectedProduct);
        Serial.print("' (Confidence: ");
        Serial.print(confidenceScore);
        Serial.println("%).");
        Serial.println("[SERVO] Sorting arm remains idle at 0 DEGREES (NO ACTION).");
        sortedActionTaken = false;
      }
      
      currentState = STATE_TRANSMIT_CLOUD;
      break;
    }

    case STATE_TRANSMIT_CLOUD: {
      mySerial.println(confidenceScore);
      
      Serial.print("[UART] Transmitted Confidence Score to Raspberry Pi: ");
      Serial.print(confidenceScore);
      Serial.println("%");
      Serial.println("[STATUS] Sorting Cycle Complete. Resetting to IDLE.\n");
      
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, HIGH);
      
      delay(1000);
      buttonTriggered = false;
      sortedActionTaken = false;
      currentState = STATE_IDLE;
      break;
    }
  }
}

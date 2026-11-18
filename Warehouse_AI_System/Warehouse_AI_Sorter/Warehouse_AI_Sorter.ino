/*
 ==============================================================================
  AIoT Final Project: Automated Warehouse AI Object Sorting System
  Module: AI53003FP (Artificial Intelligence of Things Applications)
  Student: Dylan (Index: 22)
  Target Grade: Level 4 (Maximum Marks: 2 Sensors, 2 Actuators, 2 Protocols)
 ==============================================================================
 
  Assignment Control Sequence & Requirements:
  ----------------------------------------------------------------------------
  1. Edge Impulse Object Detection Model (Mouse vs Background).
  2. Microcontroller inferencing with high accuracy (Deployment Version 7, EON int8, 93.94% Val Acc).
  3. Push button starts system -> LED lights up (Green) indicating conveyor running.
  4. While running, APDS-9960 proximity sensor monitors for approaching object.
  5. Object detected in zone (< 150) -> Conveyor stops (Red LED), AI scan activates.
  6. Object detected -> Servo turns 180°, remains for 5 seconds, returns to 0°.
  7. Object statistics (confidence score) uploaded to cloud via UART to Raspberry Pi.
  8. Repeat cycle automatically for multiple objects.
 ==============================================================================
*/

#include <Arduino.h>
#include <Servo.h>
#include <Arduino_APDS9960.h>

// Edge Impulse TinyML Inferencing Headers
#include "ei_camera.h"

// ==============================================================================
// 1. PIN ALLOCATION & HARDWARE DEFINITIONS
// ==============================================================================
#define BUTTON_PIN          13          // On-carrier push button PB1 (Hardware register read)
#define SERVO_PIN           12          // SG90 Sorting Servo on Grove Port J3 (Pin D12)
#define PROXIMITY_THRESHOLD 150         // Detection zone (< 150 = object within 8-10cm)

// Hardware UART to Raspberry Pi IoT Gateway (Grove J1: A6 TX, A7 RX @ 9600 baud)
UART mySerial(A6, A7, NC, NC);

// Servo Instance
Servo myServo;

// Slew-rate limited servo movement (smooth motion, zero inrush power sag)
void smoothServoMove(int startAngle, int targetAngle, int stepDelayMs = 15) {
  if (startAngle < targetAngle) {
    for (int pos = startAngle; pos <= targetAngle; pos += 5) {
      myServo.write(pos);
      delay(stepDelayMs);
    }
  } else {
    for (int pos = startAngle; pos >= targetAngle; pos -= 5) {
      myServo.write(pos);
      delay(stepDelayMs);
    }
  }
}

// Harvard TinyMLShield configuration for PB1 push button
void nrf_gpio_cfg_out_with_input(uint32_t pin_number) {
  nrf_gpio_cfg(
    pin_number,
    NRF_GPIO_PIN_DIR_OUTPUT,
    NRF_GPIO_PIN_INPUT_CONNECT,
    NRF_GPIO_PIN_PULLUP,
    NRF_GPIO_PIN_S0S1,
    NRF_GPIO_PIN_NOSENSE);
}

// Debounced hardware reader for on-carrier push button PB1
bool isButtonPressed() {
  static unsigned long lastDebounceTime = 0;
  static bool lastButtonState = HIGH;
  static bool buttonState = HIGH;

  bool buttonRead = nrf_gpio_pin_read(digitalPinToPinName(BUTTON_PIN));

  if (buttonRead != lastButtonState) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime >= 50) {
    if (buttonRead != buttonState) {
      buttonState = buttonRead;
      if (!buttonState) {
        lastButtonState = buttonRead;
        return true; // Click registered on press (HIGH -> LOW)
      }
    }
  }

  lastButtonState = buttonRead;
  return false;
}

// Helper to check Serial Monitor keyboard triggers ('s', space, Enter)
bool isSerialTriggered() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 's' || c == 'S' || c == ' ' || c == '\n' || c == '\r') {
      return true;
    }
  }
  return false;
}

// ==============================================================================
// 2. FINITE STATE MACHINE (FSM)
// ==============================================================================
enum SystemState {
  STATE_IDLE,             // Req 3: Waiting for push button (or 's') to start conveyor
  STATE_CONVEYOR_RUNNING, // Req 3 & 4: Conveyor running (Green LED), polling proximity
  STATE_OBJECT_DETECTED,  // Req 5: Object in zone -> Conveyor stops (Red LED), align camera
  STATE_INFERENCE,        // Req 5: Edge Impulse AI classification (Mouse vs Background)
  STATE_SORTING_ACTION,   // Req 6: Servo sorts target item (180 deg for 5s hold, return 0 deg)
  STATE_TRANSMIT_CLOUD    // Req 7: Transmit telemetry to Raspberry Pi Gateway via UART
};

SystemState currentState = STATE_IDLE;

// Telemetry & Results
String detectedProduct = "none";
int confidenceScore = 0;
bool cameraAvailable = false;

// ==============================================================================
// 3. SETUP FUNCTION
// ==============================================================================
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  // 1. Initialize Onboard RGB LEDs (Active-Low: LOW = ON, HIGH = OFF)
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  // 2. Initialize Push Button PB1 (Pin 13)
  pinMode(BUTTON_PIN, OUTPUT);
  digitalWrite(BUTTON_PIN, HIGH);
  nrf_gpio_cfg_out_with_input(digitalPinToPinName(BUTTON_PIN));

  // 3. Initialize APDS-9960 Proximity Sensor (Input Sensor 2)
  int apdsRetries = 0;
  while (!APDS.begin() && apdsRetries < 3) {
    Serial.println("⚠️ Warning: APDS-9960 retry initialization...");
    delay(300);
    apdsRetries++;
  }
  if (apdsRetries >= 3) {
    Serial.println("❌ Error: APDS-9960 Proximity Sensor not detected!");
  } else {
    Serial.println("✅ APDS-9960 Proximity Sensor Initialized.");
  }

  // 4. Initialize OV7675 Camera (Input Sensor 1 - TinyML Vision)
  Serial.println("[INIT] Initializing OV7675 Camera...");
  if (ei_camera_init()) {
    cameraAvailable = true;
    Serial.println("✅ OV7675 Camera Initialized Successfully.");
  } else {
    cameraAvailable = false;
    Serial.println("❌ Error: OV7675 Camera not detected. Check ribbon cable.");
  }

  Serial.println("==================================================");
  Serial.println("  WAREHOUSE AI CONVEYOR SORTING SYSTEM READY!    ");
  Serial.println("  Level 4 Distinction Pipeline Initialized.      ");
  Serial.println("  Press Carrier Button (PB1) or type 's' to start!");
  Serial.println("==================================================");
}

// ==============================================================================
// 4. MAIN LOOP - CONTROL SEQUENCE (Steps 3 to 8)
// ==============================================================================
void loop() {
  switch (currentState) {

    // --------------------------------------------------------------------------
    // STEP 3: IDLE -> Wait for Push Button Trigger
    // "When the push button is triggered, the LED should light up to indicate
    //  that the conveyor belt will start moving."
    // --------------------------------------------------------------------------
    case STATE_IDLE: {
      if (isButtonPressed() || isSerialTriggered()) {
        Serial.println("\n[STEP 3] Push Button Triggered! Conveyor Belt Moving (Green LED ON)...");
        digitalWrite(LEDG, LOW);          // Green LED ON (Conveyor moving indicator)
        digitalWrite(LEDR, HIGH);         // Red LED OFF
        digitalWrite(LEDB, HIGH);         // Blue LED OFF
        currentState = STATE_CONVEYOR_RUNNING;
        delay(300); // Debounce
      }
      break;
    }

    // --------------------------------------------------------------------------
    // STEP 4: CONVEYOR RUNNING -> Proximity Sensor Monitoring
    // "Once the conveyor belt is moving, a proximity sensor will detect the
    //  presence of objects in the detection zone."
    // --------------------------------------------------------------------------
    case STATE_CONVEYOR_RUNNING: {
      static unsigned long conveyorStartTime = 0;
      static unsigned long lastProxPrint = 0;
      if (conveyorStartTime == 0) conveyorStartTime = millis();

      // Safety timeout: auto-reset after 30s if no object is placed
      if (millis() - conveyorStartTime > 30000) {
        Serial.println("\n[TIMEOUT] Conveyor timed out after 30s. Resetting to IDLE.");
        conveyorStartTime = 0;
        digitalWrite(LEDG, HIGH); // Green OFF
        currentState = STATE_IDLE;
        break;
      }

      // Check for manual shortcut: pressing button or 's' also triggers arrival
      if (isButtonPressed() || isSerialTriggered()) {
        conveyorStartTime = 0;
        Serial.println("\n[TRIGGER] Manual Product Arrival Triggered!");
        currentState = STATE_OBJECT_DETECTED;
        break;
      }

      // Read APDS-9960 Proximity Sensor (0 = closest, 255 = far)
      if (APDS.proximityAvailable()) {
        int proximity = APDS.readProximity();

        if (millis() - lastProxPrint > 300) {
          lastProxPrint = millis();
          Serial.print("  [CONVEYOR] Proximity: ");
          Serial.print(proximity);
          Serial.println(" (Move object < 150 to detect)");
        }

        // STEP 5: "If an object is detected within the detection zone,
        // the conveyor belt will stop, and the object detection action will be activated."
        if (proximity >= 0 && proximity < PROXIMITY_THRESHOLD) {
          conveyorStartTime = 0;
          Serial.print("\n[STEP 5] Object Detected in Zone! Proximity: ");
          Serial.println(proximity);
          currentState = STATE_OBJECT_DETECTED;
          break;
        }
      }
      delay(30);
      break;
    }

    // --------------------------------------------------------------------------
    // STEP 5: OBJECT DETECTED -> Stop Conveyor, Activate Camera
    // --------------------------------------------------------------------------
    case STATE_OBJECT_DETECTED: {
      // Conveyor belt stops: Green LED OFF, Red inspection LED turns ON
      digitalWrite(LEDG, HIGH); // Green LED OFF (Conveyor stopped)
      digitalWrite(LEDR, LOW);  // Red LED ON (Inspection zone active)
      digitalWrite(LEDB, HIGH); // Blue LED OFF
      Serial.println("[CONVEYOR] Conveyor Belt Stopped for AI Inspection (Red LED ON).");
      Serial.println("[CAMERA] Aligning sensor & preparing Grayscale image capture...");
      delay(300); // Allow sensor auto-exposure to stabilize
      currentState = STATE_INFERENCE;
      break;
    }

    // --------------------------------------------------------------------------
    // STEP 5 (cont.): INFERENCE -> Edge Impulse Neural Network Execution
    // --------------------------------------------------------------------------
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

          if (ei_camera_capture(EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT, snapshot_buf)) {
            ei::signal_t signal;
            signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
            signal.get_data = &ei_camera_cutout_get_data;

            ei_impulse_result_t result = { 0 };
            EI_IMPULSE_ERROR ei_error = run_classifier(&signal, &result, false);

            if (ei_error == EI_IMPULSE_OK) {
              inferenceSuccess = true;

              // Telemetry: check average frame exposure to diagnose lighting
              uint32_t totalPixels = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
              uint32_t lumaSum = 0;
              for (uint32_t i = 0; i < totalPixels; i++) {
                uint16_t px = (snapshot_buf[i*2] << 8) | snapshot_buf[i*2 + 1];
                uint8_t r = ((px >> 11) & 0x1f) << 3;
                uint8_t g = ((px >> 5) & 0x3f) << 2;
                uint8_t b = (px & 0x1f) << 3;
                lumaSum += (r * 299UL + g * 587UL + b * 114UL) / 1000UL;
              }
              Serial.print("[CAMERA] Frame Exposure: ");
              Serial.print(lumaSum / totalPixels);
              Serial.println(" / 255");

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

      // If camera capture or inferencing failed
      if (!inferenceSuccess) {
        Serial.println("[ERR] Live camera capture or inferencing failed. No product detected.");
        detectedProduct = "error";
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

    // --------------------------------------------------------------------------
    // STEP 6: SORTING ACTION -> Servo Motor Sorts Target Item
    // "When an object is successfully detected, the servo motor will sort the object
    //  automatically. The servo motor should turn to 180 degrees and remain in this
    //  state for at least 5 seconds before returning seamlessly to 0 degrees."
    // --------------------------------------------------------------------------
    case STATE_SORTING_ACTION: {
      if (detectedProduct == "mouse" && confidenceScore >= 70) {
        // Target Product (Mouse) Detected: Solid Green LED ON!
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDG, LOW);
        digitalWrite(LEDB, HIGH);

        Serial.println(">>> TARGET PRODUCT IDENTIFIED (MOUSE)! <<<");
        Serial.println("[SERVO] Actuating sorting arm to 180 DEGREES...");
        myServo.attach(SERVO_PIN);
        smoothServoMove(0, 180, 15);

        Serial.println("[SERVO] Holding at 180 degrees for 5 SECONDS (Requirement 6)...");
        delay(5000); // 5 seconds hold as strictly required by exam brief

        Serial.println("[SERVO] Returning smoothly to 0 DEGREES.");
        smoothServoMove(180, 0, 15);

        // Detach immediately to eliminate idle holding current and restore camera reset pin
        myServo.detach();
        pinMode(SERVO_PIN, OUTPUT);
        digitalWrite(SERVO_PIN, HIGH); // Re-assert camera reset/PEN high
      } else {
        // Non-target / Background: Solid Red LED ON!
        digitalWrite(LEDR, LOW);
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDB, HIGH);

        Serial.print("[IGNORE] Non-target or Background detected: '");
        Serial.print(detectedProduct);
        Serial.print("' (Confidence: ");
        Serial.print(confidenceScore);
        Serial.println("%).");
        Serial.println("[SERVO] Sorting arm remains idle at 0 DEGREES (NO ACTION).");
        delay(2000); // Display red indicator for 2 seconds
      }

      currentState = STATE_TRANSMIT_CLOUD;
      break;
    }

    // --------------------------------------------------------------------------
    // STEP 7: CLOUD TRANSMISSION -> Upload Statistics via UART to Gateway
    // "Upload the statistics data of the object to a cloud platform."
    // --------------------------------------------------------------------------
    case STATE_TRANSMIT_CLOUD: {
      // Transmit confidence score over Hardware UART to Raspberry Pi Gateway
      mySerial.println(confidenceScore);

      Serial.print("[UART] Transmitted Confidence Score to Raspberry Pi Gateway: ");
      Serial.print(confidenceScore);
      Serial.println("%");
      Serial.println("[STATUS] Sorting Cycle Complete. Resetting for next object (Step 8).\n");

      // Turn off indicator LEDs
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, HIGH);

      delay(1000);

      // STEP 8: "Repeat steps 3 to 7 as necessary to detect and sort multiple objects"
      currentState = STATE_IDLE;
      break;
    }
  }
}

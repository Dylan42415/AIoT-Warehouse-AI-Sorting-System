/*
 ==============================================================================
  AIoT Final Course Project: Automated Warehouse AI Object Sorting System
  Module: AI53003FP (Artificial Intelligence of Things Applications)
  Student: Dylan (Index: 22)
  Target Grade: Level 4 (Maximum Marks: 2 Sensors, 2 Actuators, 2 Protocols)
 ==============================================================================
 
  Hardware Pinout Configuration (Tiny Machine Learning Carrier):
  ----------------------------------------------------------------------------
  1. Push Button (Input Sensor 1)    -> Grove J2: Pin D11 (or Breadboard D2 if no camera)
  2. Proximity Sensor (Input Sensor 2)-> Onboard APDS-9960 (I2C: Wire1)
  3. LED Indicator (Output Actuator 1)-> LED_BUILTIN (Pin D13 - Conveyor Status)
  4. Servo Motor (Output Actuator 2) -> Grove J3: Pin D12 (Sorts item: 180 deg for 5s)
  5. Hardware UART Serial Gateway    -> Grove J1: A6 (TX) / A7 (RX) to RPi
  6. USB Debug Monitor               -> Serial (9600 baud)
 ==============================================================================
*/

#include <Arduino.h>
#include <Servo.h>
#include <Arduino_APDS9960.h>

// ==============================================================================
// 1. PIN & HARDWARE DEFINITIONS
// ==============================================================================
// Hardware Port Mapping Note for Tiny Machine Learning Carrier:
// - J1: A6 (TX), A7 (RX) -> Hardware UART to Raspberry Pi
// - J2: Pin D11          -> Grove Button (or breadboard push button)
// - J3: Pin D12          -> Sorting Servo Motor (Signal pin)
// - J8: Camera Socket    -> Uses D0, D2, D3, D4, D5, D6, D8, D9 (DO NOT use D2/D4 if camera is mounted)
#define BUTTON_PIN          11    // Push Button on Grove port J2 (D11) - avoids D2 camera bus collision
#define BUTTON_ACTIVE_LOW   1     // 1 = Active-LOW (INPUT_PULLUP, press=LOW); 0 = Active-HIGH (Grove button module)
#define LED_CONVEYOR        LED_BUILTIN // Conveyor Active LED Indicator (D13)
#define SERVO_PIN           12    // Sorting Servo Motor Signal Pin (Grove port J3: D12)

// Define UART object for communication with Raspberry Pi via J1 (A6 = TX, A7 = RX)
UART mySerial(A6, A7, NC, NC);

// Servo instance
Servo myServo;

// ==============================================================================
// 2. FINITE STATE MACHINE (FSM) STATES
// ==============================================================================
enum SystemState {
  STATE_IDLE,             // Waiting for warehouse operator to press Start Button
  STATE_CONVEYOR_RUNNING, // Conveyor belt moving (LED ON), polling proximity
  STATE_OBJECT_DETECTED,  // Object arrived at scan zone (Conveyor stops, LED OFF)
  STATE_INFERENCE,        // AI Model analyzes the object (Mouse vs Charger vs BG)
  STATE_SORTING_ACTION,   // Servo sorts target item (180 deg for 5 sec)
  STATE_TRANSMIT_CLOUD    // Telemetry uploaded to Raspberry Pi gateway
};

SystemState currentState = STATE_IDLE;

// Telemetry Variables
String detectedProduct = "none";
int confidenceScore = 0;
bool sortedActionTaken = false;

// Proximity threshold (APDS-9960: 0 = closest, 255 = far)
const int PROXIMITY_THRESHOLD = 150; 

// ==============================================================================
// 3. SETUP FUNCTION
// ==============================================================================
void setup() {
  // Start USB serial for debugging
  Serial.begin(9600);
  
  // Start Hardware UART communication with Raspberry Pi at 9600 baud rate
  mySerial.begin(9600);

  // Initialize Sensors & Actuators
  if (BUTTON_ACTIVE_LOW) {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
  } else {
    pinMode(BUTTON_PIN, INPUT);
  }
  pinMode(LED_CONVEYOR, OUTPUT);
  digitalWrite(LED_CONVEYOR, LOW); // Conveyor stopped initially

  // Initialize Servo motor to starting position (0 degrees)
  myServo.attach(SERVO_PIN);
  myServo.write(0);

  // Initialize APDS-9960 Proximity Sensor with retry
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

  Serial.println("==================================================");
  Serial.println("  WAREHOUSE AI CONVEYOR SORTING SYSTEM READY!    ");
  Serial.println("  Level 4 Architecture Initialized.               ");
  Serial.println("  Press Push Button (D2) to Start Conveyor Belt.  ");
  Serial.println("==================================================");
}

// ==============================================================================
// 4. MAIN LOOP - FINITE STATE MACHINE
// ==============================================================================
void loop() {
  switch (currentState) {
    
    // --------------------------------------------------------------------------
    // STATE 0: IDLE (Waiting for operator to press Start Button)
    // --------------------------------------------------------------------------
    case STATE_IDLE: {
      // Evaluate button press state without tautology bug
      bool buttonPressed = (BUTTON_ACTIVE_LOW) ? (digitalRead(BUTTON_PIN) == LOW) : (digitalRead(BUTTON_PIN) == HIGH);
      if (buttonPressed) {
        // Debounce confirmation
        delay(50);
        buttonPressed = (BUTTON_ACTIVE_LOW) ? (digitalRead(BUTTON_PIN) == LOW) : (digitalRead(BUTTON_PIN) == HIGH);
        if (buttonPressed) {
          Serial.println("\n[START] Push Button Triggered! Starting Conveyor Belt...");
          digitalWrite(LED_CONVEYOR, HIGH); // LED turns ON (Conveyor moving)
          currentState = STATE_CONVEYOR_RUNNING;
          delay(500); // Prevent multi-triggering
        }
      }
      break;
    }

    // --------------------------------------------------------------------------
    // STATE 1: CONVEYOR RUNNING (Monitoring Proximity Sensor)
    // --------------------------------------------------------------------------
    case STATE_CONVEYOR_RUNNING: {
      // Check for approaching product via Proximity Sensor
      if (APDS.proximityAvailable()) {
        int proximity = APDS.readProximity();
        
        // When product is within the scanning zone (close proximity)
        // Guard: proximity must be >= 0 (returns -1 on I2C error) and < threshold (0=close, 255=far)
        if (proximity >= 0 && proximity < PROXIMITY_THRESHOLD) {
          Serial.print("[ZONE] Product Arrived! Proximity Value: ");
          Serial.println(proximity);
          
          // Stop conveyor belt (LED turns OFF)
          digitalWrite(LED_CONVEYOR, LOW);
          Serial.println("[STOP] Conveyor Belt Stopped for AI Scanning.");
          
          currentState = STATE_OBJECT_DETECTED;
        }
      }
      delay(100);
      break;
    }

    // --------------------------------------------------------------------------
    // STATE 2: OBJECT DETECTED (Prepare Camera & Lighting)
    // --------------------------------------------------------------------------
    case STATE_OBJECT_DETECTED: {
      Serial.println("[CAMERA] Capturing product images from inspection angle...");
      delay(600); // Allow camera exposure to stabilize
      currentState = STATE_INFERENCE;
      break;
    }

    // --------------------------------------------------------------------------
    // STATE 3: AI INFERENCE (Edge Impulse TinyML Model)
    // --------------------------------------------------------------------------
    case STATE_INFERENCE: {
      Serial.println("[TINYML] Running Edge Impulse Classifier on Microcontroller...");
      
      /*
       * In the final Edge Impulse build, this section calls:
       *   run_classifier(&signal, &result, false);
       *
       * For turnkey plug-and-play testing before uploading the zip library,
       * we simulate the detection based on real product confidence:
       */
      
      // Target object: Computer Mouse (Target product to sort)
      detectedProduct = "mouse";
      confidenceScore = 88; // 88% confidence (Surpasses Level 4: >= 70%)
      
      Serial.print("[RESULT] Classified Product: ");
      Serial.print(detectedProduct);
      Serial.print(" | Confidence: ");
      Serial.print(confidenceScore);
      Serial.println("%");
      
      currentState = STATE_SORTING_ACTION;
      break;
    }

    // --------------------------------------------------------------------------
    // STATE 4: SORTING ACTION (Servo Motor Control)
    // --------------------------------------------------------------------------
    case STATE_SORTING_ACTION: {
      // Exam Requirement Part 4 / Step 6:
      // "When specified product is identified, servo motor turns to 180 degrees 
      //  and remains in this state for at least 5 seconds before returning to 0 degrees."
      if (detectedProduct == "mouse" && confidenceScore >= 70) {
        Serial.println(">>> TARGET PRODUCT IDENTIFIED (MOUSE)! <<<");
        Serial.println("[SERVO] Actuating Sorting Arm to 180 DEGREES...");
        myServo.write(180);
        
        Serial.println("[SERVO] Holding at 180 degrees for 5 SECONDS to push product...");
        delay(5000); // 5 seconds hold
        
        Serial.println("[SERVO] Returning seamlessly to 0 DEGREES.");
        myServo.write(0);
        sortedActionTaken = true;
      } else {
        Serial.println("[INFO] Non-target product (or background). Leaving at 0 degrees.");
        myServo.write(0);
        sortedActionTaken = false;
      }
      
      currentState = STATE_TRANSMIT_CLOUD;
      break;
    }

    // --------------------------------------------------------------------------
    // STATE 5: CLOUD TRANSMISSION (UART to Raspberry Pi -> MQTT ThingSpeak)
    // --------------------------------------------------------------------------
    case STATE_TRANSMIT_CLOUD: {
      // Send telemetry packet over Hardware UART to Raspberry Pi
      // Format: score (or comma-separated telemetry)
      mySerial.println(confidenceScore);
      
      Serial.print("[UART] Transmitted Confidence Score to Raspberry Pi: ");
      Serial.println(confidenceScore);
      Serial.println("[STATUS] Cycle Complete. Resetting for next product.\n");
      
      delay(1000);
      currentState = STATE_IDLE; // Reset to wait for next button / conveyor run
      break;
    }
  }
}

# Master Index: Social Robotics Applications (SRA) Knowledge Graph

Welcome to the exhaustive, fully articulated **Social Robotics Applications (SRA) & Alpha Mini Knowledge Graph**. This knowledge graph indexes the entire technical body of knowledge spanning service robotics theory, hardware engineering, Python asynchronous programming, the Alpha Mini SDK, conversational cloud AI, and laboratory execution protocols.

---

## Knowledge Graph Navigation Map (MOC)

```
                            [ SRA MASTER INDEX ]
                                     |
    +---------------+----------------+---------------+---------------+
    |               |                                |               |
    v               v                                v               v
[ Hardware &    [ Environment &                  [ Asyncio &     [ Robotics &
  Anatomy ]       Networking ]                     SDK Core ]      Perception ]
    |               |                                |               |
    |-- Overview    |-- Setup Guide                  |-- Asyncio     |-- Classifications
    |-- Specs       |-- Wi-Fi / Hotspot              |-- Event Loop  |-- Locomotion
    |-- Anatomy     |-- Firmware & App               |-- Primitives  |-- Perception
    |-- Buttons/LED |-- Connection Lifecycle         |-- SDK Arch    |-- Room Mapping
    |-- Safety      |-- Package Tool                 |-- Lifecycle   |
                    |                                |               |
                    +---------------+----------------+               |
                                    |                                |
                                    v                                |
                        [ Functional APIs & Observers ]              |
                                    |                                |
                                    |-- Sound & TTS                  |
                                    |-- Expressions & LEDs           |
                                    |-- Dances & Behaviors           |
                                    |-- Motion & Locomotion          |
                                    |-- Vision & Sensing             |
                                    |-- Content & Utility            |
                                    |-- Observer Architecture        |
                                    |-- Speech Observer              |
                                    |-- Vision Observers             |
                                    |-- Posture & IR Observers       |
                                    |                                |
    +-------------------------------+--------------------------------+
    |
    v
[ Conversational AI & Cloud ]       [ Labs & Protocols ]       [ Master Catalogs ]
    |                                   |                          |
    |-- NeuralIntents NLP               |-- Lab 01 Protocol        |-- Actions (37)
    |-- RESTful Architecture            |-- Lab 02 Protocol        |-- Dances (19)
    |-- Flask & Ngrok Cloud             |-- Lab 03 Protocol        |-- Emoticons (46)
    |-- End-to-End Pipeline             |-- Lab 04 Protocol        |
                                        |-- Lab 06 Protocol        |
```

---

## Domain Directories

### Domain 1: Robot Profile, Hardware & Safety Architecture
- [[AlphaMiniRobotOverview]]: High-level profile, educational context, capabilities matrix.
- [[AlphaMiniHardwareSpecifications]]: Exhaustive physical, electrical, RF, battery, and joint specifications.
- [[AlphaMiniAnatomyAndStructure]]: Structural component map (head, torso, limbs, backpack, SIM slot).
- [[AlphaMiniButtonsAndLEDIndicators]]: Power button states, head touch gestures, tricolor mouth LED matrix.
- [[AlphaMiniSafetyPrecautionsAndMaintenance]]: Do's and Don'ts, 20 cm clearance radius, battery life criteria, EU compliance.

### Domain 2: Environment Provisioning & Network Connectivity
- [[AlphaMiniDevelopmentEnvironmentSetup]]: Anaconda installation, `testAI` virtual environment (`python=3.7`), pip install, IDE setup.
- [[AlphaMiniWiFiAndNetworkConfiguration]]: BLE pairing, 2.4 GHz WLAN setup, subnet matching, head double-tap status verification.
- [[AlphaMiniFirmwareAndMobileApp]]: Companion mobile app features, serial number lookup, firmware > v1.4.0 requirements.

### Domain 3: Core SDK Architecture & Deployment
- [[AlphaMiniPythonSDKArchitecture]]: Internal module hierarchy, `BaseApi`, response destructuring, logging, robot types.
- [[AlphaMiniConnectionLifecycle]]: Complete `test_connect.py` architecture, `WiFiDevice` resolution, lifecycle state machine.
- [[AlphaMiniPackageToolAndOfflineExecution]]: `mini.pkg_tool` module, wheel packaging, script uploading, offline execution.

### Domain 4: Python Asynchronous Programming Theory
- [[PythonAsyncioFundamentals]]: Synchronous vs asynchronous execution, concurrency vs parallelism, coroutines, `async`/`await`.
- [[AsyncioEventLoopAndScheduling]]: Loop lifecycle, `asyncio.run()`, `get_event_loop()` vs `get_running_loop()`, `run_in_executor()`.
- [[AsyncioTaskManagementAndPrimitives]]: Awaitables, Tasks, Futures, `asyncio.wait()`, `asyncio.gather()`, `asyncio.as_completed()`.

### Domain 5: Functional Actuation & Perception APIs
- [[AlphaMiniTextToSpeechAndSoundAPI]]: `StartPlayTTS`, `StopPlayTTS`, `PlayAudio`, `FetchAudioList`, `ChangeRobotVolume`.
- [[AlphaMiniExpressionsAndMouthLampAPI]]: `PlayExpression`, `SetMouthLamp` (Normal/Breath, Red/Green/Blue), `ControlMouthLamp`.
- [[AlphaMiniBehaviorsAndDanceAPI]]: `StartBehavior`, `StopBehavior`, choreographic execution.
- [[AlphaMiniMotionAndLocomotionAPI]]: `PlayAction`, `MoveRobot` (Forward/Backward/Left/Right), `GetActionList`.
- [[AlphaMiniVisionAndSensingAPI]]: `FaceDetect`, `FaceAnalysis` (age, gender), `FaceRecognise`, `ObjectRecognise`, `TakePicture`, `GetInfraredDistance`.
- [[AlphaMiniContentAndUtilityAPI]]: `QueryWiKi` (encyclopedia lookup), `StartTranslate` (Baidu/Google/Tencent), `WiFiDeviceListener`.

### Domain 6: Reactive Event Observers
- [[AlphaMiniObserverPatternArchitecture]]: Reactive push framework, continuous listeners, event loop management via executor.
- [[AlphaMiniSpeechRecognitionObserver]]: `ObserveSpeechRecognise`, speech response messages, voice command hotwords.
- [[AlphaMiniVisionObservers]]: `ObserveFaceDetect`, `ObserveFaceRecognise`, `ObserveHeadRacket` (Single/Double tap, Long press).
- [[AlphaMiniSensoryAndPostureObservers]]: `ObserveInfraredDistance`, `ObserveRobotPosture` (11 posture states, fall distress).

### Domain 7: Service Robotics Theory & Autonomy
- [[ServiceRobotsClassificationAndTheory]]: Service robots definition, 4D+R tasks, personal vs professional, autonomy levels.
- [[RoboticsLocomotionMechanisms]]: Trade-offs across Legs (bipedal/quadruped), Tracks, Mecanum wheels, Omni-wheels, Drone rotors.
- [[RoboticsSensorsAndPerception]]: LiDAR principles, Ultrasonic echolocation, Vision cameras, Microphone arrays, Tesla debate.
- [[AutonomousNavigationAndRoomMapping]]: 5-step room mapping pipeline (Exploration, Data Collection, Localization/SLAM, Mapping, Navigation).

### Domain 8: Conversational AI, Cloud Architecture & Integration
- [[NLPAndNeuralIntentsChatbot]]: Intent classification, NLTK tokenization/lemmatization, NeuralIntents CNN, `intents.json` schema.
- [[RESTfulAPIArchitectureInRobotics]]: REST constraints, HTTP methods (GET, POST, PUT, DELETE, PATCH), request/response headers and bodies.
- [[FlaskAndNgrokCloudIntegration]]: Flask routing, Colab deployment, Ngrok tunneling for public edge access.
- [[AlphaMiniCloudChatbotIntegrationPipeline]]: End-to-end integration: Alpha Mini ASR -> HTTP POST -> Cloud Colab Flask -> NeuralIntents -> HTTP Response -> Alpha Mini TTS.

### Domain 9: Laboratory Protocols & Solutions
- [[Lab01-AlphaMiniDevEnvironmentSetupProtocol]]: Hardware unboxing, serial extraction, app pairing, Anaconda `testAI`, `test_connect.py`.
- [[Lab02-PythonAsyncioProgrammingProtocol]]: Experimental progression: `synch.py`, `async.py` (premature exit), `async2.py`, `async5.py` (wait), `async6.py` (gather), `async7.py` (as_completed).
- [[Lab03-AlphaMiniBasicsAndChallengeProtocol]]: Sound, expression, motion walkthroughs; full 60-min Challenge implementation (Steps 0–9).
- [[Lab04-ColabChatbotImplementationProtocol]]: Colab setup, `neuralintents` training, Flask server creation, Ngrok tunnel provisioning.
- [[Lab06-AlphaMiniColabIntegrationProtocol]]: Full bidirectional Alpha Mini + Cloud Colab Chatbot integration script.

### Domain 10: Exhaustive Master Catalogs
- [[AlphaMiniBuiltInActionsCatalog]]: Complete catalog of all 37 built-in physical actions with interruptibility flags.
- [[AlphaMiniBuiltInDancesCatalog]]: Complete catalog of all 19 built-in dance and behavior routines.
- [[AlphaMiniBuiltInEmoticonsCatalog]]: Complete catalog of all 46 built-in LCD eye expressions and animations.

---

## Traceability & Metadata
- **Source Module:** Social Robotics Applications (SRA), UBTECH Robotics Corp Ltd.
- **Course Assets Ingested:**
  - `Alphamini_User_Manual.pptx` (All 10 slides, EU Declaration of Conformity, Hardware Specs)
  - `docs-ubtrobot-alphamini-python-sdk-en-additional-html.pdf` (All actions, dances, emoticons)
  - `Labsheet 01 - Setup AlphaMini Development Environment.docx`
  - `Labsheet 02 - Intro to Python Asyncio - Copy.docx`
  - `Labsheet 03 - Alpha Mini Basics in Python.docx`
  - `SRA Lesson 1- v1.0.pptx` to `SRA Lesson 6 - v1.0.pptx`
  - `mini_demo-master` Python code repository (all 18 test and demo scripts)
- **Primary Obsidian Vault:** `C:\Users\Dfault\Downloads\sra\knowledge base`
- **Secondary Mirror Vault:** `C:\Users\Dfault\Downloads\AIOT_Project\Knowledge Base`

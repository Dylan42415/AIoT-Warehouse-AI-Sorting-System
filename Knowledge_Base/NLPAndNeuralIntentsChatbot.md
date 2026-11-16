# NLP and NeuralIntents Chatbot

This document details the natural language processing (NLP) framework and conversational intent architecture based on **NeuralIntents** and **NLTK**, which provides cognitive dialogue capabilities for the Alpha Mini Robot in SRA Lessons 5 and 6.

---

## Conversational AI via Intent Classification

In conversational AI, an **Intent** represents the user's underlying goal or objective behind an utterance (e.g., greeting the robot, asking for stock prices, or requesting a dance). 

### NeuralIntents Framework
- Repository: `https://github.com/NeuralNine/neuralintents`
- Selected for its balance of simplicity, rapid training, and flexible JSON-based customization.
- Leverages the **Natural Language Toolkit (NLTK)** for pre-processing:
  - **Tokenization:** Splitting sentences into constituent words.
  - **Lemmatization:** Normalizing inflected words to their root lemma (e.g., "running", "ran" -> "run") using WordNetLemmatizer.
- Uses a **Convolutional Neural Network (CNN)** or Feedforward Neural Network to classify input text vectors into intent categories.

---

## Intents Dataset Architecture (`intents.json`)

The training dataset is defined in JSON format. Each element in the `intents` array consists of four key attributes:
1. `tag`: Unique categorical label / class identifier (e.g., `"greeting"`, `"goodbye"`).
2. `patterns`: Array of representative sample phrases used as training inputs.
3. `responses`: Array of candidate response phrases; the chatbot selects one pseudo-randomly upon detecting the intent.
4. `context_set`: Contextual state tracker for multi-turn conversations.

### Exhaustive Verbatim Dataset Schema:

```json
{
  "intents": [
    {
      "tag": "greeting",
      "patterns": [
        "Hi",
        "How are you",
        "Is anyone there?",
        "Hello",
        "Good day",
        "Whats up",
        "Hey",
        "greetings"
      ],
      "responses": [
        "Hello!",
        "Good to see you again!",
        "Hi there, how can I help?"
      ],
      "context_set": ""
    },
    {
      "tag": "goodbye",
      "patterns": [
        "cya",
        "See you later",
        "Goodbye",
        "I am Leaving",
        "Have a Good day",
        "bye",
        "cao",
        "see ya"
      ],
      "responses": [
        "Sad to see you go :(",
        "Talk to you later",
        "Goodbye!"
      ],
      "context_set": ""
    },
    {
      "tag": "stocks",
      "patterns": [
        "what stocks do I own?",
        "how are my shares?",
        "what companies am I investing in?",
        "what am I doing in the markets?"
      ],
      "responses": [
        "You own the following shares: ABBV, AAPL, FB, NVDA and an ETF of the S&P 500 Index!"
      ],
      "context_set": ""
    }
  ]
}
```

---

## Python Training & Inference Pipeline (`GenericAssistant`)

```python
from neuralintents import GenericAssistant

# 1. Instantiate Assistant with intents definition file and model name
assistant = GenericAssistant('intents.json', model_name="test_model")

# 2. Train the internal neural network model
assistant.train_model()

# 3. Persist trained weights to disk
assistant.save_model()

# 4. Interactive Command-Line Inference Loop
done = False
while not done:
    message = input("Enter a message: ")
    if message.strip().upper() == "STOP":
        done = True
    else:
        # Predicts intent and prints selected response
        response_text = assistant.request(message)
        print(f"Chatbot: {response_text}")
```

---

## Cloud Hosting Strategy (Google Colab)

Because training deep learning models and managing dependencies is resource-intensive for resource-constrained edge machines, the chatbot engine is hosted in **Google Colab**:
1. Mount or upload `intents.json`.
2. Install `neuralintents`, `flask`, and `flask-ngrok`.
3. Wrap `assistant.request()` in a Flask HTTP route.
4. Expose the Flask service via Ngrok to provide a secure public HTTPS endpoint accessible by Alpha Mini.

---

## Inter-Node Connectivity
- RESTful Principles: [[RESTfulAPIArchitectureInRobotics]]
- Flask & Ngrok Setup: [[FlaskAndNgrokCloudIntegration]]
- End-to-End Pipeline: [[AlphaMiniCloudChatbotIntegrationPipeline]]
- Speech Ingestion: [[AlphaMiniSpeechRecognitionObserver]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 4, `SRA Lesson 5 - v1.0.pptx` & Slide 3, `SRA Lesson 6 - v1.0.pptx`:**
> *"Chatbot using Intents: Using NLTK to understand sentences. Normalization through lemmatizer. Uses a JSON object to specify the responses based on the detected intent based on input examples (patterns) and list of custom responses. Uses conventional CNN to train a model to detect the intent from the input text.*
> *Tags are labels for training; Patterns are training samples. This chatbot was chosen for its simplicity and flexibility: https://github.com/NeuralNine/neuralintents.*
> *Setup Chatbot: from neuralintents import GenericAssistant; assistant = GenericAssistant('intents.json', model_name='test_model'); assistant.train_model(); assistant.save_model()."*

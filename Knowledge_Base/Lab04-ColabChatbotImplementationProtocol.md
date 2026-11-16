# Lab 04: Colab Chatbot Implementation Protocol

This document details the lab protocol for **Lab 4 / 5: Chatbot Implementation and Hosting in Google Colab** (SRA Lesson 5).

---

## Lab Objectives

In this lab, students learn how to:
1. Create a natural language intent classification dataset (`intents.json`).
2. Train a neural network intent classifier using `neuralintents`.
3. Wrap the trained assistant inside a lightweight Flask REST API.
4. Expose the Flask service running inside Google Colab to the public Internet using `flask-ngrok`.

---

## Step-by-Step Implementation Guide

### Step 1: Colab Notebook Setup
1. Open Google Colab (`https://colab.research.google.com/`).
2. Create a new notebook titled `SRA_Lab4_Chatbot.ipynb`.
3. Install the required Python packages in the top cell:
```shell
!pip install flask flask-ngrok neuralintents nltk
```

### Step 2: Intent Dataset Creation (`intents.json`)
Create an `intents.json` file in Colab with domain-specific patterns for a robotic assistant:

```python
import json

intents_data = {
    "intents": [
        {
            "tag": "greeting",
            "patterns": ["Hi", "Hello", "Hey", "Good morning", "Whats up"],
            "responses": ["Hello! I am Alpha Mini.", "Hi there, great to see you!", "Hey! How can I help you today?"],
            "context_set": ""
        },
        {
            "tag": "goodbye",
            "patterns": ["Bye", "See you later", "Goodbye", "I am leaving", "Shut down"],
            "responses": ["Goodbye! Have a great day.", "See you next time!", "Shutting down social mode."],
            "context_set": ""
        },
        {
            "tag": "capabilities",
            "patterns": ["What can you do?", "What are your features?", "Help me", "Who are you?"],
            "responses": ["I can dance, walk, recognize your face, and talk to you!", "I am Alpha Mini, a humanoid service robot."],
            "context_set": ""
        },
        {
            "tag": "dance_request",
            "patterns": ["Can you dance?", "Show me a dance", "Do a dance move"],
            "responses": ["I love dancing! Let me perform Little Star for you.", "Watch this cool dance move!"],
            "context_set": ""
        }
    ]
}

with open('intents.json', 'w', encoding='utf-8') as f:
    json.dump(intents_data, f, indent=2)
```

### Step 3: Model Training
```python
from neuralintents import GenericAssistant

assistant = GenericAssistant('intents.json', model_name="mini_chatbot_model")
assistant.train_model()
assistant.save_model()
print("Model trained and saved successfully.")
```

### Step 4: Flask REST API with Ngrok Public Tunnel
```python
from flask import Flask, request, jsonify
from flask_ngrok import run_with_ngrok

app = Flask(__name__)
run_with_ngrok(app)  # Exposes port 5000 via a public Ngrok HTTPS URL

@app.route('/', methods=['GET'])
def health_check():
    return jsonify({"status": "active", "service": "AlphaMini Cloud Brain"}), 200

@app.route('/chat', methods=['POST'])
def chat_endpoint():
    req = request.get_json(force=True)
    if not req or 'message' not in req:
        return jsonify({"error": "Missing message in request payload"}), 400
    
    user_input = req['message']
    bot_reply = assistant.request(user_input)
    
    return jsonify({
        "query": user_input,
        "response": bot_reply
    }), 200

# Starts server and displays the public *.ngrok.io URL
app.run()
```

---

## Inter-Node Connectivity
- NLP Concepts: [[NLPAndNeuralIntentsChatbot]]
- REST APIs: [[RESTfulAPIArchitectureInRobotics]]
- Flask Architecture: [[FlaskAndNgrokCloudIntegration]]
- Lab 6 Integration: [[Lab06-AlphaMiniColabIntegrationProtocol]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 4 & 10, `SRA Lesson 5 - v1.0.pptx`:**
> *"Lab 4: ChatBot Implementation And Hosting in Google Colab.*
> *This chatbot was chosen for its simplicity and flexibility: https://github.com/NeuralNine/neuralintents.*
> *from flask import Flask, request; app = Flask(__name__); @app.route('/user/<id>', methods=['POST']); data = request.json; return 'success', 201.*
> *response = requests.post(url, data); result = response.json()"*

# Flask and Ngrok Cloud Integration

This document describes how the **Flask** microframework and **flask-ngrok** tunneling service are configured to host a cloud conversational API on Google Colab to serve the Alpha Mini Robot (SRA Lessons 5 and 6).

---

## Flask Microframework Overview

**Flask** is a lightweight, extensible Python Web Server Gateway Interface (WSGI) web application framework:
- Written in pure Python.
- Uses decorators (`@app.route()`) to map URL endpoints and HTTP verbs to Python handler functions.
- Default local binding: `http://localhost:5000` (or `127.0.0.1:5000`).
- To accept external inbound traffic, server must be bound to all interfaces: `host="0.0.0.0"`.

---

## Core Flask Routing Syntax

```python
from flask import Flask, request, jsonify

app = Flask(__name__)

# Basic GET Route
@app.route('/', methods=['GET'])
def index():
    return 'Alpha Mini Cloud Backend Active', 200

# POST Route for Natural Language Inference
@app.route('/chat', methods=['POST'])
def handle_chat():
    # Extract JSON payload from incoming HTTP request body
    data = request.json
    if not data or 'message' not in data:
        return jsonify({'error': 'Missing message field'}), 400
    
    user_msg = data['message']
    # Execute NLP inference (e.g. NeuralIntents)
    reply = assistant.request(user_msg)
    
    return jsonify({'response': reply}), 200

if __name__ == '__main__':
    # Local development run
    app.run(host='0.0.0.0', port=5000)
```

---

## Overcoming Cloud Isolation with Ngrok

### The Google Colab Connectivity Challenge
When running a Flask server inside Google Colab, the environment executes inside an isolated Google virtual container. Its local network port (5000) is behind Google's internal NAT firewall and is completely inaccessible to external machines or Alpha Mini robots operating over local Wi-Fi.

### The Ngrok Solution
**Ngrok** establishes a secure, persistent reverse tunnel from the Colab container to a public edge node on the Internet.
- When `flask-ngrok` is attached to a Flask app, it spawns an Ngrok agent process during startup.
- Allocates a public HTTPS URL (e.g., `https://xxxx-xx-xxx.ngrok-free.app`).
- External HTTP POST requests sent to this public URL are transparently tunneled directly into the Colab Flask application on port 5000.

---

## Complete Google Colab Server Implementation

```python
# 1. Install required cloud packages in Colab
!pip install flask flask-ngrok neuralintents nltk

# 2. Complete Server Script in Colab Notebook
import nltk
from flask import Flask, request, jsonify
from flask_ngrok import run_with_ngrok
from neuralintents import GenericAssistant

# Initialize and train NLP assistant
assistant = GenericAssistant('intents.json', model_name="alphamini_nlp")
assistant.train_model()

# Initialize Flask application
app = Flask(__name__)
run_with_ngrok(app)  # Binds Ngrok tunnel to Flask application

@app.route('/predict', methods=['POST'])
def predict_intent():
    req_data = request.get_json(force=True)
    user_text = req_data.get('query', '')
    
    # Process text through NeuralIntents CNN
    bot_reply = assistant.request(user_text)
    
    return jsonify({
        'status': 'success',
        'query': user_text,
        'response': bot_reply
    }), 200

if __name__ == '__main__':
    # Starts local server and outputs public ngrok.io URL to Colab console
    app.run()
```

---

## Inter-Node Connectivity
- RESTful Standards: [[RESTfulAPIArchitectureInRobotics]]
- Chatbot Training: [[NLPAndNeuralIntentsChatbot]]
- Robot Integration: [[AlphaMiniCloudChatbotIntegrationPipeline]]
- Lab 4 Protocol: [[Lab04-ColabChatbotImplementationProtocol]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 7, `SRA Lesson 5 - v1.0.pptx` & Slide 3, `SRA Lesson 6 - v1.0.pptx`:**
> *"Flask - A Python Web Framework: Quick and dirty way to get a Web App up and running. Written in pure Python. Use annotation to specify the route and verb above the handler definition.*
> *Default is localhost at port 5000. app.run() equivalent to app.run(host='localhost', port=5000).*
> *To allow external client to reach, need to specify a host='0.0.0.0' to bind it to the server IP.*
> *@app.route() also takes an additional parameter: methods for example @app.route('/about', methods=['GET'])*
> *Colab Chatbot is the back-end: NLP engine. Expose API to Alpha Mini using Flask-NGROK. Process text from Alpha Mini. Generate possible response to text. Relay response to Alpha Mini."*

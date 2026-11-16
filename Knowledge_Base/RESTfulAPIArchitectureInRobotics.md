# RESTful API Architecture in Robotics

In distributed service robotics, physical robots operate as edge clients connected to enterprise and cloud microservices. This document outlines the constraints of **RESTful APIs**, HTTP transport protocols, and communication mechanics as taught in SRA Lesson 5.

---

## What is a RESTful API?

**Representational State Transfer (REST)** is an architectural style defining a set of architectural constraints for creating interoperable web services. A REST API provides a lightweight, stateless mechanism for software systems to query and manipulate distributed resources without complex local processing.

---

## HTTP Verbs & Standard Status Codes

| HTTP Verb | CRUD Operation | Semantic Purpose in Service Architecture | Standard Success Code | Standard Failure Code |
| :--- | :--- | :--- | :---: | :---: |
| **GET** | Read | Retrieve a representation of a specified resource | `200 OK` | `404 Not Found` |
| **POST** | Create / Process | Submit new data or trigger computation (e.g. NLP query) | `201 Created` / `200 OK` | `400 Bad Request` |
| **PUT** | Replace / Update | Update or replace an existing target resource completely | `200 OK` / `204 No Content` | `404 Not Found` / `400 Bad Request` |
| **DELETE** | Delete | Remove the specified resource | `200 OK` / `204 No Content` | `404 Not Found` |
| **PATCH** | Partial Modify | Partially modify a resource (rarely used compared to PUT) | `200 OK` / `204 No Content` | `404 Not Found` / `400 Bad Request` |

---

## Anatomy of HTTP Requests & Responses

Every HTTP transaction consists of structured headers and bodies transmitted over TCP/IP:

```
[ HTTP Request ]
├── Transport: HTTP (Port 80) or HTTPS (Port 443 - SSL/TLS Encrypted)
├── URL / Endpoint: https://api.service.com/v1/chatbot
├── Query Parameters: ?id=00879&mode=fast (used in GET/DELETE to filter data)
├── Request Headers: Metadata (Content-Type: application/json, Authorization, User-Agent)
└── Request Body: Payload data formatted in JSON (used in POST/PUT/PATCH)

[ HTTP Response ]
├── Status Code: 200 OK / 400 Bad Request / 404 Not Found / 500 Internal Error
├── Response Headers: Metadata (Content-Type: application/json, Set-Cookie)
└── Response Body: Returned data payload formatted in JSON
```

---

## Specifying Request Scope: Query vs. Path Parameters

### 1. Query Parameters (GET, DELETE)
- Appended to URL string following a question mark (`?`):
  `https://api.robotics.org/query?tag=greeting&language=en`
- Used to filter, search, or refine retrieval without altering endpoint semantics.

### 2. Path / Request Parameters (POST, PUT, PATCH, GET)
- Embedded directly into URL path hierarchy:
  `https://api.robotics.org/robots/00879/posture`
- Identifies a specific individual resource instance.

---

## Data Interchange via Python `requests` Library

In the Alpha Mini ecosystem, Python uses the standard `requests` module to execute outbound HTTP calls to cloud endpoints:

```python
import requests

# Target cloud REST endpoint (e.g. exposed Colab Ngrok tunnel)
url = "https://a1b2-34-123-45.ngrok-free.app/chat"

# JSON Payload containing speech recognized from Alpha Mini
payload = {
    "robot_id": "00879",
    "message": "Hello, what can you do?"
}

# Execute HTTP POST request
headers = {"Content-Type": "application/json"}
response = requests.post(url, json=payload, headers=headers)

# Process Response
if response.status_code == 200:
    data = response.json()
    reply_text = data.get("response")
    print(f"Cloud Chatbot Response: {reply_text}")
else:
    print(f"HTTP Error {response.status_code}: {response.text}")
```

---

## Inter-Node Connectivity
- Chatbot Engine: [[NLPAndNeuralIntentsChatbot]]
- Flask & Ngrok Server: [[FlaskAndNgrokCloudIntegration]]
- End-to-End Pipeline: [[AlphaMiniCloudChatbotIntegrationPipeline]]
- Speech Observer: [[AlphaMiniSpeechRecognitionObserver]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 5 & 6, `SRA Lesson 5 - v1.0.pptx`:**
> *"RESTFul API: Representational State Transfer (REST) is an architectural style that defines a set of constraints to be used for creating web services.*
> *Verbs: GET (Read, 200, 404); POST (Create, 201, 400); PUT (Update, 200/204, 404/400); DELETE (Remove, 200, 404); PATCH (Modify, 200/204, 404/400).*
> *Request and Response: Supports HTTP and HTTPS transport protocols. Typically, all web browsers show a small lock icon for secure transaction with https (encrypted).*
> *The header is the place to store meta information about the request/response (Authentication, Session cookie).*
> *The body is the place to send/receive data (JSON formatted) for the request/response. JSON stands for JavaScript Object Notation."*

# Reference: Python 3 Cheat Sheet

This document provides an exhaustive technical reference of the **Python 3 Cheat Sheet** by Laurent Pointal, documenting core types, containers, control structures, and methods used across the [[Single-Board-Computer-Raspberry-Pi-Gateway|SBC gateway scripts]].

---

## 1. Base Data Types & Literals

| Type | Description | Syntax Examples |
| :--- | :--- | :--- |
| `int` | Arbitrary precision integer | `42`, `-10`, `0x2A` (hex), `0b101010` (binary) |
| `float` | IEEE 754 64-bit floating point | `3.14159`, `-1.2e-4`, `float("inf")` |
| `bool` | Boolean logical flag | `True`, `False` |
| `str` | Immutable Unicode character string | `"Hello"`, `'AIoT'`, `f"Value: {val}"` |
| `bytes` | Immutable sequence of 8-bit integers | `b"Hello\r\n"`, `"data".encode('utf-8')` |
| `NoneType`| Null object representing absence of value | `None` |

---

## 2. Sequence & Container Data Structures

### 1. Lists (`list` - Mutable, Ordered):
```python
my_list = [10, "sensor", 3.14]
my_list.append(42)          # Append item to end
val = my_list.pop(0)        # Remove and return item at index 0
my_list.sort()              # In-place sort
```

### 2. Dictionaries (`dict` - Key-Value Mapping):
```python
auth_dict = {
    'client_id': '1927000_client',
    'username': 'gate_user',
    'password': 'secret_password'
}
client_id = auth_dict.get('client_id', 'default_id')
```

### 3. Tuples (`tuple` - Immutable, Ordered):
```python
coordinates = (1.3521, 103.8198) # Latitude, Longitude
lat, lon = coordinates           # Tuple unpacking
```

### 4. Sets (`set` - Unordered, Unique Elements):
```python
devices = {"nano33_01", "nano33_02", "nano33_01"} # {"nano33_01", "nano33_02"}
```

---

## 3. Slicing & Indexing Operations

For sequence `s = [0, 10, 20, 30, 40, 50]`:
- `s[0]` $\rightarrow$ First element (`0`)
- `s[-1]` $\rightarrow$ Last element (`50`)
- `s[1:4]` $\rightarrow$ Slice from index 1 up to (excluding) index 4 (`[10, 20, 30]`)
- `s[::-1]` $\rightarrow$ Reverse sequence (`[50, 40, 30, 20, 10, 0]`)

---

## 4. Control Flow & Error Handling

### 1. Conditionals (`if`, `elif`, `else`):
```python
if cpu_usage > 90:
    alert_level = "CRITICAL"
elif cpu_usage > 70:
    alert_level = "WARNING"
else:
    alert_level = "NORMAL"
```

### 2. For Loop & Iteration:
```python
for item in ['accX', 'accY', 'accZ']:
    print(f"Sampling axis: {item}")
```

### 3. Exception Handling (`try...except...finally`):
```python
try:
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    data = ser.readline().decode('utf-8').strip()
except serial.SerialException as err:
    print(f"Serial Port Error: {err}")
finally:
    ser.close()
```

---

## 5. String Manipulation & Encoding

- `s.strip()`: Removes leading and trailing whitespace / newlines (`\r\n`).
- `s.split(',')`: Splits string on delimiter into a list of substrings.
- `s.encode('utf-8')`: Converts string to binary `bytes` for transmission over serial or network sockets.
- `b.decode('utf-8', errors='ignore')`: Converts binary bytes back to string.

---

## Traceability & Source Excerpts

> **Source:** `09_Additional Resources/Python_Cheat_Sheet_01 - Copy.pdf` (Pages 1-2)
> 
> "Python 3 Cheat Sheet by Laurent Pointal. Base types: int, float, bool, str, bytes... Containers: list, tuple, dict, set... Indexing and slicing: s[start:end:step]... Control flow: if/elif/else, for/in, while... Exception handling: try/except/finally."

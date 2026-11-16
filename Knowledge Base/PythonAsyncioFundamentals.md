# Python Asyncio Fundamentals

Asynchronous programming is the computational paradigm underpinning the **Alpha Mini Python SDK**. This document outlines the fundamental concepts of asynchronous programming, contrasting synchronous versus asynchronous execution, parallelism versus concurrency, and defining coroutine semantics in Python.

---

## Synchronous vs. Asynchronous Execution

### Synchronous Execution
- Code executes sequentially, step-by-step from top to bottom.
- When an operation blocks (e.g. waiting for network socket reply, file I/O, or sleeping via `time.sleep()`), the entire process execution thread freezes.
- Highly inefficient for robotics where physical actuators take seconds to move, vision pipelines experience inference latencies, and sensor polling occurs across Wi-Fi.

### Asynchronous Execution
- Task-based execution model where long-running I/O or delays do not block the execution thread.
- Tasks yield execution control voluntarily during waiting periods, allowing other pending tasks or sensor listeners to execute concurrently on the same thread.
- Enabled in Python via the `asyncio` standard library module (introduced in Python 3.4).

---

## Parallelism vs. Concurrency

| Dimension | Parallelism | Concurrency (Asyncio) |
| :--- | :--- | :--- |
| **Execution Model** | Simultaneous execution across physical cores | Interleaved cooperative execution on a single core/thread |
| **Hardware Requirement** | Multi-threaded / Multi-core processors | Operates on single-threaded / single-core runtimes |
| **Context Switching** | Managed preemptively by OS kernel | Managed cooperatively by the Python **Event Loop** |
| **Course Focus** | Not required for basic robotics | **Primary focus in SRA module**: Alpha Mini SDK uses single-threaded concurrency |

> **Principle from SRA Lesson 3:** *"Parallelism requires multi-threaded/multi-core processors. Concurrency for single thread/single-core processors (during IDLE time). Consider Concurrency only in this course. Usually asyncio runs in Single thread. Concurrency is provided NOT by threads, but by using single event loop for running different coroutines(tasks)."*

---

## The Four Pillars of Asyncio Importance

1. **Improved Performance:** Eliminates idle CPU cycles during I/O-bound operations (e.g. network requests to robot over Wi-Fi, waiting for TTS completion, or reading infrared ToF range).
2. **Scalability:** Enables handling multiple concurrent streams and sensor observers without spawning resource-heavy OS threads.
3. **Responsiveness:** In robotics and GUI systems, prevents interface lock-ups and allows physical safety interrupts (e.g., tap head to abort motion) to fire instantly while motion executes.
4. **Simplified Code:** Replaces complex callback hell with readable, linear-looking `async` / `await` syntax.

---

## Coroutines & The `async` / `await` Syntax

A **coroutine** (short for *cooperative routine*) is a specialized Python function declared with the `async def` keyword:
- When a coroutine is called, it **does not run immediately**; instead, it returns a **coroutine object**.
- Coroutines must be scheduled onto an event loop or awaited to execute.
- Inside a coroutine, the `await` keyword yields execution back to the scheduler until the awaited awaitable completes.
- **Rule of Non-blocking:** Coroutines **must never run blocking calls** like `time.sleep()`. Doing so freezes the event loop. Instead, use non-blocking equivalents like `await asyncio.sleep()`.

### Code Example: Syntax Comparison

```python
# Synchronous (Blocking)
import time

def sync_work():
    print("Step 1")
    time.sleep(2)  # FREEZES ENTIRE THREAD FOR 2 SECONDS
    print("Step 2")

# Asynchronous (Non-blocking Cooperative)
import asyncio

async def async_work():
    print("Task started")
    await asyncio.sleep(2)  # YIELDS THREAD TO OTHER TASKS FOR 2 SECONDS
    print("Task finished")

asyncio.run(async_work())
```

---

## Inter-Node Connectivity
- Event Loop Architecture: [[AsyncioEventLoopAndScheduling]]
- Tasks & Awaitables: [[AsyncioTaskManagementAndPrimitives]]
- Lab 2 Protocol: [[Lab02-PythonAsyncioProgrammingProtocol]]
- SDK Integration: [[AlphaMiniPythonSDKArchitecture]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 3 & 4, `SRA Lesson 3 - v1.0.pptx` & Labsheet 02:**
> *"Asynchronous Programming: Synchronous: Steps by steps. Execution from top to bottom in order.*
> *Asynchronous: Tasks based. Execution in parallel or concurrently, out-of-order.*
> *Parallelism requires multi-threaded/multi-core processors. Concurrency for single thread/single-core processors (during IDLE time). Consider Concurrency only in this course.*
> *Usually asyncio runs in Single thread. Concurrency is provided NOT by threads, but by using single event loop for running different coroutines(tasks).*
> *Improved Performance: Asynchronous programming allows you to execute multiple tasks in parallel, rather than executing them sequentially. This leads to improved performance, particularly in I/O-bound or network-bound applications, as the program continue to execute other tasks while it's waiting for data.*
> *Coroutines: Short for co-operative routines. Asynchronous functions. Declared with async/await syntax. Must behave and co-operate. Cannot run blocking code eg. time.sleep(). Returns coroutine object when called. Must await coroutine to receive the return value from it."*

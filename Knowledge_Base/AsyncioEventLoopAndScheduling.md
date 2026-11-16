# Asyncio Event Loop and Scheduling

The **Event Loop** is the central execution engine and scheduler of the Python `asyncio` framework. This document details its mechanics, lifecycle management, loop acquisition methods, and the bridge to synchronous execution via thread pool executors.

---

## What is the Event Loop?

Conceptually, the event loop is a continuous scheduling loop:
```python
while True:
    callbacks = get_ready_callbacks_and_io_events()
    for callback in callbacks:
        run_callback(callback)
    if should_stop():
        break
```
- Manages and coordinates tasks, coroutines, and network socket I/O events.
- Tracks task states: `ready`, `running`, `suspended`, `cancelled`, and `done`.
- When an I/O operation (or `asyncio.sleep`) is encountered, the coroutine yields control. The loop runs other ready tasks, and resumes the paused coroutine once its I/O event resolves.

---

## Methods for Accessing & Starting the Event Loop

### 1. Modern Recommended Pattern: `asyncio.run()`
Introduced in Python 3.7; automatically creates a new event loop, sets it as the current loop, executes the passed root coroutine until completion, cancels any remaining pending tasks, and cleanly closes the loop:
```python
import asyncio

async def main():
    await asyncio.sleep(1)
    print("Main complete")

if __name__ == '__main__':
    asyncio.run(main())
```

### 2. Explicit Management: `asyncio.get_event_loop()` vs `asyncio.get_running_loop()`

| Function | Mechanics & Behavior | Usage Context |
| :--- | :--- | :--- |
| `asyncio.get_event_loop()` | Checks if a loop is set in current thread. If none exists (and thread is main), it automatically instantiates a new loop, sets it, and returns it. | Legacy pattern popular in Python 3.4–3.9; used extensively in Alpha Mini demo codes (`run_forever()`). Deprecated in Python 3.10+. |
| `asyncio.get_running_loop()` | Returns the active running event loop in the current OS thread. Raises a `RuntimeError` if no loop is actively running. | Preferred modern method when called inside an active coroutine or task to schedule callbacks or access executors. |

---

## `await` vs. `run_until_complete()`

| Construct | Execution Context | Blocking Scope | Return Mechanics |
| :--- | :--- | :--- | :--- |
| `await <awaitable>` | Used exclusively **inside an `async def` function** | Suspends only the enclosing coroutine; **does not block the thread or event loop** | Returns the value yielded by the awaitable once complete |
| `loop.run_until_complete(<awaitable>)` | Used in **synchronous outer code** | **Blocks the current OS thread** synchronously until the target awaitable terminates | Returns the final return value of the coroutine/future |

---

## Running Forever & Persistent Event Listeners: `loop.run_forever()`

In robotics applications with event observers (e.g. listening for touch gestures, speech recognition, or obstacle proximity), the program must remain alive indefinitely until an exit condition occurs:
```python
import asyncio

loop = asyncio.get_event_loop()
# Schedule initial setup
loop.run_until_complete(setup_robot())

# Keep loop running to handle incoming observer events
try:
    loop.run_forever()
finally:
    loop.run_until_complete(shutdown())
    loop.close()
```

---

## Multi-threading Bridge: `loop.run_in_executor()`

When a Python application must run blocking synchronous functions (e.g., standard `time.sleep()`, disk file I/O, heavy mathematical transforms, or calling `loop.stop()` from within a synchronous handler), `run_in_executor()` runs the function inside an underlying thread pool:
```python
# Running a blocking synchronous function without freezing the event loop
loop = asyncio.get_running_loop()
await loop.run_in_executor(None, synchronous_blocking_function)

# Stopping the event loop from within an observer callback:
# (None instructs asyncio to use the default ThreadPoolExecutor)
asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)
```

---

## Inter-Node Connectivity
- Core Theory: [[PythonAsyncioFundamentals]]
- Tasks & Primitives: [[AsyncioTaskManagementAndPrimitives]]
- Practical Lab: [[Lab02-PythonAsyncioProgrammingProtocol]]
- Reactive Observers: [[AlphaMiniObserverPatternArchitecture]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 8, 9, 10, `SRA Lesson 3 - v1.0.pptx` & Labsheet 02:**
> *"Asyncio - Event Loop: A scheduler. Just a BIG while loop. While True: Collect the ready callbacks; Run them.*
> *Comparison: get_event_loop() vs get_running_loop():*
> *get_running_loop() returns the running event loop in the current OS thread. Generate an Exception if None is found. Preferred way is to use get_running_loop() with asyncio.run().*
> *get_event_loop() if no current event loop set in current OS thread... creates a new event loop and returns it. Deprecated since 3.10.*
> *await vs run_until_complete: await is used to block the execution of a coroutine until a task marked as awaitable has completed. run_until_complete() is used to run a task until it is complete, and it blocks the execution of the current thread until the task is finished.*
> *Another event loop method you will encounter is the run_in_executor() method... allows you to run a synchronous function in a separate thread pool. The None parameter means to use the default thread pool for this operation."*

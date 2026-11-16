# Lab 02: Python Asyncio Programming Protocol

This document provides the exhaustive procedural lab manual and experimental walkthrough for **Labsheet 02: Introduction to Python Asynchronous Programming**.

---

## Lab Objectives

In this lab, students will learn:
1. What asynchronous programming is and why it is critical for robotics.
2. The architectural structure of an asynchronous coroutine block (`async def`, `await`).
3. How to create and schedule tasks on the event loop using `asyncio.create_task()`.
4. How to coordinate multiple concurrent tasks using `asyncio.wait()`, `asyncio.gather()`, and `asyncio.as_completed()`.
5. How to manage the event loop explicitly and execute synchronous code using `run_in_executor()`.

---

## Experimental Code Progression

### Experiment 1: Synchronous Baseline (`synch.py`)
```python
import time

def other_function():
    print("other_function: 1")
    time.sleep(2)
    print("other_function: 2")

def main():
    print("main: 1")
    other_function()
    print("main: 2")

if __name__ == '__main__':
    main()
```
- **Console Output:**
  ```
  main: 1
  other_function: 1
  other_function: 2
  main: 2
  ```
- **Observation:** Strict sequential execution. `main: 2` is blocked for 2 seconds while `time.sleep(2)` runs.

---

### Experiment 2: The Premature Exit Bug (`async.py`)
```python
import asyncio

async def other_function():
    print("other_function: 1")
    await asyncio.sleep(2)
    print("other_function: 2")

async def main():
    print("main: 1")
    task = asyncio.create_task(other_function())
    print("main: 2")

if __name__ == '__main__':
    asyncio.run(main())
```
- **Console Output:**
  ```
  main: 1
  main: 2
  other_function: 1
  ```
- **Analysis:** Notice that `"other_function: 2"` is **missing from console output**!
- **Root Cause:** `main()` finishes executing immediately after printing `"main: 2"`. Because `task` was not awaited, the program exited before the 2-second sleep finished, terminating the event loop prematurely.

---

### Experiment 3: Fixing with `await` (`async2.py`)
```python
import asyncio

async def other_function():
    print("other_function: 1")
    await asyncio.sleep(2)
    print("other_function: 2")

async def main():
    print("main: 1")
    task = asyncio.create_task(other_function())
    print("main: 2")
    await task  # Blocks main() until task finishes

if __name__ == '__main__':
    asyncio.run(main())
```
- **Console Output:**
  ```
  main: 1
  main: 2
  other_function: 1
  other_function: 2
  ```
- **Observation:** `main: 2` prints *before* `other_function: 1` completes because `create_task()` scheduled it concurrently. `await task` ensures the program waits for completion.

---

### Experiment 4: Awaiting Coroutine Directly (`async3.py`)
```python
import asyncio

async def other_function():
    print("other_function: 1")
    await asyncio.sleep(2)
    print("other_function: 2")

async def main():
    print("main: 1")
    await other_function()  # Direct await without create_task
    print("main: 2")

if __name__ == '__main__':
    asyncio.run(main())
```
- **Difference:** Awaiting the coroutine directly behaves sequentially like synchronous code: `main: 2` does not execute until `other_function()` completely finishes.

---

### Experiment 5: Retrieving Return Values (`async4.py`)
```python
import asyncio

async def other_function():
    print("Computing in background...")
    await asyncio.sleep(2)
    return "Data from Robot Sensor"

async def main():
    task = asyncio.create_task(other_function())
    result = await task
    print(f"Received result: {result}")

asyncio.run(main())
```

---

### Experiment 6: Concurrent Multi-Tasking via `asyncio.wait()` (`async5.py`)
```python
import asyncio

async def function1():
    await asyncio.sleep(1)
    return "Result 1"

async def function2():
    await asyncio.sleep(2)
    return "Result 2"

async def function3():
    await asyncio.sleep(3)
    return "Result 3"

async def main():
    task1 = asyncio.create_task(function1())
    task2 = asyncio.create_task(function2())
    task3 = asyncio.create_task(function3())

    # Tuple destructuring into done and pending sets
    done, pending = await asyncio.wait([task1, task2, task3])

    # Extract results using list comprehension
    results = [x.result() for x in done]
    print(f"All done: {results}")

asyncio.run(main())
```

---

### Experiment 7: Spreading Unpacking with `asyncio.gather()` (`async6.py`)
```python
import asyncio

async def main():
    coros = [function1(), function2(), function3()]
    # Argument spreading syntax: *coros
    results = await asyncio.gather(*coros)
    print(f"Gathered ordered results: {results}")

asyncio.run(main())
```

---

### Experiment 8: Progressive Processing via `asyncio.as_completed()` (`async7.py`)
```python
import asyncio

async def main():
    coros = [function1(), function2(), function3()]
    # Iterates in order of completion (fastest task returns first)
    for finished_task in asyncio.as_completed(coros):
        res = await finished_task
        print(f"Task finished: {res}")

asyncio.run(main())
```

---

### Experiment 9: Multi-Threading Bridge via `run_in_executor()`
```python
import asyncio
import time

def blocking_sync_function():
    print("Synchronous blocking task running in thread pool...")
    time.sleep(3)
    return "Done blocking"

async def main():
    loop = asyncio.get_running_loop()
    # Runs blocking synchronous function without halting asyncio event loop
    result = await loop.run_in_executor(None, blocking_sync_function)
    print(f"Executor returned: {result}")

asyncio.run(main())
```

---

## Inter-Node Connectivity
- Asyncio Fundamentals: [[PythonAsyncioFundamentals]]
- Event Loop Architecture: [[AsyncioEventLoopAndScheduling]]
- Concurrency Primitives: [[AsyncioTaskManagementAndPrimitives]]
- Lab 3 Robot Challenge: [[Lab03-AlphaMiniBasicsAndChallengeProtocol]]

---

## Source Excerpt

> **Verbatim Excerpt from Labsheet 02 (`Labsheet 02 - Intro to Python Asyncio - Copy.docx`):**
> *"In line 11, the other_function() is supposed to print out a 'other_function: 2'. However, this is missing from the console output. Why?*
> *The reason for the missing print out is because the program has terminated normally, and the asynchronous task task has no time to complete.*
> *After the line 6, insert a line that reads await task.*
> *In the code for async5.py: done, pending = await asyncio.wait([task1, task2, task3]) - This is called destructing the result (tuple) into its component.*
> *In line 26: print([x.result() for x in done])*
> *In async6.py: tasks = asyncio.gather(*coros) - This is called spreading.*
> *asyncio.as_completed() returns an iterable that we can use to retrieve the 'awaitables' in the order that they are completed.*
> *The run_in_executor() method allows you to run a synchronous function in a separate thread pool. The None parameter means to use the default thread pool."*

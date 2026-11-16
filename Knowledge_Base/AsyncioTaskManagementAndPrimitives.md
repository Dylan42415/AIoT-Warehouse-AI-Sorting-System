# Asyncio Task Management and Primitives

This document covers the core concurrency primitives in Python `asyncio`: **Awaitables**, **Coroutines**, **Tasks**, **Futures**, and collective scheduling methods (`asyncio.wait`, `asyncio.gather`, `asyncio.as_completed`).

---

## The Awaitables Hierarchy

An **Awaitable** is any Python object that can be passed to an `await` expression. There are three primary types of awaitables:

```
                      +-------------------+
                      |     Awaitable     |
                      +---------+---------+
                                |
         +----------------------+----------------------+
         |                      |                      |
         v                      v                      v
+-----------------+    +-----------------+    +-----------------+
|   Coroutines    |    |      Tasks      |    |     Futures     |
| Functions with  |    | Wraps coroutine |    | Low-level event |
| async def       |    | in Event Loop   |    | placeholder for |
| Pauses & yields |    | Task scheduling |    | future result   |
+-----------------+    +-----------------+    +-----------------+
```

1. **Coroutines:** Return coroutine objects when invoked. Must be awaited directly or wrapped in a Task.
2. **Tasks (`asyncio.Task`):** Higher-level constructs created via `asyncio.create_task()`. Wrap coroutines, schedule them immediately onto the event loop, and provide cancellation, state querying, and exception handling.
3. **Futures (`asyncio.Future`):** Low-level objects representing an eventual result of an asynchronous operation that may not be available yet. Provide callback chaining (`add_done_callback()`) and explicit resolution via `fut.set_result(val)`.

---

## Creating & Scheduling Tasks: `asyncio.create_task()`

Calling a coroutine directly (`my_coro()`) only constructs a coroutine object. Wrapping it in `create_task()` registers it into the event loop scheduler:
```python
import asyncio

async def worker(id, delay):
    print(f"Worker {id} started")
    await asyncio.sleep(delay)
    print(f"Worker {id} finished")
    return id * 10

async def main():
    # Schedules task into event loop immediately (non-blocking)
    task1 = asyncio.create_task(worker(1, 2))
    task2 = asyncio.create_task(worker(2, 1))

    # Await results when needed
    res1 = await task1
    res2 = await task2
    print(f"Results: {res1}, {res2}")

asyncio.run(main())
```

---

## Collective Concurrency Patterns

When running multiple asynchronous tasks concurrently, three distinct coordination methods are used:

### 1. `asyncio.wait()` with Tuple Destructuring
Waits for an iterable of tasks to complete. Returns a tuple of two sets: `(done, pending)`:
```python
tasks = [asyncio.create_task(worker(i, i)) for i in range(1, 4)]

# Tuple destructuring
done, pending = await asyncio.wait(tasks)

# Extract results using list comprehension
results = [t.result() for t in done]
print(f"Done results: {results}")
```

### 2. `asyncio.gather()` with Argument Spreading (`*coros`)
Takes awaitables unpacked via spreading (`*coros`), runs them concurrently, and returns an ordered list containing all return values in the exact order the awaitables were supplied:
```python
coros = [worker(1, 2), worker(2, 1), worker(3, 3)]

# Spreading unpacking operator (*)
results = await asyncio.gather(*coros)
print(f"Gathered ordered results: {results}")
```

### 3. `asyncio.as_completed()` for Progressive Iteration
Returns an iterator that yields tasks in the **chronological order in which they finish**, regardless of launch order:
```python
coros = [worker(1, 3), worker(2, 1), worker(3, 2)]

# Iterates as tasks finish (Worker 2 will yield first, then Worker 3, then Worker 1)
for finished_task in asyncio.as_completed(coros):
    result = await finished_task
    print(f"Progressive result received: {result}")
```

---

## Futures & Callbacks (`test_future.py`)

Low-level Futures allow registering synchronous callbacks that fire the instant a calculation or I/O event sets the result:
```python
import asyncio
import functools

def callback(future, n):
    print(f"{n}: future done: {future.result()}")

async def main():
    fut = asyncio.Future()
    fut.add_done_callback(functools.partial(callback, n=1))
    fut.add_done_callback(functools.partial(callback, n=2))
    
    # Simulate setting result
    fut.set_result("Operation Success")

asyncio.run(main())
```

---

## Inter-Node Connectivity
- Fundamentals: [[PythonAsyncioFundamentals]]
- Event Loop: [[AsyncioEventLoopAndScheduling]]
- Lab 2 Protocol: [[Lab02-PythonAsyncioProgrammingProtocol]]
- Simultaneous Robot Actions: [[Lab03-AlphaMiniBasicsAndChallengeProtocol]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 6, `SRA Lesson 3 - v1.0.pptx` & Labsheet 02:**
> *"An 'awaitable' is any object that can be used with the await keyword inside an asynchronous function. This includes coroutines, tasks, and futures.*
> *Coroutines are functions that can be paused and resumed during execution, allowing other code to run in the meantime.*
> *Tasks are a higher-level construct that wrap coroutines and provide additional features like cancellation and result propagation.*
> *Futures are objects that represent the result of a computation that may not be available yet.*
> *done, pending = await asyncio.wait([task1, task2, task3]) - This is called destructing the result (tuple) into its component.*
> *tasks = asyncio.gather(*coros) - This is called spreading. It is equivalent to asyncio.gather(function1(), function2(), function3()).*
> *asyncio.as_completed() returns an iterable that we can use to retrieve the 'awaitables' in the order that they are completed."*

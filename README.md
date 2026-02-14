<!--
DuinoCollections
Copyright (c) 2026 Pierre Debas
SPDX-License-Identifier: MIT
-->

# DuinoCollections

<!-- [![Arduino Library](https://www.ardu-badge.com/badge/DuinoCollections.svg)](https://www.ardu-badge.com/DuinoCollections)
 [![PlatformIO Registry](https://badges.registry.platformio.org/packages/pierrolefou881/library/DuinoCollections.svg)](https://registry.platformio.org/libraries/pierrolefou881/DuinoCollections)  -->
 ![Header Only](https://img.shields.io/badge/header--only-yes-brightgreen) ![License](https://img.shields.io/github/license/Pierrolefou881/DuinoMemory) ![C++](https://img.shields.io/badge/C%2B%2B-11%2F14-blue) ![Embedded Friendly](https://img.shields.io/badge/embedded-friendly-blue)

Tired of managing raw arrays on Arduino?

**DuinoCollections** provides lightweight STL-like containers designed for 
embedded systems — fixed-capacity, single-allocation, and predictable memory 
usage.

Compatible with Arduino and PlatformIO on major targets (AVR, ESP32, RP2040...).

License: MIT

## Quick example
```cpp
#include <DuinoCollections.hpp>
using namespace DuinoCollections;

FixedVector<int> values(5);

values.push(10);
values.push(20);
values.push(30);

for (int v : values)
{
    Serial.println(v);
}
```

## Why **DuinoCollections**?
Arduino gives you arrays. **DuinoCollections** gives you containers.

### With raw arrays
Manual size tracking, index errors, boilerplate shifting... and code that gets 
messy fast.
```cpp
int values[10];
size_t size = 0;

values[size++] = 42;

// remove index 2, manual left-shifting
for (size_t i = 2; i < size - 1; i++) {
    values[i] = values[i + 1];
}
size--;

// iteration, the old-fashioned way.
for (size_t i = 0; i < size; i++) {
    Serial.println(values[i]);
}
```

### With DuinoCollections
Same job, but with:
- Fixed capacity (no resizing).
- Safer and clearer API.
- STL-like usage familiar to C++ developers.
- Cleaner, more concise, and maintainable code.

```cpp
FixedVector<int> values{ 10 };

// Automatically determines the insertion index and returns a status.
if (!values.push(42))
{
    Serial.println("Collection is full.");
}

// Here removed is an out parameter.
int removed{ };
if (values.remove_at(2, removed))
{
    Serial.println("Removal successful.");
}

// range for iterations
for (auto v : values) {
    Serial.println(v);
}

```
Quality-of-life features:
- Fixed-capacity containers (Vector, Set, Map, RingBuffer).
- Feedback on critical methods (`push()`, `insert_at()`, `pop()`, 
`remove_at()`...).
- Ordered variants with automatic sorting.
- Range-for iterators.
- Stack and queue-style operations.
- Efficient search and insertion policies.
- Single allocation at construction — no resizing, no heap fragmentation over 
time.

Simple containers. Predictable memory. Built for Arduino.

Feel free to take a look at the [cheat-sheet](CHEATSHEET.md) for quick use cases 
description.

## Why not use STL containers?
Standard C++ containers (`std::vector`, `std::map`, etc.) are often not a good 
fit for small microcontrollers:

- **Limited support**  
  STL support varies between Arduino cores and may be incomplete or heavy for 
  small targets.


- **Dynamic memory usage**  
  STL containers grow dynamically and reallocate memory, which can lead to **heap 
  fragmentation** and runtime failures on small devices.

- **Unpredictable RAM consumption**  
  Capacity changes at runtime, making worst-case memory usage hard to estimate.

- **Larger code footprint**  
  STL implementations may significantly increase binary size.

## When should I use DuinoCollections?
Use **DuinoCollections** when you would normally use a raw array, but need 
something safer and easier to manage.

It’s a good fit when:
- The maximum capacity is known in advance.
- You want to avoid manual size tracking and shifting.
- You need predictable memory usage.
- You want clearer, more maintainable code.

Typical use cases:
- Sensor data buffers.
- Communication or event queues.
- Fixed-size history or rolling windows.
- Lookup tables or small in-memory datasets.

If your data structure must grow dynamically at runtime, **DuinoCollections** is 
not the right tool — and on most Arduino targets, dynamic resizing is usually a 
bad idea anyway.

## Design goals
**DuinoCollections** is built with embedded constraints in mind:
- Deterministic memory: single allocation at construction. No resizing, no 
fragmentation over time.
- Fixed capacity by design: define the maximum size once, behavior stays 
predictable.
- No hidden costs: no virtual tables, no exceptions, no unnecessary overhead.
- Fail-safe operations: critical operations return a status instead of crashing 
or silently failing.
- STL-inspired, Arduino-friendly: familiar C++ patterns adapted to constrained 
environments.
- Minimal runtime footprint: efficient on small targets (AVR, ESP32, RP2040…).

Predictable behavior first. Convenience second.

## Non-goals
**DuinoCollections** is not meant to replace the full C++ STL.
- No dynamic resizing: Containers never grow automatically.
- No general-purpose desktop usage: This library is optimized for embedded 
systems, not for PCs or large-memory environments.
- No advanced STL features: No allocators, no exceptions, no complex iterator 
categories.
- No hidden heap activity: Memory is allocated once at construction — never 
during normal operation.
- Not for highly dynamic workloads: If your application constantly changes 
container sizes, a dynamically resizing structure may be more appropriate.

**DuinoCollections** is designed to bring pragmatic convenience to embedded 
development — not STL completeness.

## Installation

### Arduino IDE
Install directly from the IDE's LibraryManager.

OR

Clone the following [git repo](https://github.com/Pierrolefou881/DuinoCollections.git) 
directly into your Arduino/libraries directory. 

### PlatformIO
Add the library to your project's **platformio.ini** file:

#### Stable
```ini
lib_deps =
    https://github.com/Pierrolefou881/DuinoCollections.git#v1.0.0
```

#### Development
```ini
lib_deps =
    https://github.com/Pierrolefou881/DuinoCollections.git#main
```

## Structure
**DuinoCollections** is a small framework of generic, fixed-capacity containers.

Because most supported MCUs have limited memory, containers avoid unbounded 
dynamic allocation and heap fragmentation. All collections are array-backed and 
their capacity is defined at construction.

Memory is allocated once at construction and released on destruction.
This approach avoids the safety issues of external buffers and guarantees 
predictable lifetime and memory usage.

Containers are implemented using a CRTP-style design.
Runtime polymorphism is not supported — collections must be used through their 
concrete type.

## Available containers

### Core containers
- `FixedVector` — variable size with fixed capacity (general-purpose container).
- `FixedRingBuffer` — circular FIFO buffer for streaming data (optional overwrite 
mode).

- `FixedMap` — fixed-capacity key/value container, sorted by key.

### Utility containers
- `FixedSet` — unique elements only.
- `FixedOrderedVector` — automatically sorted vector.
- `FixedOrderedSet` — automatically sorted set.

All containers share a consistent API, deterministic memory usage, and perform a 
single allocation at construction only.

## Basic Usage
DuinoCollections provides fixed-capacity containers designed for predictable 
memory usage and deterministic behavior.
All containers:

* Allocate memory once at construction
* Never grow dynamically
* Return **bool status** for critical operations (always check it)
* Support **compile-time polymorphism only** (no virtual functions, no base-class 
pointers)

> **Important**
> Runtime polymorphism is **not supported**. Always use the concrete container 
> type.
>
> Many operations may fail (full container, duplicate rejection, not found, etc.).
> **Always check the returned bool**.

## Common base interface
All linear containers (`FixedVector`, `FixedSet`, `FixedOrderedVector`, 
`FixedOrderedSet` and `FixedMap`) share a common read-only interface:

* `size()`
* `capacity()`
* `is_empty()`
* `is_full()`
* `remove_at(index, item)`
* `clear()`
* `contains(item)`
* `find(item)`
* `operator[](index)` (const)
* Range-for iteration

Example:

```cpp
FixedSet<int> set(10);

set.insert(3);
set.insert(7);

if (set.contains(3))
{
    size_t index = set.find(3);
}

for (const auto& v : set)
{
    Serial.println(v);
}
```

## FixedVector
Unordered container that allows duplicates.
Can be used as a dynamic array, stack or queue.

### Public interface

* `push(item)`
* `pop(out_value)`
* `push_atomic(item)`
* `pop_atomic(out_value)`
* `insert_at(item, index)`
* `remove_first(item)`
* `remove_all(item)`
* `front()`
* `back()`
* Mutable `operator[]`
* Mutable iterators

### Example

```cpp
FixedVector<int> vec(5);

// Stack behavior
vec.push(10);
vec.push(20);

int value;
if (vec.pop(value))
{
    // value contains the removed element
}

if (vec.remove_at(0, value))
{
    // Queue behavior.
}

// Modify in place
vec[0] = 42;

for (auto& v : vec)
{
    v += 1;
}
```

### Atomic operations (ISR safety)
FixedVector provides:

* `push_atomic()`
* `pop_atomic()`

These operations temporarily disable interrupts and are intended for safe access 
between `loop()` and an ISR.
 
> Atomic operations are available on:
> - `FixedVector`
> - `FixedRingBuffer`

> These operations temporarily disable interrupts and must **not** be called from 
inside an ISR.

## FixedRingBuffer

Circular FIFO container designed for streaming data and real-time systems.

Typical uses:
- Sensor sampling
- Serial or communication buffers
- Event queues
- Rolling history

### Push behavior when full

The behavior is selected at compile time:

- **Reject (default)** — `push()` fails and returns `false`
- **Overwrite** — oldest data is discarded and the new item is stored

```cpp
// Default (safe): reject when full
FixedRingBuffer<int> buffer(16);

// Overwrite mode: keep most recent data
FixedRingBuffer<int, RingBufferMode::OVERWRITE> history(16);
```

### Public interface
- `push(item)`
- `pop(out_value)`
- `push_atomic(item)`
- `pop_atomic(out_value)`
- `front()`
- `back()`
- `clear()`
- `operator[]`
- Range-for iteration (logical order)

### Example
```cpp
FixedRingBuffer<int> buffer(4);

buffer.push(10);
buffer.push(20);
buffer.push(30);

int value;
if (buffer.pop(value))
{
    // value == 10 (FIFO)
}

for (int v : buffer)
{
    Serial.println(v);
}
```

### Atomic operations (ISR safety)
- `push_atomic()`
- `pop_atomic()`

> These methods temporarily disable interrupts and allow safe access between
loop() and an ISR.

> Do not call atomic methods from inside an ISR.

## FixedSet
Unordered collection with **unique elements**.
Elements can be accessed but **not modified**.

### Public interface

* `insert(item)`
* `insert_at(item, index)`
* `erase(item)`

### Example

```cpp
FixedSet<int> set(5);

set.insert(10);
if (set.insert(10))
{   
    // false (duplicate)
}

if (!set.erase(10))
{
    Serial.println("Element not found our collection is empty");
}
```

## FixedOrderedVector
Sorted container that allows duplicates.
Insertion keeps elements ordered automatically.

### Public interface

* `insert(item)`
* `remove_first(item)`
* `remove_all(item)`
* `front()`
* `back()`

### Example

```cpp
FixedOrderedVector<int> vec(5);

vec.insert(30);
vec.insert(10);
vec.insert(20);

// Order is maintained: 10, 20, 30
```

Custom order:

```cpp
FixedOrderedVector<int, Descending<int>> vec(5);
```

## FixedOrderedSet
Sorted container with **unique elements**.

### Public interface

* `insert(item)`
* `erase(item)`
* `front()`
* `back()`

### Example

```cpp
FixedOrderedSet<int> set(5);

set.insert(20);
set.insert(10);

if(!set.insert(20)) // false (duplicate)
{
    Serial.println("Duplicate was detected or collection is full");
}

// Order: 10, 20
```

## FixedMap
Associates a **unique key** to a value.
Keys are ordered internally for fast lookup.

### Public interface

* `add(key, value)`
* `remove(key, out_value)`
* `try_get(key, out_value)`

### Example

```cpp
FixedMap<uint8_t, int> map(5);

map.add(1, 100);
map.add(2, 200);

int value;
if (map.try_get(2, value))
{
    // value == 200
}

map.remove(1, value);
```

## Error handling pattern (recommended)

```cpp
if (!vec.push(value))
{
    // Handle error: container full or invalid
}
```

Always check return values to keep behavior deterministic.

## Memory footprint
Memory usage is deterministic and depends on:

- Element size
- Container capacity
- Internal metadata
- Alignment padding (platform dependent)

### LinearCollection overhead
Each container stores:

- Pointer to data
- Current size
- Capacity

| Architecture | Metadata size |
|---|---|
| AVR (8-bit) | 6 bytes |
| 32-bit targets (ESP32, RP2040, ARM) | 12 bytes |

### Examples (AVR)
| Container | Example | Total RAM |
|---|---|---|
| FixedVector<int> | capacity = 10 | ~26 bytes |
| FixedVector<float> | capacity = 10 | ~46 bytes |
| FixedRingBuffer<byte> | capacity = 64 | ~70 bytes |
| FixedSet<uint16_t> | capacity = 16 | ~38 bytes |
| FixedMap<uint8_t, uint16_t> | capacity = 8 | ~38 bytes |

Notes:
- AVR sizes: `int = 2`, `float = 4`
- `FixedMap<K,V>` stores `KeyValue<K,V>` which may include alignment padding
- Total RAM = metadata + (capacity × sizeof(T))
- Exact size depends on architecture and alignment rules

> If you know the capacity and element size, you know the exact RAM usage at 
compile time.

## Limitations
**DuinoCollections** is designed for small microcontrollers and predictable
memory usage. This design implies a number of intentional limitations:

- **Fixed capacity**  
  Container size is limited at construction time and cannot grow dynamically.
  Attempting to exceed the capacity has no effect and the operation returns a 
  failure status.

- **No unbounded dynamic allocation**  
  Memory is allocated once at construction and never resized afterward.

- **No runtime polymorphism**  
  Collections use a CRTP-like pattern. They must be used through their concrete 
  type and cannot be manipulated via a common base class.

- **Limited STL compatibility**  
  The API is inspired by the STL but intentionally reduced to keep the 
  implementation lightweight and predictable.

- **Performance optimized for small sizes**  
  Some containers use linear algorithms. They are efficient for small capacities 
  but are not intended for large datasets.

- **No thread safety**  
  Containers are not safe for concurrent access without external synchronization.

- **Overwrite mode (RingBuffer)**  
  may discard old data by design.


**DuinoCollections** is designed to bring pragmatic convenience to embedded
development, not to be a full STL replacement.

## License
MIT - see [LICENSE](LICENSE).
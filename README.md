# DuinoCollections

Still fighting raw arrays on Arduino?

**DuinoCollections** provides lightweight STL-like containers designed for 
embedded systems — fixed-capacity, single-allocation, and predictable memory 
usage.

Compatible with Arduino and PlatformIO on major targets (AVR, ESP32, RP2040...).

License: MIT

## Quick example
```C++
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

## Why DuinoCollections?

Arduino gives you arrays. DuinoCollections gives you containers.

### With raw arrays
Manual size tracking, index errors, boilerplate shifting... and code that gets 
messy fast.
```C++
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

```C++
FixedVector<int> values{ 10 };

// Automatically determines the insertion index and returns a status.
if (!values.push(42))
{
    Serial.println("Collection is full.");
}

if (values.remove_at(2))
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

Feel free to take a look at the [cheat-sheet](CHEATSHEET.md).

## When should I use DuinoCollections?

Use DuinoCollections when you would normally use a raw array, but need something 
safer and easier to manage.

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

If your data structure must grow dynamically at runtime, DuinoCollections is not 
the right tool — and on most Arduino targets, dynamic resizing is usually a bad 
idea anyway.


## Design goals
DuinoCollections is built with embedded constraints in mind:
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

DuinoCollections is not meant to replace the full C++ STL.
- No dynamic resizing: Containers never grow automatically.
- No general-purpose desktop usage: This library is optimized for embedded 
systems, not for PCs or large-memory environments.
- No advanced STL features: No allocators, no exceptions, no complex iterator 
categories.
- No hidden heap activity: Memory is allocated once at construction — never 
during normal operation.
- Not for highly dynamic workloads: If your application constantly changes 
container sizes, a dynamically resizing structure may be more appropriate.

DuinoCollections is designed to bring pragmatic convenience to embedded 
development — not STL completeness.

## Installation

### Arduino IDE

Clone the following [git repo](https://github.com/Pierrolefou881/DuinoCollections.git) 
directly into your Arduino/libraries directory. 

### PlatformIO

Add the library to your project's **platformio.ini** file:

<!-- #### Stable
```ini
lib_deps =
    https://github.com/Pierrolefou881/DuinoCollections.git#v1.0.0
``` -->

#### Development
```ini
lib_deps =
    https://github.com/Pierrolefou881/DuinoCollections.git#main
```

## Structure

DuinoCollections is a small framework of generic, fixed-capacity containers.

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
- **FixedVector** — variable size with fixed capacity (general-purpose container).
- **FixedRingBuffer** — circular FIFO buffer for streaming data.
- **FixedMap** — fixed-capacity key/value container, sorted by key.

### Utility containers
- **FixedSet** — unique elements only, sorted.
- **FixedOrderedVector** — automatically sorted vector.


All containers share a consistent API, deterministic memory usage, and perform a 
single allocation at construction only.


## Basic usage

### Importing collections
```C++
// example.ino

#include <DuinoCollections.hpp>      // Import the entire framework (not recommended).

// Or import each collection separately according to your needs.
#include <FixedVector.hpp>
```

## Memory footprint

Memory usage is deterministic and depends only on:
- Element size.
- Container capacity.
- Small internal metadata (indexes, size counters).

Typical memory usage examples:

| Container | Example | Approx. memory (AVR) |
|---|---|---|
| `FixedVector<int>` | capacity = 10 | ~20 bytes |
| `FixedVector<float>` | capacity = 10 | ~40 bytes |
| `FixedSet<uint16_t>` | capacity = 16 | ~32 bytes |
| `FixedRingBuffer<byte>` | capacity = 64 | ~64 bytes |
| `FixedMap<uint8_t, uint16_t>` | capacity = 8 | ~24 bytes |

Notes:
- AVR sizes: `int = 2 bytes`, `float = 4 bytes`.
- `FixedMap<K,V>` stores key and value together (`sizeof(K) + sizeof(V)`).
- A small control overhead is added (pointer, size, capacity), typically **6–8 
**bytes** on AVR.


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

DuinoCollections is designed to bring pragmatic convenience to embedded
development, not to be a full STL replacement.

## License

MIT - see [LICENSE](LICENSE)

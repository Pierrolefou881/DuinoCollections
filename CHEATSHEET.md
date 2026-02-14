<!--
DuinoCollections
Copyright (c) 2026 Pierre Debas
SPDX-License-Identifier: MIT
-->

# Cheat sheet

## TL;DR – Which container should I use?
All DuinoCollections containers are:

* Fixed capacity (no reallocation)
* Deterministic and Arduino-friendly
* O(1) memory usage after construction
* Returning **bool** on critical operations

## Quick decision table

| Need                                     | Use                  |
| ---------------------------------------- | -------------------- |
| Simple array / stack, duplicates allowed | `FixedVector`        |
| FIFO / streaming buffer                  | `FixedRingBuffer`    |
| Unique values, order does not matter     | `FixedSet`           |
| Always sorted, duplicates allowed        | `FixedOrderedVector` |
| Always sorted, unique values             | `FixedOrderedSet`    |
| Key → Value association                  | `FixedMap`           |

## FixedVector

**Use when:**

* You need a small dynamic array
* Order = insertion order
* Duplicates allowed
* Stack behavior needed (`push` / `pop`)
* You want mutable access

```cpp
FixedVector<int> vec(10);

vec.push(10);
vec.push(20);

vec[0] = 42;

int last;
vec.pop(last);
```

ISR-safe variant:

```cpp
vec.push_atomic(value);
```

## FixedRingBuffer

**Use when:**

* You need a FIFO buffer
* Data comes continuously (streaming)
* You want constant-time push/pop
* Perfect for producer/consumer patterns
* Ideal for ISR ↔ loop communication

```cpp
FixedRingBuffer<int> buffer(8);

buffer.push(10);
buffer.push(20);

int value;
buffer.pop(value);   // value == 10
```

ISR-safe usage:
```C++
buffer.push_atomic(sample);
buffer.pop_atomic(out);
```
### Typical use cases:
- Serial data buffering
- Sensor sampling
- Event queues
- ISR → main loop communication

## FixedSet

**Use when:**

* Values must be unique
* Order does not matter
* You just need presence checks

```cpp
FixedSet<uint8_t> activeIds(8);

activeIds.insert(3);
activeIds.insert(3);    // false (duplicate)

if (activeIds.contains(3))
{
    // already active
}
```

Typical use cases:

* Active flags
* Resource tracking
* Unique device IDs

## FixedOrderedVector

**Use when:**

* Data must stay sorted
* Duplicates allowed
* You frequently read min/max

```cpp
FixedOrderedVector<int> samples(16);

samples.insert(30);
samples.insert(10);
samples.insert(20);

// Always sorted: 10, 20, 30

int minVal = samples.front();
int maxVal = samples.back();
```

Typical use cases:

* Sensor value buffers
* Priority lists
* Statistics (min/max/median)

Custom order:

```cpp
FixedOrderedVector<int, Descending<int>> scores(10);
```

## FixedOrderedSet

**Use when:**

* Values must be **unique**
* And always **sorted**

```cpp
FixedOrderedSet<int> alarms(8);

alarms.insert(50);
alarms.insert(10);
alarms.insert(50);   // false

// Sorted: 10, 50
```

Typical use cases:

* Threshold lists
* Unique priorities
* Configuration tables

## FixedMap

**Use when:**

* You need a small dictionary
* Key must be unique
* Fast lookup by key

```cpp
FixedMap<uint8_t, int> sensors(8);

sensors.add(1, 100);
sensors.add(2, 200);

int value;
if (sensors.try_get(2, value))
{
    // value == 200
}

sensors.remove(1, value);
```

Typical use cases:

* ID → state
* Pin → configuration
* Address → value

## Error handling (important)
Operations may fail:

* Container full
* Duplicate rejected
* Item not found

Always check:

```cpp
if (!vec.push(value))
{
    // handle error
}
```

## When performance or safety matters
Choose ordered containers if:

* You need `front()` / `back()` quickly
* You want automatic sorting

Use atomic operations when sharing data with interrupts:

```cpp
vec.push_atomic(value);
vec.pop_atomic(out);
```

**Do not call atomic methods from inside an ISR.**

## Runtime polymorphism?
Not supported.

```cpp
// ❌ Don't do this
LinearCollection<int>* col;

// ✔️ Use concrete types
FixedVector<int> vec;
FixedSet<int> set;
```

DuinoCollections uses **compile-time polymorphism** to avoid virtual tables and 
reduce RAM usage.

## Rule of thumb

* Need mutation + duplicates → **Vector**
* Need uniqueness → **Set**
* Need sorting → **Ordered**
* Need key/value → **Map**
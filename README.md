# DuinoCollections

Lightweight collection framework for the Arduino environment.

Supports Arduino / PlatformIO.

License: MIT.

## Quick example
```C++

```

## Why DuinoCollections?

Feel free to take a look at the [cheat-sheet](CHEATSHEET.md).

## When should I use DuinoCollections?

## Design goals

## Non-goals

## Installation

### Arduino IDE

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

**DuinoCollections** is a small framework of generic collections. Due to
hardware limitations on most supported MCUs, these collections avoid
unbounded dynamic memory allocation and therefore limit heap fragmentation.
For that reason, all containers are array-backed and have a fixed capacity.

Memory allocation occurs once at construction time and is released on
destruction. This compromise was made to avoid safety issues inherent to
external buffers, especially due to their unknown lifetime.

Since the collections use a CRTP-like inheritance pattern, runtime polymorphic 
usage is not supported. Collections should therefore always be referred to 
by their concrete type.

## Basic usage

### Importing collections
```C++
// example.ino

#include <DuinoCollections.hpp>      // Import the entire framework (not recommended).

// Or import each collection separately according to your needs.
#include <FixedVector.hpp>
```

## Safety considerations and limitations

## License

MIT - see [LICENSE](LICENSE)

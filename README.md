# DuinoCollections

Lightweight collection framework for the Arduino environment.

Supports Arduino / PlatformIO.

License: MIT.

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

Polymorphic usage is supported for collections inheriting from the base
type **LinearCollection**. However, all critical methods remain non-virtual
to minimize RAM overhead. For these methods, inheritance is achieved through
the Curiously Recurring Template Pattern (CRTP).

## Examples

### Importing collections
```C++
// example.ino

#include <DuinoCollections.hpp>      // Import the entire framework (not recommended).

// Or import each collection separately according to your needs.
#include <FixedVector.hpp>
```

## License

MIT License

Copyright (c) 2026 Pierre DEBAS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

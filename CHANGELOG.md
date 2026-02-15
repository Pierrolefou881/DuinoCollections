<!--
DuinoCollections
Copyright (c) 2026 Pierre Debas
SPDX-License-Identifier: MIT
-->

# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.1] - 2026-02-15

## Changed
- `README.md`, `library.properties`, `library.json`: added Arduino Library
manager and PlatformIO registry badges.

## [1.0.0] - 2026-02-14

### Added
- `FixedRingBuffer` implementation.

### Changed
- `README.md` and `CHEATSHEET.md` to include `FixedRingBuffer`.
- `library.properties` and `library.json` to prepare the launch 
of `v1.0.0`.
- `ScopedInterruptLock.hpp`, `Iterator.hpp`: harmonized namespaces with
directory structure.

## [0.5.0] - 2026-02-13

### Added
- Added `CHANGELOG.md` to track project updates.
- `FixedVector` implementation and policies.
- `FixedSet` implementation and policies.
- `FixedOrderedVector` implementation and policies.
- `FixedOrderedSet` implementation and policies.
- `FixedMap` impelemntation and policies.
- `SortingOrder` for sorted collections.
- `Iterator` and `ConstIterator` for range for control loops.
- `ScopedInterruptLock` for providing ISR-safe `push` and `pop` for `FixedVector`.
- `TestVector.ino`, `TestOrderedSet.ino` and `TestMap.ino` for testing
and examples.
- `CHEATSHEET.md` for a quick-starting guide.
- `.gitignore`

### Changed
- Updated `README.md` to document `FixedVector`, `FixedSet`, `FixedOrderedVector`,
`FixedOrderedSet` and `FixedMap`.


## [0.1.0] - 2026-02-05

### Added
- Library skeleton: `README.md`, `LICENCE`, `library.properties, library.json`
, `DuinoCollections.hpp`.
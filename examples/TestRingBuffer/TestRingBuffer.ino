/*
 ******************************************************************************
 *  TestRingBuffer.ino
 *
 *  Testbed and examples for the FixedRingBuffer collection.
 *
 *  Author: Pierre DEBAS
 *  Copyright (c) 2026
 *
 *  MIT License
 *  https://github.com/Pierrolefou881/DuinoCollections
 *
 *  SPDX-License-Identifier: MIT
 *
 *  Description:
 *    This sketch provides tests and use case examples for the FixedRingBuffer
 *    collection, part of the DuinoCollections library.
 *
 ******************************************************************************
 */
#include <FixedRingBuffer.hpp>

#define _OVERWRITE
#define _RANGE_FOR
// #define _MUTABLE
// #define _CLEAR

const int CAPACITY{ 6 };
const char letters[] = { 'F', 'o', 'o', 'B', 'A', 'R', 'R', 'd', 'U', 'i', 'N', 'o' };

#ifdef _OVERWRITE
DuinoCollections::FixedRingBuffer<char, DuinoCollections::RingBufferMode::OVERWRITE> buffer{ CAPACITY };
#else
DuinoCollections::FixedRingBuffer<char> buffer{ CAPACITY };
#endif
int index{ };
bool is_popping{ };


void setup() {
    Serial.begin(9600);
    DuinoCollections::FixedRingBuffer<int> five{ };
    Serial.println(five.capacity());
}

void loop() {
    char _{ };
    if (index <= CAPACITY + 1)
    {
        if (!buffer.push(letters[index]))
        {
            Serial.println("BUFFER IS FULL");
        }
        index++;
    }
#ifdef _OVERWRITE
    else
    {
#elif defined(_CLEAR)
    else
    {
        buffer.clear();
#else
    else if (!buffer.pop(_))
    {
        Serial.println("BUFFER IS EMPTY");
#endif
        index = 0;
    }

    print_buffer();
    delay(500);
}

void print_buffer()
{
#ifdef _RANGE_FOR
    for (auto letter : buffer)
    {
    #ifdef _MUTABLE
        letter = 'Z';
    #endif
        Serial.print(letter);
#else
    for (auto i = 0; i < buffer.size(); i++)
    {
    #ifdef _MUTABLE
        buffer[i] = 'Z';
    #endif
        Serial.print(buffer[i]);
#endif
        Serial.print(", ");
    }
    Serial.print('\t');
    Serial.println(buffer.size());
}
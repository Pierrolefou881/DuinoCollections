/*
 ******************************************************************************
 *  TestVector.ino
 *
 *  Testbed and examples for the FixedVector collection.
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
 *    This sketch provides tests and use case examples for the FixedVector
 *    collection, part of the DuinoCollections library.
 *
 ******************************************************************************
 */
#include <FixedVector.hpp>

const size_t MAX_CAPACITY{ 26 };
DuinoCollections::FixedVector<char> vec{ MAX_CAPACITY };
size_t index{ };

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Tests push and capacity.
  if (index < MAX_CAPACITY + 1)
  {
    if (!vec.push('a' + index))
    {
      Serial.println("VECTOR IS FULL");
    }
    else
    {
      print_vector();
    }
    index++;
  }
  delay(500);
}

void print_vector()
{
  for (size_t i = 0; i < vec.size(); i++)
  {
    Serial.print(vec[i]);
    Serial.print(", ");
  }
  Serial.print('\t');
  Serial.println(vec.size());
}

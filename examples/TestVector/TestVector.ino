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

// Perform these tests in sequence (comment all except one).
// #define _PUSH_POP
#define _INSERT_REMOVE_CLEAR

const size_t MAX_CAPACITY{ 26 };
DuinoCollections::FixedVector<char> vec{ MAX_CAPACITY };
size_t index{ };
bool is_pushing{ true };
int led_status{ };

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
#ifdef _PUSH_POP
  // Tests push and capacity.
  if (index < MAX_CAPACITY + 1 && is_pushing)
  {
    if (!vec.push('a' + index))
    {
      Serial.println("VECTOR IS FULL");
      is_pushing = false;
    }
    else
    {
      print_vector();
    }
    index++;
  }
  else
  {
    index--;
    char val{ };
    if (vec.pop(val))
    {
      Serial.print("POPPED:\t");
      Serial.println(val);
      print_vector();
    }
    is_pushing = index == 0;
  }
#elif defined(_INSERT_REMOVE_CLEAR)
  if (vec.size() < 5)
  {
    vec.insert_at('a', 0);
  }
  else if (vec.size() < 10)
  {
    vec.insert_at('B', 2);
  }
  else if (vec.size() < 15)
  {
    vec.insert_at('c', vec.size());
  }
  else
  {
    // char _{ };
    // vec.remove_at(5, _);
    // vec.remove_first('B');
    // vec.remove_all('a');
    vec.clear();
  }
  print_vector();
  
#endif
  led_status ^= HIGH;
  digitalWrite(LED_BUILTIN, led_status);
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

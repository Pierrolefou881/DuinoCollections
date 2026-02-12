/*
 ******************************************************************************
 *  TestMap.ino
 *
 *  Testbed and examples for the FixedMap collection.
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
 *    This sketch provides tests and use case examples for the FixedMap
 *    collection, part of the DuinoCollections library.
 *
 ******************************************************************************
 */
#include <FixedMap.hpp>

DuinoCollections::FixedMap<int, float> the_map{ 8 };
const int keys[] = { 3, 20, 5, 1, 16, 6, 18, 1, 4 };
const float values[] = { 10e6, NAN, .0, 5.7, -10e3, 10e-4, INFINITY, 1.0, PI};
int index{ };
int led_state{ };

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    if (the_map.is_full())
    {
      the_map.clear();
      index = 0;
    }
    else 
    {
      if(!the_map.add(keys[index], values[index]))
      {
        Serial.println("KEY DUPLICATION DETECTED");
      }
      print_map();

      float current{ };
      if (the_map.try_get(keys[index], current))
      {
        Serial.print("Current Element:\t");
        Serial.println(current);
      }
      index++;
    }
    
    led_state ^= HIGH;
    digitalWrite(LED_BUILTIN, led_state);
    delay(2000);
}

void print_map() {
  for (auto keyval : the_map)
  {
    Serial.print('<');
    Serial.print(keyval.key);
    Serial.print(", ");
    Serial.print(keyval.value);
    Serial.print(">, ");
  }
  Serial.print('\t');
  Serial.println(the_map.size());
}
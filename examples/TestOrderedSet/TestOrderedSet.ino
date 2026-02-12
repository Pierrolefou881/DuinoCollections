/*
 ******************************************************************************
 *  TestOrderedSet.ino
 *
 *  Testbed and examples for the FixedOrderedSet collection.
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
 *    This sketch provides tests and use case examples for the FixedOrderedSet
 *    collection, part of the DuinoCollections library.
 *
 ******************************************************************************
 */
#include <FixedOrderedSet.hpp>
#include <FixedSet.hpp>

#define _DESCENDING

const size_t MAX_CAPACITY{ 26 };
const char data_set[] = { 'Z', 'f', 'u', 'b', 'A', 'r', 'A', 'M', 'n', 'I' };
const char to_remove[] = { 'Z', 'f', 'u', 'b', 'A', 'r', 'M', 'n', 'I', 'A' };
const int size{ 10 };
bool is_removing{ };
int index{ };

#ifdef _DESCENDING
DuinoCollections::FixedOrderedSet<char, DuinoCollections::Descending<char>> set { MAX_CAPACITY };
#else
DuinoCollections::FixedOrderedSet<char> set{ MAX_CAPACITY };
#endif

// Check compilation. FixedOrderedSet already tests all functionalities of FixedSet.
DuinoCollections::FixedSet<int> unordered{ };

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (index < size)
  {
    if (is_removing && !set.erase(to_remove[index]))
    {
      Serial.println("COLLECTION IS EMPTY");
    }
    else if (!is_removing && !set.insert(data_set[index]))
    {
      Serial.println("TRIED TO INSERT DUPLICATE");
    }

    print_set();
    index++;
  }
  else 
  {
    index = 0;
    is_removing ^= true;
  }
  delay(1000);
}

void print_set() {
  for (auto& item : set)
  {
    // Uncomment to check assignment. item should be const
    // and the following line should not compile.
    // item = 'Z';
    
    Serial.print(item);
    Serial.print(", ");
  }
  Serial.print('\t');
  Serial.println(set.size());
}
/*
 ******************************************************************************
 *  FixedVector.hpp
 *
 *  Fixed-size Vector implementation for the Arduino environment.
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
 *    Fixed-size, array-backed Vector implementation for Arduino compatible
 *    boards. Part of the DuinoCollections library.
 * 
 ******************************************************************************
 */
#pragma once
#include "internal/LinearCollection.hpp"

namespace DuinoCollections
{
    template<typename T>
    class FixedVector : public LinearCollection<T, FixedVector<T>>
    {

    };
}
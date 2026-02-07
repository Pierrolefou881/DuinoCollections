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
#include "internal/policy/IndexingPolicy.hpp"
#include "internal/policy/DuplicationPolicy.hpp"

namespace DuinoCollections
{
    template<typename T>
    class FixedVector : public Internal::LinearCollection<
        T, Internal::Policy::SequentialIndexingPolicy<T>, 
        Internal::Policy::AllowDuplicatePolicy<T>
    > 
    {
    public:
        using Base = Internal::LinearCollection<
            T, Internal::Policy::SequentialIndexingPolicy<T>,
            Internal::Policy::AllowDuplicatePolicy<T>
        >;

        FixedVector(size_t max_capacity = 5) : Base{ max_capacity }
        {
            // Empty body.
        }
    };
}
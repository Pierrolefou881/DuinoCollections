/*
 ******************************************************************************
 *  FixedOrderedSet.hpp
 *
 *  Fixed-size Set implementation for the Arduino environment.
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
    /**
     * Ordered collection of items that does not allow duplicates.
     * FixedOrderedSet does not allow indexed insertion, popping
     * out (does not behave as a stack) and duplications.
     * @param T can be any type as long as it implements a
     *        default initializer.
     */
    template<typename T>
    class FixedOrderedSet : public Internal::LinearCollection<
        T, Internal::Policy::OrderedIndexingPolicy<T>, 
        Internal::Policy::AllowDuplicatePolicy<T>
    > 
    {
    };
}
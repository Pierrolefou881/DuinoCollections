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
#include "SortingOrder.hpp"
#include "internal/policy/indexing/OrderedIndexingPolicy.hpp"
#include "internal/policy/duplication/ForbidDuplicationPolicy.hpp"

namespace DuinoCollections
{
    /**
     * Ordered collection of items that does not allow duplicates.
     * FixedOrderedSet does not allow indexed insertion, popping
     * out (does not behave as a stack) and duplications.
     * @param T can be any type as long as it implements a
     *        default initializer.
     */
    template<typename T, typename SortingOrder = Ascending<T>>
    class FixedOrderedSet : public Internal::LinearCollection<
        T, Internal::Policy::Indexing::OrderedIndexingPolicy<T, SortingOrder>, 
        Internal::Policy::Duplication::ForbidDuplicationPolicy<T>
    > 
    {
    public:
        using Base = Internal::LinearCollection<
            T, Internal::Policy::Indexing::OrderedIndexingPolicy<T, SortingOrder>,
            Internal::Policy::Duplication::ForbidDuplicationPolicy<T>
        >;

        FixedOrderedSet(size_t max_capacity) : Base{ max_capacity }
        {
            // Empty body.
        }

        bool insert(const T& item)
        {
            return Base::push(item);
        }
    };
}
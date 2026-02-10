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
#include "internal/policy/duplication/DuplicationPolicy.hpp"

namespace DuinoCollections
{
    /**
     * Ordered collection of items that does not allow duplicates.
     * FixedOrderedSet does not allow indexed insertion, popping
     * out (does not behave as a stack) and duplications.
     * @param T can be any type as long as it implements a
     *        default initializer, equality operators == and !=,
     *        and comparison operators <, <=, > and >=.
     * @param SortingOrder can be either ascending or descending.
     *        Defaulted to Ascending.
     */
    template<typename T, typename SortingOrder = Ascending<T>>
    class FixedOrderedSet : public Internal::LinearCollection<
        T, Internal::Policy::Indexing::OrderedIndexingPolicy<T, SortingOrder>, 
        Internal::Policy::Duplication::DuplicationPolicy::FORBID_DUPLICATES
    > 
    {
    public:
        using Base = Internal::LinearCollection<
            T, Internal::Policy::Indexing::OrderedIndexingPolicy<T, SortingOrder>,
            Internal::Policy::Duplication::DuplicationPolicy::FORBID_DUPLICATES
        >;

        /**
         * Initializes this FixedOrderedSet with the provided capacity.
         * If none is provided, it shall be defaulted to 5.
         * @param max_capacity maximum number of elements this FixedOrderedSet
         *        can contain. Defaulted to 5.
         */
        FixedOrderedSet(size_t max_capacity = 5) : Base{ max_capacity }
        {
            // Empty body.
        }

        /**
         * Inserts the provided item into this FixedOrderedSet, if possible.
         * If item already present or if this FixOrderedSet cannot contain
         * more elements (i.e. is full), insertion shall not occur.
         * @param item to insert.
         * @return true if insertion successful, false otherwise.
         */
        bool insert(const T& item)
        {
            return Base::push(item);
        }

        /**
         * Removes the provided item from this FixedOrderedSet, if possible.
         * Removal may fail if this FixedOrderedSet has no element to
         * remove (i.e. is empty) or the item is not present.
         * @param item to remove.
         * @return true 
         */
        bool erase(const T& item)
        {
            return Base::remove_first(item);
        }
    };
}
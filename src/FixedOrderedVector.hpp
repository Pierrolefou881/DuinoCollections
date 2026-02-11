/*
 ******************************************************************************
 *  FixedOrderedVector.hpp
 *
 *  Fixed-size sorted Vector implementation for the Arduino environment.
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
 *    Fixed-size, array-backed, sorted Vector implementation for 
 *    Arduino compatible boards. Part of the DuinoCollections library.
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
     * Ordered collection of items that allows duplicates.
     * Unlike FixedVector, OrderedFixedVector cannot be used
     * as a stack.
     * @param T can be any type as long as it implements a
     *        default initializer, equality operators == and !=
     *        and comparison operators <, <=, > and >=.
     * @param SortingOrder can be either ascending or descending.
     *        Defaulted to Ascending.
     */
    template<typename T, typename SortingOrder = Ascending<T>>
    class FixedOrderedVector : public Internal::LinearCollection<
        T, Internal::Policy::Indexing::OrderedIndexingPolicy<T, SortingOrder>,
        Internal::Policy::Duplication::DuplicationPolicy::ALLOW_DUPLICATES
    >
    {
        using Base = Internal::LinearCollection<
            T, Internal::Policy::Indexing::OrderedIndexingPolicy<T, SortingOrder>,
            Internal::Policy::Duplication::DuplicationPolicy::ALLOW_DUPLICATES
        >;

    public:
        /**
         * Initializes this FixedOrderedVector with the provided capacity.
         * If none is provided, it shall be defaulted to 5.
         * @param max_capacity maximum number of elements this FixedOrderedVector
         *        can contain. Defaulted to 5.
         */
        FixedOrderedVector(size_t max_capacity = 5) : Base{ max_capacity }
        {
            // Empty body.
        }

        /**
         * Inserts the provided element into this FixedOrderedVector.
         * Insertion may fail if the collection cannot accept new elements
         * (i.e. is full).
         * @param item to insert.
         * @return true if insertion successful, false otherwise.
         */
        bool insert(const T& item)
        {
            return Base::push(item);
        }

        /**
         * Removes the first occurrence of the provided item from this 
         * FixedOrderedVector, if possible. Removal may fail if this
         * FixedOrderedVector is empty or does not contain item.
         * @param item to remove.
         * @return true if item removed successfully, false otherwise.
         */
        bool remove_first(const T& item)
        {
            return Base::remove_first(item);
        }

        /**
         * Removes all instances of the provided item contained in this
         * FixedOrderedVector, if any. Removal may fail if the collection
         * is empty of item is not present at least once.
         * @param item to remove completely.
         * @return true if removal successful, false otherwise.
         */
        bool remove_all(const T& item)
        {
            return Base::remove_all(item);
        }

        /**
         * CAUTION: Undefined behavior if out of bounds. Always ensure collection
         *          is not empty before calling.
         * @return the first element of this FixedVector.
         */
        const T& front(void) const
        {
            return Base::at(0);
        }

        /**
         * CAUTION: Undefined behavior if out of bounds. Always ensure collection
         *          is not empty before calling.
         * @return the last element of this FixedVector.
         */
        const T& back(void) const
        {
            return Base::at(Base::size() - 1);
        }
    };
}
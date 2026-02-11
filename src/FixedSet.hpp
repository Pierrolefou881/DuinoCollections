/*
 ******************************************************************************
 *  FixedSet.hpp
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
 *    Fixed-size, array-backed Set implementation for Arduino compatible
 *    boards. Part of the DuinoCollections library.
 * 
 ******************************************************************************
 */
#pragma once
#include "internal/LinearCollection.hpp"
#include "internal/policy/indexing/SequentialIndexingPolicy.hpp"
#include "internal/policy/duplication/DuplicationPolicy.hpp"

namespace DuinoCollections
{
    /**
     * Fixed-size, sequential and non-ordered container that does not allow
     * duplicates. Elements can be accessed through their index, but not
     * modified.
     * @param T type of element. Can be any type as long as it has a default
     *        initializer and implements equality operators == and !=.
     */
    template<typename T>
    class FixedSet : public Internal::LinearCollection<
        T, Internal::Policy::Indexing::SequentialIndexingPolicy<T>,
        Internal::Policy::Duplication::DuplicationPolicy::FORBID_DUPLICATES
    >
    {
        using Base = Internal::LinearCollection<
            T, Internal::Policy::Indexing::SequentialIndexingPolicy<T>,
            Internal::Policy::Duplication::DuplicationPolicy::FORBID_DUPLICATES
        >;

    public:
        /**
         * Initializes this FixedSet with the provided maximum capacity.
         * If no capacity is provided, it shall be defaulted to 5.
         * @param max_capacity number of elements this FixedSet can contain at most.
         *        Defaulted to 5.
         */
        FixedSet(size_t max_capacity = 5) : Base{ max_capacity }
        {
            // Empty body.
        }

        /**
         * Inserts the provided item into this FixedSet. Insertion may fail if
         * the collection is already at full capacity or if item is already contained
         * in this FixedSet.
         * @param item to insert.
         * @return true if insertion was successful, false otherwise.
         */
        bool insert(const T& item)
        {
            return Base::push(item);
        }

        /**
         * Inserts the provided item at the provided index. Insertion may fail if
         * the collection is already at full capacity, if index is out of bounds
         * (i.e. superior to this FixedSet's size) or if item is already contained
         * in this FixedSet.
         * @param item to insert.
         * @param index where the insertion should occur.
         * @return true if insertion was successful, false otherwise.
         */
        bool insert_at(const T& item, size_t index)
        {
            return Base::insert_at(item, index);
        }

        /**
         * Removes item from this FixedSet. Removal may fail if the collection
         * is empty (i.e. contains no element) or if item is not present.
         * @param item to remove.
         * @return true if item was removed successfully, false otherwise.
         */
        bool erase(const T& item)
        {
            return Base::remove_first(item);
        }
    };
}
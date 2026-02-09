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
#include "internal/policy/indexing/SequentialIndexingPolicy.hpp"
#include "internal/policy/duplication/AllowDuplicationPolicy.hpp"

namespace DuinoCollections
{
    /**
     * Unordered collection of items that allows duplicates.
     * FixedVector can either be used as an array of items or
     * as a stack.
     * @param T can be any type as long as it implements a
     *        default initializer.
     */
    template<typename T>
    class FixedVector : public Internal::LinearCollection<
        T, Internal::Policy::Indexing::SequentialIndexingPolicy<T>, 
        Internal::Policy::Duplication::AllowDuplicationPolicy<T>
    > 
    {
    public:
        using Base = Internal::LinearCollection<
            T, Internal::Policy::Indexing::SequentialIndexingPolicy<T>,
            Internal::Policy::Duplication::AllowDuplicationPolicy<T>
        >;

        /**
         * Initializes this FixedVector as an empty collection with
         * the provided max capacity. If none is provided, a default
         * value shall be assigned.
         * @param max_capacity of this FixedVector. Defaulted to 5.
         */
        FixedVector(size_t max_capacity = 5) : Base{ max_capacity }
        {
            // Empty body.
        }

        /**
         * Adds the provided item to this FixedVector. Gives feedback
         * upon success or failure.
         * Success depends on size and capacity or duplication policies.
         * @param item to add to this FixedVector.
         * @return true if push was successful, false otherwise.
         */
        bool push(const T& item)
        {
            return Base::push(item);
        }

        /**
         * Removes the item at the index determined by the FixedVector. Gives
         * feedback on success or failure.
         * @param out_value removed item (out parameter).
         * @return true if pop was successful, false otherwise.
         */
        bool pop(T& out_value)
        {
            return Base::pop(out_value);
        }

        /**
         * Atomically adds an item to the collection.
         *
         * This method temporarily disables interrupts to ensure the operation
         * cannot be interrupted by an ISR accessing the same collection.
         * It is intended to protect concurrent access between the main context
         * (e.g. loop()) and an interrupt routine.
         *
         * Internally, this method calls push() within a critical section.
         *
         * @param item Item to add to the collection.
         * @return true if the item was successfully added, false otherwise
         *         (e.g. collection full, invalid storage, or duplication policy rejection).
         *
         * @note This method is portable across Arduino platforms.
         *
         * @warning This method must NOT be called from within an ISR, as it will
         * re-enable interrupts when exiting the critical section.
         */
        bool push_atomic(const T& item)
        {
            return Base::push_atomic(item);
        }

        /**
         * Atomically removes an item from the collection.
         *
         * This method temporarily disables interrupts to ensure the operation
         * cannot be interrupted by an ISR accessing the same collection.
         * It is intended to protect concurrent access between the main context
         * (e.g. loop()) and an interrupt routine.
         *
         * Internally, this method calls pop() within a critical section.
         *
         * @param out_value Reference where the removed item will be stored.
         * @return true if an item was successfully removed, false otherwise
         *         (e.g. collection empty or invalid storage).
         *
         * @note This method is portable across Arduino platforms.
         *
         * @warning This method must NOT be called from within an ISR, as it will
         * re-enable interrupts when exiting the critical section.
         */
        bool pop_atomic(T& out_value)
        {
            return Base::pop_atomic(out_value);
        }

        /**
         * Inserts the provided item at the provided index, if possible.
         * Insertion may fail if this FixedVector is full or if
         * index is out of bounds (exceeds _size). Insertion at index
         * _size is allowed.
         * @param item to insert.
         * @param index of insertion. Must be within bounds.
         * @return true if insertion successful, false otherwise.
         */
        bool insert_at(const T& item, size_t index)
        {
            return Base::insert_at(item, index);
        }

        /**
         * Removes the item at provided index from this FixedVector,
         * if possible. Removal may fail if this FixedVector is empty
         * or if index is out of bounds (exceeds or equals _size).
         * @param index where removal should occur.
         * @param out_item retrieved value, if any (out parameter).
         * @return true if removal successful, false otherwise.
         */
        bool remove_at(size_t index, T& out_item)
        {
            return Base::remove_at(index, out_item);
        }

        /**
         * Removes the first occurrence of the provided item from this
         * FixedVector. Removal may fail if item not present.
         * @param item to remove.
         * @return true if removal successful, false otherwise.
         */
        bool remove_first(const T& item)
        {
            return Base::remove_first(item);
        }

        /**
         * CAUTION: Undefined behavior if out of bounds. Always ensure collection
         *          is not empty before calling.
         * @return the first element of this FixedVector.
         */
        T& front(void)
        {
            return Base::at(0);
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
        T& back(void)
        {
            return Base::at(size() - 1);
        }

        /**
         * CAUTION: Undefined behavior if out of bounds. Always ensure collection
         *          is not empty before calling.
         * @return the last element of this FixedVector.
         */
        const T& back(void) const
        {
            return Base::at(size() - 1);
        }

        /**
         * Removes all instances of the provided item from this FixedVector.
         * Successful if at least one instance is removed.
         * @param item to purge out of this FixedVector.
         * @return true if removal successful, false otherwise.
         */
        bool remove_all(const T& item)
        {
            return Base::remove_all(item);
        }

        /**
         * @return the current number of elements accessible in this
         *         FixedVector.
         */
        size_t size(void) const
        {
            return Base::size();
        }

        /**
         * @return true if this FixedVector has reached its max capacity,
         *         false otherwise.
         */
        bool is_full(void) const
        {
            return Base::is_full();
        }

        /**
         * @return true if this FixedVector contains no element, false otherwise.
         */
        bool is_empty(void) const
        {
            return Base::is_empty();
        }
    };
}
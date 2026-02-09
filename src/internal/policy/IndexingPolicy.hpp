/*
 ******************************************************************************
 *  Indexing.hpp
 *
 *  Indexing and inserition policies to be used with LinearCollection.
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
 *    These types are internal and meant to be used as templates for
 *    LinearCollection and its derived types.
 * 
 *    All types in this file should implement the following method:
 *    
 *    size_t get_push_index(const T* data, size_t size, const T& item) const
 *    void insert(T* data, size_t size, size_t index, const T& item) const
 *    size_t get_pop_index(const T* data, size_t size) const
 *    void remove(T* data, size_t size, size_t index) const
 *    size_t find_index(const T* data, size_t size, const T& item) const
 *    size_t remove_all(T* data, size_t size, const T& item) const
 * 
 *    where T is the template type of items contained in the collection,
 *    data is the data array from the LinearCollection (visitor pattern),
 *    size is the current size of the LinearCollection and item is the
 *    item to insert, if policy allows, and index is the index where the
 *    insertion should occur.
 *    
 *    To avoid warnings, comment out the name (not the type) of all unused
 *    parameters.
 *    
 *    CAUTION: this file is an internal header and not part of the public API.
 * 
 ******************************************************************************
 */
#pragma once
#include <stddef.h>

namespace DuinoCollections
{
    namespace Internal
    {
        namespace Policy
        {
            /*
             ******************************************************************
             * SHIFT INDEX POLICIES
             ****************************************************************** 
             */

            /**
             * Base behavior for indexing policies that require left or
             * right shifting for data arrangement. This policy is
             * used by vectors and sets.
             * @param T type contained in the owning collection.
             */
            template<typename T>
            struct BaseShiftIndexPolicy
            {
                /**
                 * Inserts the provided item at the specified index and
                 * rearranges the data array accordingly through a
                 * rigth shift.
                 * @param data array from the owning collection.
                 * @param size of the owning collection.
                 * @param target_index where the insertion should occur.
                 * @param item to insert.
                 */
                void insert(T* data, size_t size, size_t target_index, const T& item) const
                {
                    for (size_t current_index = size; current_index > target_index; --current_index)
                    {
                        data[current_index] = data[current_index - 1];
                    }
                    data[target_index] = item;
                }

                /**
                 * Removes the item at the specidied index by performing a
                 * left shift in the data array.
                 * @param data array from the owning collection.
                 * @param size of the owning collection.
                 * @param target_index where deletion should occur.
                 */
                void remove(T* data, size_t size, size_t target_index) const
                {
                    for (size_t current_index = target_index; current_index < size - 1; ++current_index)
                    {
                        data[current_index] = data[current_index + 1];
                    }
                }

                /**
                 * Determines where popping out should occur. For ShiftIndexPolicyBase,
                 * always consider the last element to be popped out.
                 * @param data unused.
                 * @param size of the owning collection.
                 */
                size_t get_pop_index(const T* /*data*/, size_t size) const
                {
                    return size - 1;
                }

            protected:
                /**
                 * Initializes this BaseShiftIndexPolicy. The visibility
                 * is set as protected to avoid direct instantiation;
                 * BaseShiftIndexPolicy should always be regarded as an
                 * abstract class.
                 */
                BaseShiftIndexPolicy() = default;
            };

            /**
             * Defines sequential, unordered indexing policy.
             * @param T type contained in the owning collection. Must
             *          implement equality operators == and !=.
             */
            template<typename T>
            struct SequentialIndexingPolicy : public BaseShiftIndexPolicy<T>
            {
                static const bool IS_ORDERED{ false };

                /**
                 * Determines the index where a push should occur. For
                 * sequential and unordered policies, always consider
                 * appending as a push-in policy (stack behavior).
                 * @param data unused.
                 * @param size of the owning collection. Corresponds to the push-in index.
                 * @param item unused.
                 * @return size.
                 */
                size_t get_push_index(const T* /*data*/, size_t size, const T& /*item*/) const
                {
                    return size;
                }

                /**
                 * Finds the index of a provided item, if present.
                 * @param data array of the owning collection.
                 * @param size of the owning collection.
                 * @param item to find the index of.
                 * @return index of the first occurrence of item, if present;
                 *         size of collection otherwise.
                 */
                size_t find_index(const T* data, size_t size, const T& item) const
                {
                    for (size_t i = 0; i < size; ++i)
                    {
                        if (data[i] == item)
                        {
                            return i;
                        }
                    }

                    return size; // not found
                }

                /**
                 * Removes all occurrences of the provided item from the owning collection.
                 * @param data array of the owning collection.
                 * @param size of the owning collection.
                 * @param item to remove entirely from the owning collection.
                 * @return the number of occurrences deleted.
                 */
                size_t remove_all(T* data, size_t size, const T& item) const
                {

                    size_t write = 0;

                    for (size_t read = 0; read < size; ++read)
                    {
                        if (!(data[read] == item))
                        {
                            data[write++] = data[read];
                        }
                    }

                    return size - write;    // Number of occurrences removed.
                }

                // Forbid dynamic allocation
                void* operator new(size_t) = delete;
                void* operator new[](size_t) = delete;
                void operator delete(void*) = delete;
                void operator delete[](void*) = delete;
            };

            struct SearchResult
            {
                size_t index;
                bool found;
            };

            /**
             * Defines sequential, unordered indexing policy.
             * @param T type contained in the owning collection. Must
             *          implement equality operators == and != and
             *          comparison operators <, <=, > and >=.
             */
            template<typename T>
            struct OrderedIndexingPolicy : public BaseShiftIndexPolicy<T>
            {
                static const bool IS_ORDERED{ true };

                /**
                 * Returns the index where the item should be inserted in order to keep
                 * the collection sorted (ascending order).
                 *
                 * Uses binary search (lower_bound).
                 * Complexity: O(log n)
                 * 
                 * @param data array of the owning collection.
                 * @param size of the owning collection.
                 * @param item to push in.
                 * @return the item where insertion should occur.
                 */
                size_t get_push_index(const T* data, size_t size, const T& item) const
                {
                    size_t left = 0;
                    size_t right = size;

                    while (left < right)
                    {
                        size_t middle = left + ((right - left) >> 1);

                        if (data[middle] < item)
                        {
                            left = middle + 1;
                        }
                        else
                        {
                            right = middle;
                        }
                    }

                    // first index where data[index] >= item
                    return left;
                }

                /**
                 * Finds the index of the first occurrence of the provided item,
                 * if any.
                 *
                 * Uses binary search (lower_bound).
                 * Complexity: O(log n)
                 * 
                 * @param data array of the owning collection.
                 * @param size of the owning collection.
                 * @param item to find.
                 * @return index of the found item, size otherwise.
                 */
                size_t find_index(const T* data, size_t size, const T& item) const
                {
                    size_t left = 0;
                    size_t right = size;

                    while (left < right)
                    {
                        size_t mid = left + ((right - left) >> 1);

                        if (data[mid] < item)
                        {
                            left = mid + 1;
                        }
                        else
                        {
                            right = mid;
                        }
                    }

                    // left = lower_bound
                    if (left < size && data[left] == item)
                    {
                        return left;
                    }

                    return size; // not found
                }

                /**
                 * Determines the number of occurrences of the provided item, if
                 * any.
                 * 
                 * Uses binary search (lower_bound).
                 * Complexity: O(log n)
                 * 
                 * @param data array of the owning collection.
                 * @param size of the owning collection.
                 * @param item to remove entirely from the owning collection.
                 * @param firt occurrence of item (out parameter).
                 * @param last occurrence of item (out parameter).
                 */
                size_t remove_all(T* data, size_t size, const T& item, 
                                  size_t& first, size_t& last) const
                {
                    // lower_bound
                    size_t left = 0;
                    size_t right = size;

                    while (left < right)
                    {
                        size_t mid = left + ((right - left) >> 1);
                        if (data[mid] < item)
                            left = mid + 1;
                        else
                            right = mid;
                    }

                    if (left >= size || (item < data[left]) || (data[left] < item))
                    {
                        first = last = size;
                        return 0;
                    }

                    first = left;

                    // upper_bound
                    right = size;
                    while (left < right)
                    {
                        size_t mid = left + ((right - left) >> 1);
                        if (!(item < data[mid]))
                            left = mid + 1;
                        else
                            right = mid;
                    }

                    last = left;
                    return last - first;
                }

                /** 
                 * Determines whether an item can be inserted and where insertion should occur.
                 * @param data array of the owning collection.
                 * @param size of the owning collection.
                 * @param item to insert.
                 */
                SearchResult find_insert_position(const T* data, size_t size, const T& item) const
                {
                    size_t left = 0;
                    size_t right = size;

                    while (left < right)
                    {
                        size_t mid = left + ((right - left) >> 1);

                        if (data[mid] < item)
                            left = mid + 1;
                        else
                            right = mid;
                    }

                    bool found = (left < size) &&
                                !(item < data[left]) &&
                                !(data[left] < item);

                    return { left, found };
                }

                // Forbid dynamic allocation
                void* operator new(size_t) = delete;
                void* operator new[](size_t) = delete;
                void operator delete(void*) = delete;
                void operator delete[](void*) = delete;
            };
        }
    }
}
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
 *    size_t get_insertion_index(const T* data, size_t size, const T& item) const
 *    void insert(T* data, size_t size, size_t index, const T& item) const
 *    size_t get_removal_index(const T* data, size_t size) const
 *    void remove(T* data, size_t size, size_t index) const
 *    size_t find_index(const T* data, size_t size, const T& item) const
 *    size_t find_range(const T* data, size_t size, const T& item,
 *                      size_t& first, size_t& last) const;
 *
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
            template<typename T>
            struct ShiftIndexingPolicyBase
            {
                void insert(T* data, size_t size, size_t index, const T& value) const
                {
                    for (size_t i = size; i > index; --i)
                    {
                        data[i] = data[i - 1];
                    }
                    data[index] = value;
                }

                void remove(T* data, size_t size, size_t index) const
                {
                    for (size_t i = index; i < size - 1; ++i)
                    {
                        data[i] = data[i + 1];
                    }
                }

                // index = last element
                size_t get_removal_index(const T* /*data*/, size_t size) const
                {
                    return size - 1;
                }

            protected:
                ShiftIndexingPolicyBase() = default;
            };

            template<typename T>
            struct SequentialIndexingPolicy : public ShiftIndexingPolicyBase<T>
            {
                // index = after last element, no offset.
                size_t get_insertion_index(const T* /*data*/, size_t size, 
                                           const T& /*item*/) const
                {
                    return size;
                }

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

                size_t find_range(const T* data, size_t size, const T& item,
                                  size_t& first, size_t& last) const
                {
                    first = 0;

                    while (first < size && !(data[first] == item))
                        first++;

                    if (first == size)
                    {
                        last = size;
                        return 0;
                    }

                    last = first;
                    while (last < size && data[last] == item)
                        last++;

                    return last - first;
                }

            };

            template<typename T>
            struct OrderedIndexingPolicy : public ShiftIndexingPolicyBase<T>
            {
                /**
                 * Returns the index where the item should be inserted in order to keep
                 * the collection sorted (ascending order).
                 *
                 * Uses binary search (lower_bound).
                 * Complexity: O(log n)
                 */
                size_t get_insertion_index(const T* data, size_t size, const T& item) const
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

                size_t find_range(const T* data, size_t size, const T& item, 
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
            };
        }
    }
}
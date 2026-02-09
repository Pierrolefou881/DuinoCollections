/*
 ******************************************************************************
 *  OrderedIndexingPolicy.hpp
 *
 *  Indexing policy for sequential and ordered collections.
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
 *    This type is internal and should not be used outside the
 *    library DuinoCollections.
 * 
 *    All indexing policies must implement the following methods.
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
#include "BaseShiftIndexingPolicy.hpp"

namespace DuinoCollections
{
    namespace Internal
    {
        namespace Policy
        {
            namespace Indexing
            {
                /**
                 * Search result for the find_insert_position
                 * method of OrderedIndexingPolicy.
                 */
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
                 *          (at least < and > required).
                 * @param Compare ssor
                 */
                template<typename T, typename SortOrder>
                struct OrderedIndexingPolicy : public BaseShiftIndexingPolicy<T>
                {
                    static const bool IS_ORDERED{ true };

                    SortOrder order{};

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
                            if (order(data[middle], item))
                            {
                                left = middle + 1;
                            }
                            else
                            {
                                right = middle;
                            }
                        }

                        // first index where 
                        //    data[index] >= item (ascending order) 
                        // or data[index] <= item (descending order)
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
                        auto index = get_push_index(data, size, item);
                        return (index < size && data[index] == item) ? index : size;
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
                    size_t remove_all(T* data, size_t size, const T& item) const
                    {
                        // lower bound
                        auto lower_bound = find_index(data, size, item);
                        if (lower_bound == size || item != data[lower_bound])
                        {
                            return 0;
                        }

                        // upper_bound
                        auto left = lower_bound;
                        auto right = size;

                        while (left < right)
                        {
                            size_t middle = left + ((right - left) >> 1);
                            if (!order(item, data[middle]))
                            {
                                left = middle + 1;
                            }
                            else
                            {
                                right = middle;
                            }
                        }

                        auto count = left - lower_bound;
                        for (auto current_index = left; current_index < size; current_index++)
                        {
                            data[current_index - count = data[current_index]];
                        }

                        return count;
                    }

                    /** 
                     * Determines whether an item can be inserted and where insertion should occur.
                     * @param data array of the owning collection.
                     * @param size of the owning collection.
                     * @param item to insert.
                     */
                    SearchResult find_insert_position(const T* data, size_t size, const T& item) const
                    {
                        auto index = get_push_index(data, size, item);
                        bool found = index != size && item == data[index];
                        return { index, found };
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
}
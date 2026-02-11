/*
 ******************************************************************************
 *  SequentialIndexingPolicy.hpp
 *
 *  Indexing policy for sequential and unordered collections.
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
 *    SearchResult find_insert_position(const T* data, size_t size, const T& item) const
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
                 * Defines sequential, unordered indexing policy.
                 * @param T type contained in the owning collection. Must
                 *          implement equality operators == and !=.
                 */
                template<typename T>
                struct SequentialIndexingPolicy : public BaseShiftIndexingPolicy<T>
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

                    /** 
                     * Determines whether an item can be inserted and where insertion should occur.
                     * For SequentialIndexingPolicy, always true, always at the last element.
                     * @param data array of the owning collection.
                     * @param size of the owning collection.
                     * @param item to insert.
                     * @return { true, size }
                     */
                    SearchResult find_insert_position(const T* data, size_t size, const T& item) const
                    {
                        return { get_push_index(data, size, item), true };
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
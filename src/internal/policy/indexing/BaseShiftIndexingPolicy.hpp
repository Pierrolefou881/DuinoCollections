/*
 ******************************************************************************
 *  BaseShiftIndexingPolicy.hpp
 *
 *  Base behavior for sequential and ordered indexing policies.
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
#include <stddef.h>

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
                 * Base behavior for indexing policies that require left or
                 * right shifting for data arrangement. This policy is
                 * used by vectors and sets.
                 * @param T type contained in the owning collection.
                 */
                template<typename T>
                struct BaseShiftIndexingPolicy
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
                     * Initializes this BaseShiftIndexingPolicy. The visibility
                     * is set as protected to avoid direct instantiation;
                     * BaseShiftIndexingPolicy should always be regarded as an
                     * abstract class.
                     */
                    BaseShiftIndexingPolicy() = default;
                };
            }
        }
    }
}

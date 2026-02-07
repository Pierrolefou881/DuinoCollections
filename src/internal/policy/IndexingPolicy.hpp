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
 * 
 *    where T is the template type of items contained in the collection,
 *    data is the data array from the LinearCollection (visitor pattern),
 *    size is the current size of the LinearCollection and item is the
 *    item to insert, if policy allows, and index is the index where the
 *    insertion should occur.
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
            struct SequentialIndexingPolicy
            {
                // index = after last element, no offset.
                size_t get_insertion_index(const T* /*data*/, size_t size, 
                                           const T& /*item*/) const
                {
                    return size;
                }

                void insert(T* data, size_t /*size*/, size_t index,
                            const T& value) const
                {
                    data[index] = value;
                }
            };
        }
    }
}
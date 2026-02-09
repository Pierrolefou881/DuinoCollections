/*
 ******************************************************************************
 *  AllowDuplicationPolicy.hpp
 *
 *  Policy for collections that always allows item duplicates.
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
 *    bool allows(const T* data, size_t size, const T& item) const
 * 
 *    where T is the template type of items contained in the collection,
 *    data is the data array from the LinearCollection (visitor pattern),
 *    size is the current size of the LinearCollection and item is the
 *    item to insert, if policy allows.
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
            namespace Duplication
            {
                /**
                 * This policy allows duplicates at all times. It is mainly
                 * used in FixedVector, FixedOrderedVector and FixedRingBuffer.
                 * @param T type of element contained in the owning collection.
                 */
                template<typename T>
                struct AllowDuplicationPolicy
                {
                    static const bool ALLOWS_DUPLICATES{ true };
                    
                    /** 
                     * @param data unused.
                     * @param size unused.
                     * @param item unused.
                     * @return always true. 
                     */
                    bool allows(const T* /*data*/, size_t /*size*/, const T& /*item*/) const
                    {
                        return ALLOWS_DUPLICATES;
                    }
                };
            }
        }
    }
}

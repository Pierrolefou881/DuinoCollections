/*
 ******************************************************************************
 *  SortingOrder.hpp
 *
 *  Sorting orders to be used in collection indexing policies.
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
 *    Sorting order to be used for data comparison in OrderedIndexingPolicy.
 *    Can be used to initialize templates with ordered collections.
 * 
 *    ex:
 *      FixedOrderedSet<int, Descending<int>> set{ 20 };
 * 
 ******************************************************************************
 */
#pragma once

namespace DuinoCollections
{

    /**
     * Sorting order to be used with OrderedIndexPolicy as the
     * SortOrder type. It is also a template type for instancing
     * ordered collections.
     * 
     * example:
     *      FixedOrderedSet<int, Ascending<int>> set{ 20 };
     * 
     * @param T type contained in the owning collection. Must
     *          implement equality operators == and != and
     *          comparison operators <, <=, > and >= 
     *          (at least < and > required).
     */
    template<typename T>
    struct Ascending
    {
        bool operator()(const T& a, const T& b) const
        {
            return a < b;
        }
    };

    /**
     * Sorting order to be used with OrderedIndexPolicy as the
     * SortOrder type. It is also a template type for instancing
     * ordered collections.
     * 
     * example:
     *      FixedOrderedSet<int, Descending<int>> set{ 20 };
     * 
     * @param T type contained in the owning collection. Must
     *          implement equality operators == and != and
     *          comparison operators <, <=, > and >= 
     *          (at least < and > required).
     */
    template<typename T>
    struct Descending
    {
        bool operator()(const T& a, const T& b) const
        {
            return a > b;
        }
    };
}
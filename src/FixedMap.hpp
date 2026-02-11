/*
 ******************************************************************************
 *  FixedMap.hpp
 *
 *  Fixed-size Map implementation for the Arduino environment.
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
 *    Fixed-size, array-backed Map implementation for Arduino compatible
 *    boards. Part of the DuinoCollections library.
 * 
 ******************************************************************************
 */
#pragma once
#include "internal/LinearCollection.hpp"
#include "SortingOrder.hpp"
#include "internal/policy/indexing/OrderedIndexingPolicy.hpp"
#include "internal/policy/duplication/DuplicationPolicy.hpp"

namespace DuinoCollections
{
    /**
     * Data element to be used in FixedMap, associates a unique, comparable
     * key with a value.
     * @param K type of key. Must have a default initializer and must
     *        implement equality operators == and != and comparison
     *        operators <, <=, >, >=. Usually integral (int, uint, size_t...).
     * @param V type of value. Must implement a default initializer.
     */
    template<typename K, typename V>
    struct KeyValue
    {
        const K key{ };
        V value{ };

        /**
         * Initializes this KeyValue as a default one.
         */
        KeyValue(void) = default;

        /**
         * Initializes this KeyValue with the provided
         * key and value.
         * @param a_key must be unique.
         * @param a_value can be any value of its type.
         */
        KeyValue(const K& a_key, const V& a_value)
            : key{ a_key }, value{ a_value}
        {
            // Empty body.
        }

        friend bool operator ==(const KeyValue<K, V>& a, const KeyValue<K, V>& b)
        {
            return a.key == b.key;
        }

        friend bool operator !=(const KeyValue<K, V>& a, const KeyValue<K, V>& b)
        {
            return !(a == b);
        }

        friend bool operator >(const KeyValue<K, V>& a, const KeyValue<K, V>& b)
        {
            return a.key > b.key;
        }

        friend bool operator >=(const KeyValue<K, V>& a, const KeyValue<K, V>& b)
        {
            return a > b || a == b;
        }

        friend bool operator <(const KeyValue<K, V>& a, const KeyValue<K, V>& b)
        {
            return !(a >= b);
        }

        friend bool operator <=(const KeyValue<K, V>& a, const KeyValue<K, V>& b)
        {
            return !(a > b);
        }
    };

    /**
     * Fixed-size collection of values indexed by a unique key.
     * The FixedMap is sequential, ordered and does not allow duplicate
     * keys (duplicates values are allowed). The association is represented by
     * the struct KeyValue.
     * @param K type of key. Must have a default initializer and must
     *        implement equality operators == and != and comparison
     *        operators <, <=, >, >=. Usually integral (int, uint, size_t...).
     * @param V type of value. Must implement a default initializer.
     */
    template<typename K, typename V>
    class FixedMap : public Internal::LinearCollection<KeyValue<K, V>,
        Internal::Policy::Indexing::OrderedIndexingPolicy<KeyValue<K, V>, Ascending<KeyValue<K,V>>>,
        Internal::Policy::Duplication::DuplicationPolicy::FORBID_DUPLICATES
    >
    {
        using Base = Internal::LinearCollection<KeyValue<K, V>, 
            Internal::Policy::Indexing::OrderedIndexingPolicy<KeyValue<K, V>, Ascending<KeyValue<K, V>>>,
            Internal::Policy::Duplication::DuplicationPolicy::FORBID_DUPLICATES
        >;
    
    public:
    };
}
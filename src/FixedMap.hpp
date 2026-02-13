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
        K key{ };
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
        explicit KeyValue(const K& a_key, const V& a_value)
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
        /**
         * Initializes this FixedMap with the provided maximum capacity.
         * If no capacity if provided, it shall be defaulted to 5.
         * @param max_capacity maximum number of KeyValues this FixedMap can
         *        contain. Defaulted to 5.
         */
        FixedMap(size_t max_capacity = 5) : Base{ max_capacity }
        {
            // Empty body.
        }

        /**
         * Adds the provided item and indexes it with the provided key.
         * Add may fail if this FixedMap is already at full capacity
         * (size == max capacity) or if key already in use.
         * @param key must be unique, i.e. not already in use.
         * @param value can be any value supported by the type V.
         * @return true if add was successful, false otherwise.
         */
        bool add(const K& key, const V& value)
        {
            return Base::push(KeyValue<K, V>{ key, value });
        }

        /**
         * Removes the item indexed a the provided index from this FixedMap and
         * frees index. Removal may fail if this FixedMap has no element (i.e. is empty)
         * or if key is not used.
         * @param key indexing the item to remove.
         * @param out_val value removed, if found; default otherwise (out parameter).
         * @return true if removal was successful, false otherwise.
         */
        bool remove(const K& key, V& out_val)
        {
            KeyValue<K, V> keyval{ key, V{ }};
            auto index = Base::find(keyval);
            if (index == Base::size())
            {
                return false;
            }

            Base::remove_at(index, keyval);
            out_val = keyval.value;
            return true;
        }

        /**
         * Fetches the value at the provided index without removing it from this
         * FixedMap.
         * @param key indexing the item to find.
         * @param out_val item, if found (out parameter).
         * @return true if item found, false otherwise.
         */
        bool try_get(const K& key, V& out_val)
        {
            auto index = Base::find(KeyValue<K, V>{ key, V{ } });
            bool is_found = index != Base::size();
            if (is_found)
            {
                out_val = Base::data()[index].value;
            }
            return is_found;
        }
    };
}
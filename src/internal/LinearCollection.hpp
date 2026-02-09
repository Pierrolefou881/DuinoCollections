/*
 ******************************************************************************
 *  LinearCollection.hpp
 *
 *  Backend logic for all fixed-size, array-backed collections.
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
 *    This internal class is meant to be used as a base for fixed-size,
 *    array-backed collections from the DuinoCollection library.
 *    
 *    CAUTION: this file is an internal header and not part of the public API.
 * 
 ******************************************************************************
 */
#pragma once
#include "utils/ScopedInterruptLock.hpp"

namespace DuinoCollections
{
    namespace Internal
    {
        /**
         * Backbone for fixed size, array-backed collections. Due to hardware
         * restrictions on most Arduino compatible boards, LinearCollection
         * implementations shall:
         * <ul>
         *     <li>Use arrays for data storage.</li>
         *     <li>Have a fixed maximal capacity and allocate dynamically once 
         *         at construction time and free memory on destruction.</li>
         *     <li>Provide failsafes in case of allocation failure.</li>
         *     <li>Allow indexing but be transparent on potential UBs.</li>
         *     <li>Return feedback on critical operations (push, pop, insert, remove...).</li>
         * </ul>
         * 
         * LinarCollection should be considered as an abstract type and not instanced
         * directly.
         * LinearCollection defines the common interface and enforces invariants. Derived
         * collection types provide façade and pick the methods that suit their needs.
         * This architecture allows compile-time polymorphism on performance-critical
         * paths and avoids RAM-expensive virtual tables. 
         * @param T type of data stored in this LinearCollection concrete
         *        implementation. Can be any type as long as it has a
         *        default initializer.
         * @param IndexingPolicy defines where insertions should occur.
         * @param DuplicationPolicy defines whether duplicates are allowed or not.
         */
        template<typename T, typename IndexingPolicy, typename DuplicationPolicy>
        class LinearCollection
        {    
        public:
            ~LinearCollection()
            {
                delete[] _data;
            }

            LinearCollection(const LinearCollection<T, IndexingPolicy, DuplicationPolicy>& other) = delete;

            LinearCollection(LinearCollection<T, IndexingPolicy, DuplicationPolicy>&& other) noexcept
                : _data{ other._data }, _capacity{ other._capacity }, _size{ other._size }
                , _index_policy{ other._index_policy }, _duplication_policy{ other._duplication_policy }
            {
                // Empty body.
            }

            /**
             * Removes all items from this LinearCollection. Memory is not actually
             * free, but might be erased later on push or insertion.
             */
            void clear(void)
            {
                _size = 0;
            }

            /**
             * @return true if _data member is usable, false otherwise.
             */
            [[nodiscard]]
            bool is_valid(void) const
            {
                return _data != nullptr;
            }
            
            /**
             * @return the maximum number of elements this LinearCollection
             *         can contain.
             */
            size_t capacity(void) const
            {
                return _capacity;
            }

            /**
             * CAUTION: Undefined behavior if out of bounds. Always ensure
             * index <  size().
             */
            T& operator [](size_t index)
            {
                return _data[index];
            }

            LinearCollection<T, IndexingPolicy, DuplicationPolicy>& operator =(
                const LinearCollection<T, IndexingPolicy, DuplicationPolicy>& other) = delete;

            LinearCollection<T, IndexingPolicy, DuplicationPolicy>& operator =(
                LinearCollection<T, IndexingPolicy, DuplicationPolicy>&& other) noexcept
            {
                _data = other._data;
                _capacity = other._capacity;
                _size = other._size;
                _index_policy = other._index_policy;
                _duplication_policy = other._duplication_policy;
            }

        protected:
            /**
             * Initializes this LinearCollection with the provided maximum
             * capacity.
             * @param capacity must be strictly positive. Defaulted to 5.
             */
            explicit LinearCollection(size_t capacity = 5)
                : _data{ capacity > 0 ? new T[capacity] : nullptr }
                , _capacity{ capacity }
                , _size{ 0 }
                , _index_policy{ }
                , _duplication_policy{ }
            
            {
                if (_data == nullptr)
                {
                    _capacity = 0;
                }
            }

            /**
             * Adds the provided item to this LinearCollection. Gives feedback
             * upon success or failure.
             * Success depends on size and capacity or duplication policies.
             * @param item to add to this LinearCollection.
             * @return true if push was successful, false otherwise.
             */
            bool push(const T& item)
            {
                if (!is_valid() || is_full)
                {
                    return false;
                }

                size_t index{ };
                bool can_add{ };

                // Ordered and does not allow duplicate, avoid double search.
                if (IndexingPolicy::IS_ORDERED && DuplicationPolicy::ALLOWS_DUPLICATES)
                {
                    auto res = _index_policy.find_insert_position(_data, _size, item);
                    index = res.index;
                    can_add = !res.found;
                }

                // Fallback for generic case.
                else
                {
                    index = _index_policy.get_push_index(_data, _size, item);
                    can_add = _duplication_policy.allows(_data, size, item);
                }

                if (!can_add)
                {
                    return false;
                }

                _index_policy.insert(_data, _size, index, item);
                _size++;
                return true;
            }

            /**
             * Removes the item at the index determined by the IndexingPolicy. Gives
             * feedback on success or failure.
             * @param out_value removed item (out parameter).
             * @return true if pop was successful, false otherwise.
             */
            bool pop(T& out_value)
            {
                if (!is_valid() || is_empty())
                {
                    return false;
                }

                auto index = _index_policy.get_pop_index(_data, _size);
                out_value = _data[index];
                _index_policy.remove(_data, _size, index);
                _size--;
                return true;
            }

            /**
             * Atomically adds an item to the collection.
             *
             * This method temporarily disables interrupts to ensure the operation
             * cannot be interrupted by an ISR accessing the same collection.
             * It is intended to protect concurrent access between the main context
             * (e.g. loop()) and an interrupt routine.
             *
             * Internally, this method calls push() within a critical section.
             *
             * @param item Item to add to the collection.
             * @return true if the item was successfully added, false otherwise
             *         (e.g. collection full, invalid storage, or duplication policy rejection).
             *
             * @note This method is portable across Arduino platforms.
             *
             * @warning This method must NOT be called from within an ISR, as it will
             * re-enable interrupts when exiting the critical section.
             */
            bool push_atomic(const T& item)
            {
                Utils::ScopedInterruptLock lock{};
                return push(item);
            }

            /**
             * Atomically removes an item from the collection.
             *
             * This method temporarily disables interrupts to ensure the operation
             * cannot be interrupted by an ISR accessing the same collection.
             * It is intended to protect concurrent access between the main context
             * (e.g. loop()) and an interrupt routine.
             *
             * Internally, this method calls pop() within a critical section.
             *
             * @param out_value Reference where the removed item will be stored.
             * @return true if an item was successfully removed, false otherwise
             *         (e.g. collection empty or invalid storage).
             *
             * @note This method is portable across Arduino platforms.
             *
             * @warning This method must NOT be called from within an ISR, as it will
             * re-enable interrupts when exiting the critical section.
             */
            bool pop_atomic(T& out_value)
            {
                Utils::ScopedInterruptLock lock{};
                return pop(out_value);
            }

            /**
             * Inserts the provided item at the provided index, if possible.
             * Insertion may fail if this LinearCollection is full or if
             * index is out of bounds (exceeds _size). Insertion at index
             * _size is allowed.
             * @param item to insert.
             * @param index of insertion. Must be within bounds.
             * @return true if insertion successful, false otherwise.
             */
            bool insert_at(const T& item, size_t index)
            {
                if (!is_valid() || is_full() 
                        || !_duplication_policy.allows(_data, _size, item) || index > _size)
                {
                    return false;
                }

                _index_policy.insert(_data, _size, index, item);
                _size++;
                return true;
            }

            /**
             * Removes the item at provided index from this LinearCollection,
             * if possible. Removal may fail if this LinearCollection is empty
             * or if index is out of bounds (exceeds or equals _size).
             * @param index where removal should occur.
             * @param out_item retrieved value, if any (out parameter).
             * @return true if removal successful, false otherwise.
             */
            bool remove_at(size_t index, T& out_item)
            {
                if (!is_valid() || is_empty() || index > _size)
                {
                    return false;
                }
                
                out_item = _data[index];
                _index_policy.remove(_data, _size, index);
                _size--;
                return true;
            }

            /**
             * Removes the first occurrence of the provided item from this
             * LinearCollection. Removal may fail if item not present.
             * @param item to remove.
             * @return true if removal successful, false otherwise.
             */
            bool remove_first(const T& item)
            {
                if (!is_valid() || is_empty())
                {
                    return false;
                }
                auto index = _index_policy.find_index(_data, _size, item);
                if (index == _size)
                {
                    return false;
                }

                _index_policy.remove(_data, _size, index);
                _size--;
                return true;
            }

            /**
             * Removes all instances of the provided item from this LinearCollection.
             * Successful if at least one instance is removed.
             * @param item to purge out of this LinearCollection.
             * @return true if removal successful, false otherwise.
             */
            bool remove_all(const T& item)
            {
                if (!is_valid() || is_empty())
                {
                    return false;
                }

                auto count = _index_policy.remove_all(_data, _size, item);
                _size -= count;
                return count > 0;
            }

            /**
             * @return the current number of elements accessible in this
             *         LinearCollection.
             */
            size_t size(void) const
            {
                return _size;
            }

            /**
             * @return true if this LinearCollection has reached its max capacity,
             *         false otherwise.
             */
            bool is_full(void) const
            {
                return _size >= _capacity;
            }

            /**
             * @return true if this LinearCollection contains no element, false otherwise.
             */
            bool is_empty(void) const
            {
                return _size == 0;
            }

        private:
            T* _data{ };
            size_t _capacity{ };
            size_t _size{ };
            IndexingPolicy _index_policy{ };
            DuplicationPolicy _duplication_policy{ };

            bool can_add_item(const T& item) const
            {
                return is_valid() && !is_full() 
                    && _duplication_policy.allows(_data, _size, item);
            }
        };
    }
}
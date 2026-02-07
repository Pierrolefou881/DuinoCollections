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
         *     <li>Allow polymorphic usage while keeping critical paths non-virtual.</li>
         *     <li>Allow indexing but be transparent on potential UBs.</li>
         *     <li>Return feedback on critical operations (push, pop, insert, remove...).</li>
         * </ul>
         * 
         * LinarCollection should be considered as an abstract type and not instanced
         * directly.
         *
         * LinearCollection defines the common public interface and enforces invariants, 
         * while derived collection types provide concrete implementations, 
         * allowing compile-time polymorphism on performance-critical paths, 
         * thus avoiding RAM-expensive virtual tables.
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

            /**
             * Adds the provided item to this LinearCollection. Gives feedback
             * upon success or failure.
             * Success depends on size and capacity or duplication policies.
             * @param item to add to this LinearCollection.
             * @return true if push was successful, false otherwise.
             */
            bool push(const T& item)
            {
                if (_data == nullptr || is_full() 
                        || !_duplication_policy.allows(_data, _size, item))
                {
                    return false;
                }

                auto index = _index_policy.get_insertion_index(_data, _size, item);
                _index_policy.insert(_data, _size, index, item);
                _size++;
                return true;
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
             * CAUTION: Undefined behavior if out of bounds. Always ensure
             * index <  size().
             */
            T& operator [](size_t index)
            {
                return _data[index];
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
            
            {
                if (_data == nullptr)
                {
                    _capacity = 0;
                }
            }

        private:
            T* _data{ };
            size_t _capacity{ };
            size_t _size{ };
            IndexingPolicy _index_policy{ };
            DuplicationPolicy _duplication_policy{ };
        };

    }
}
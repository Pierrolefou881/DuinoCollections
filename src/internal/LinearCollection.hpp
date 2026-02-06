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
         * To avoid virtual tables and ram consumption due to virtual tables,
         * LinearCollection uses the Curiously Recurring Template Pattern
         * (CRTP) as a means for inheritance.
         * @param T type of data stored in this LinearCollection concrete
         *        implementation. Can be any type as long as it has a
         *        default initializer.
         * @param Derived name of the derived type, i.e. the concrete
         *        implementation of this LinearCollection.
         */
        template<typename T, typename Derived>
        class LinearCollection
        {    
        public:
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
            
        protected:
            /**
             * Initializes this LinearCollection with the provided maximum
             * capacity.
             * @param capacity must be strictly positive. Defaulted to 5.
             */
            explicit LinearCollection(size_t capacity = 5)
                : _capacity{ capacity },
                _data{ capacity > 0 ? new (std::nothrow) T[capacity] : nullptr }
            {
                // Empty body.
            }
    
            virtual ~LinearCollection()
            {
                delete[] _data;
            }

        private:
            T* _data{ };
            size_t _capacity{ };
        };

    }
}
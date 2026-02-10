/*
 ******************************************************************************
 *  Iterator.hpp
 *
 *  Small iterator to be used with DuinoCollection collections.
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
 *    These iterators are designed to allow range for collection parsing
 *    for (auto& item : collection) and for (const auto& item : collection)
 *    without exposing the raw pointer. They should not be used in any
 *    other context.
 *    
 *    CAUTION: this file is an internal header and not part of the public API.
 * 
 ******************************************************************************
 */
#pragma once

namespace DuinoCollections
{
    namespace Utils
    {
        /**
         * Enables iteration over collections without exposing the
         * raw data pointer to the public API.
         * @param T type of data contained in the calling collection.
         */
        template<typename T>
        class Iterator
        {
        public:
            /**
             * Initializes this Iterator with the provided raw data pointer.
             * @param ptr data array to iterate over.
             */
            explicit Iterator(T* ptr) : _ptr(ptr) 
            {
                // Empty body.
            }

            /**
             * @return the item currently iterated over.
             */
            T& operator*() const 
            { 
                return *_ptr; 
            }

            // move to the next item
            Iterator& operator++() 
            { 
                _ptr++; 
                return *this; 
            }

            // Useful for checking if iteration has reached end().
            bool operator!=(const Iterator& other) const 
            { 
                return _ptr != other._ptr; 
            }

        private:
            T* _ptr;
        };

        /**
         * Enables iteration over collections without exposing the
         * raw data pointer to the public API. This version gives access
         * to non-mutable data.
         * @param T type of data contained in the calling collection.
         */
        template<typename T>
        class ConstIterator
        {
        public:
            /**
             * Initializes this Iterator with the provided raw data pointer.
             * @param ptr data array to iterate over.
             */
            explicit ConstIterator(const T* ptr) : _ptr(ptr) 
            {
                // Empty body
            }

            /**
             * @return the item currently iterated over.
             */
            const T& operator*() const 
            { 
                return *_ptr; 
            }

            // Move on to the next item.
            ConstIterator& operator++() 
            { 
                _ptr++; 
                return *this; 
            }

            // Useful for checking if iteration has reached end().
            bool operator!=(const ConstIterator& other) const 
            { 
                return _ptr != other._ptr; 
            }

        private:
            const T* _ptr;
        };

    }
}
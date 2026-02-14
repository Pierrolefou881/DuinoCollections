/*
 ******************************************************************************
 *  FixedRingBuffer.hpp
 *
 *  Fixed-size circular buffer implementation for the Arduino environment.
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
 *    Fixed-size, array-backed circular buffer implementation for Arduino 
 *    compatible boards. Part of the DuinoCollections library.
 * 
 ******************************************************************************
 */
#pragma once
#include <stddef.h>
#include "internal/utils/ScopedInterruptLock.hpp"

namespace DuinoCollections
{
    /**
     * Fixed-capacity circular FIFO buffer. Provides
     * fast and ISR-safe data access.
     * @param T type of objects contained. Must have a default
     *          initializer.
     */
    template<typename T>
    class FixedRingBuffer
    {
    public:
        /**
         * Initializes this FixedRingBuffer with the provided max_capacity.
         * If none is provided, it shall be defaulted to 5.
         * @param max_capacity maximum number of elements (size) this FixedRingBuffer
         *        can have. Defaulted to 5.
         */
        explicit FixedRingBuffer(size_t max_capacity = 5)
            : _data{ max_capacity > 0 ? new T[max_capacity] : nullptr }
            , _capacity{ max_capacity }
            , _size{ 0 }
            , _head{ 0 }
            , _tail{ 0 }
        {
            if (_data == nullptr)
            {
                _capacity = 0;
            }
        }

        ~FixedRingBuffer(void)
        {
            delete[] _data;
        }

        // Forbid copy to avoid double delete.
        FixedRingBuffer(const FixedRingBuffer&) = delete;
        FixedRingBuffer& operator=(const FixedRingBuffer&) = delete;

        FixedRingBuffer(FixedRingBuffer&& other) noexcept
            : _data{ other._data }
            , _capacity{ other._capacity }
            , _size{ other._size }
            , _head{ other._head }
            , _tail{ other._tail }
        {
            other._data = nullptr;
            other._capacity = 0;
            other._size = 0;
            other._head = 0;
            other._tail = 0;
        }

        FixedRingBuffer& operator=(FixedRingBuffer&& other) noexcept
        {
            if (this != &other)
            {
                delete[] _data;

                _data = other._data;
                _capacity = other._capacity;
                _size = other._size;
                _head = other._head;
                _tail = other._tail;

                other._data = nullptr;
                other._capacity = 0;
                other._size = 0;
                other._head = 0;
                other._tail = 0;
            }
            return *this;
        }

        /**
         * Push the provided item at the end of this FixedRingBuffer.
         * Push may fail if this FixedRingBuffer is full.
         * @return true if push was successful, false otherwise.
         */
        bool push(const T& item)
        {
            if (!is_valid() || is_full())
            {
                return false;
            }

            _data[_tail] = item;
            _tail = next(_tail);
            _size++;
            return true;
        }

        /**
         * Pops the oldest element in this FixedRingBuffer.
         * Pop may fail if this FixedRingBuffer is empty.
         * @return true if pop was successful, false otherwise.
         */
        bool pop(T& out_value)
        {
            if (!is_valid() || is_empty())
            {
                return false;
            }

            out_value = _data[_head];
            _head = next(_head);
            _size--;
            return true;
        }

        /**
         * @return true if _data is valid (not null), false otherwise.
         */
        bool is_valid(void) const 
        { 
            return _data != nullptr; 
        }

        /**
         * @return the maximum number of elements this FixedRingBuffer can hold.
         */
        size_t capacity(void) const 
        { 
            return _capacity; 
        }
        
        /**
         * @return the number of elements contained in this FixedRingBuffer.
         */
        size_t size(void) const 
        { 
            return _size; 
        }

        /**
         * @return true if this RingBuffer contains no elements, false otherwise.
         */
        bool is_empty(void) const 
        { 
            return _size == 0; 
        }

        /**
         * @return true if this FixedRingBuffer cannot accept new elements,
         *         false otherwise.
         */
        bool is_full(void) const 
        { 
            return _size >= _capacity;
        }

        /**
         * Frees all slots in the _data array and marks
         * this FixedRingBuffer as empty.
         */
        void clear(void)
        {
            _size = 0;
            _head = 0;
            _tail = 0;
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
         *         (e.g. collection full or invalid storage).
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
         * Access a mutable reference to the element at the specified index.
         * CAUTION: Undefined behavior if out of bounds. Always ensure
         * index <  size().
         * @param index must be within bounds.
         * @return non-const reference to the element at index.
         */
        T& at(size_t index)
        {
            return _data[physical_index(index)];
        }

        /**
         * Access the element at the given index.
         * CAUTION: Undefined behavior if out of bounds. Always ensure
         * index <  size().
         * @param index of the item to access.
         * @return the reference to the item at index.
         */
        const T& at(size_t index) const
        {
            return _data[physical_index(index)];
        }

        T& operator [](size_t index)
        {
            return at(index);
        }

        const T& operator [](size_t index) const
        {
            return at(index);
        }

        T& front(void)
        {
            return _data[_head];
        }

        const T& front(void) const
        {
            return _data[_head];
        }

        T& back(void)
        {
            return _data[prev(_tail)];
        }

        const T& back(void) const
        {
            return _data[prev(_tail)];
        }

        // ---------------------------------------------------------------------
        // Iteration support (logical order)
        // ---------------------------------------------------------------------
        /**
         * Iterates over mutable references from a FixedRingBuffer.
         */
        class RingBufferIterator
        {
        public:
            /**
             * Initializes this RingBufferIterator with the provided
             * pointer to the iterated over FixedRingBuffer and its
             * head index.
             * @param buffer must not be null.
             * @param index of the buffer's head.
             */
            RingBufferIterator(FixedRingBuffer* buffer, size_t index)
                : _buffer{ buffer }, _index{ index }
            {
                // Empty body.
            }

            T& operator*(void)
            {
                return _buffer->at(_index);
            }

            RingBufferIterator& operator++(void)
            {
                _index++;
                return *this;
            }

            bool operator!=(const RingBufferIterator& other) const
            {
                return _index != other._index;
            }

        private:
            FixedRingBuffer* _buffer{ };
            size_t _index{ };
        };

        /**
         * Iterates over non-mutable references from a FixedRingBuffer.
         */
        class ConstRingBufferIterator
        {
        public:
            /**
             * Intializes this ConstRingBufferIterator with the provided pointer
             * to a RingBuffer and its head index.
             * @param buffer must not be null.
             * @param index of the buffer's head.
             */
            ConstRingBufferIterator(const FixedRingBuffer* buffer, size_t index)
                : _buffer{ buffer }, _index{ index } 
            {
                // Empty body.
            }

            const T& operator*(void) const
            {
                return _buffer->at(_index);
            }

            ConstRingBufferIterator& operator++(void)
            {
                _index++;
                return *this;
            }

            bool operator!=(const ConstRingBufferIterator& other) const
            {
                return _index != other._index;
            }

        private:
            const FixedRingBuffer* _buffer;
            size_t _index;
        };

        RingBufferIterator begin()
        {
            return { this, 0 };
        }

        RingBufferIterator end()
        {
            return { this, size() };
        }

        ConstRingBufferIterator begin() const 
        { 
            return { this, 0 }; 
        }

        ConstRingBufferIterator end() const 
        { 
            return { this, size() }; 
        }
        
        ConstRingBufferIterator cbegin() const 
        { 
            return begin(); 
        }

        ConstRingBufferIterator cend() const 
        { 
            return end(); 
        }

    private:
        size_t next(size_t index) const
        {
            return (index + 1) % _capacity;
        }

        size_t prev(size_t index) const
        {
            return (index + _capacity - 1) % _capacity;
        }

        size_t physical_index(size_t logical_index) const
        {
            return (_head + logical_index) % _capacity;
        }

        T* _data{ };
        size_t _capacity{ };
        size_t _size{ };
        size_t _head{ }; // oldest element
        size_t _tail{ }; // next write position
    };
}
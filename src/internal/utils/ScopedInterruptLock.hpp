/*
 ******************************************************************************
 *  ScopedInterruptLock.hpp
 *
 *  Automatic interrupt enabler / disabler for the Arduino environment.
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
 *    This internal class is meant to be used as a lock for interrupts with
 *    LinearCollection methods.
 *    
 *    CAUTION: this file is an internal header and not part of the public API.
 * 
 ******************************************************************************
 */
#pragma once
#include <Arduino.h>

namespace DuinoCollections
{
    namespace Utils
    {
        /**
         * @brief RAII helper that disables interrupts on construction
         *        and restores the previous interrupt state on destruction.
         *
         * This class is portable across Arduino-compatible platforms
         * (AVR, ESP32, RP2040, etc.) because it relies on the core
         * noInterrupts() / interrupts() API.
         *
         * The previous interrupt state is preserved so nested usage is safe.
         *
         * @warning Must not be used inside an ISR.
         */
        class ScopedInterruptLock final
        {
        public:
            /**
             * Initializes this ScopetInterruptLock and deactivates
             * interrupts for its lifetime.
             */
            ScopedInterruptLock()
            {
                _were_enabled = interruptsEnabled();
                noInterrupts();
            }

            ~ScopedInterruptLock()
            {
                if (_were_enabled)
                {
                    interrupts();
                }
            }

            // Forbid copy.
            ScopedInterruptLock(const ScopedInterruptLock&) = delete;
            ScopedInterruptLock& operator=(const ScopedInterruptLock&) = delete;

            // Forbid dynamic allocation
            void* operator new(size_t) = delete;
            void* operator new[](size_t) = delete;
            void operator delete(void*) = delete;
            void operator delete[](void*) = delete;

        private:
            bool _were_enabled;

            /**
             * Implementation is platform-dependent.
             * @return true if interrupts are enabled, false otherwise.
             */
            static bool interruptsEnabled()
            {
            #if defined(ARDUINO_ARCH_AVR)
                return bitRead(SREG, 7);

            #elif defined(ARDUINO_ARCH_ESP32)
                return true; // ESP32 Arduino core does not expose a direct API

            #elif defined(ARDUINO_ARCH_RP2040)
                return true; // same limitation

            #else
                return true; // fallback
            #endif
            }
        };
    }
}


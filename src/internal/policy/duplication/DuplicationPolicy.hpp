/*
 ******************************************************************************
 *  DuplicationPolicy.hpp
 *
 *  Determines whether or not a collection should allow data duplicates.
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
 *    Enumerated type to be used as template parameter by LinearCollection
 *    derived types.
 *    CAUTION: this file is an internal header and not part of the public API.
 * 
 ******************************************************************************
 */
#pragma once
#include <stdint.h>

namespace DuinoCollections
{
    namespace Internal
    {
        namespace Policy
        {
            namespace Duplication
            {
                /**
                 * Defines data duplicates policy in collections.
                 * Duplicates should either be allowed or forbidden.
                 */
                enum class DuplicationPolicy : uint8_t
                {
                    ALLOW_DUPLICATES,
                    FORBID_DUPLICATES
                };
            }
        }
    }
}
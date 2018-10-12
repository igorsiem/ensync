/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <string>
#include <variant>

#include "atomic_type.h"

#ifndef _ensync_atomic_value_h_included
#define _ensync_atomic_value_h_included

namespace ensync {

/**
 * \brief A atomic type for enSync values
 * 
 * Objects of this type represent any value that an enSync attribute can hold
 * at any time.
 * 
 * The types that this variant may hold are enumerated by
 * \ref `atomic_type_id_t`. That enumeration must be updated if this type
 * is changed.
 */
using atomic_value = std::variant<int, std::wstring, double>;

/**
 * \brief Determine the current type of a value object
 * 
 * \param v The value to check
 * 
 * \return The type enumerator
 */
extern atomic_type_id type_of(const atomic_value& v);

}   // end ensync namespace

#endif

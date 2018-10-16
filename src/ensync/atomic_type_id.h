/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <cstddef>
#include <string>
#include "pointer_helpers.h"

#ifndef _ensync_atomic_type_id_h_included
#define _ensync_atomic_type_id_h_included

namespace ensync {

/**
 * \brief Enumerate the types that an \ref ensync::atomic_value object can be
 * 
 * Note that this must be updated if `atomic_value` is updated.
 */
enum class atomic_type_id : std::size_t
{
    int_avt = 0                     ///< integer
    , wstring_avt = 1               ///< (wide) string
    , double_avt = 2                ///< double-precision float
};

/**
 * \brief Retrieve a human-readable string name for the given atomic type,
 * taken from the string table
 * 
 * \param id The atomic type to convert
 * 
 * \return The human-readable name of the type
 */
extern std::wstring to_wstring(atomic_type_id id);

}   // end ensync namespace

#endif

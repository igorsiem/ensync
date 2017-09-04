/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "messages.h"

#ifndef enSync_data_type_h_included
#define enSync_data_type_h_included

namespace sync {
/**
 * \brief Enumerate the atomic data types support by *enSync*
 */
enum class data_type
{
    unknown = 0,    ///< Not (yet) determined
    integer,        ///< Integer, any size
    decimal,        ///< Float, double, any size or precision
    string          ///< Wide-string
};  // end type enum

/**
 * \brief Translate a data type enumerator into a message enumerator for
 * a human-readable description of the type
 */
extern message_code to_message_code(data_type dt);

}   // end sync namespace

#endif

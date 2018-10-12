/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "atomic_value.h"

namespace ensync {

atomic_type_id type_of(const atomic_value& v)
{
    return static_cast<atomic_type_id>(v.index());    
}   // end type_of method

}   // end ensync namespace

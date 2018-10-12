/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "atomic_type.h"
#include "string_table.h"

namespace ensync {

std::wstring to_wstring(atomic_type_id id)
{
    switch (id)
    {
        case atomic_type_id::int_avt:
            return get(message_code_t::integer_type);

        case atomic_type_id::wstring_avt:
            return get(message_code_t::string_type);

        case atomic_type_id::double_avt:
            return get(message_code_t::double_type);

        default:
            return get(message_code_t::unknown_type);
    }
}   // end to_wstring

}   // end ensync

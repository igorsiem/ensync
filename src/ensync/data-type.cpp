/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

 #include "data-type.h"

namespace sync {

message_code to_message_code(data_type dt)
{
    switch (dt)
    {
        // If we have an unrecognised data type, don't throw an
        // exception - just call it 'unknown'
        default:
        case data_type::unknown:
            return message_code::data_type_unknown;
        case data_type::integer: return message_code::data_type_integer;
        case data_type::decimal: return message_code::data_type_decimal;
        case data_type::string: return message_code::data_type_string;
    }
}   // end message_code method

}   // end sync namespace

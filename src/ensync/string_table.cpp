/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "string_table.h"

namespace ensync {

/**
 * \brief The message table used by enSync
 */
static const wchar_t* error_messages[] = {

    L""                                 // 0 - none
    , L"unknown message"                // 1 - unknown

    // --- Error Messages ---

    , L"unknown error"                  // 2 - unknown_err
    , L"runtime assertion failure: {} - {}:{}"  // 3 - runtime_assertion_err
    , L"duplicate compound type name"   // 4 - compound type name exists

    // --- Type Identifiers ---

    , L"integer"                        // 5 - integer_type
    , L"string"                         // 6 - string_type
    , L"floating-point"                 // 7 - double_type
    , L"unknown type"                   // 8 - unknown type
};

std::wstring get(message_code_t mc)
{
    return error_messages[static_cast<int>(mc)];
}   // end get method

}   // end ensync namespace

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

    // --- Type Identifiers ---

    , L"integer"                        // 4 - integer_type
    , L"string"                         // 5 - string_type
    , L"floating-point"                 // 6 - double_type
    , L"unknown"                        // 7 - unknown type
};

std::wstring get(message_code_t mc)
{
    return error_messages[static_cast<int>(mc)];
}   // end get method

}   // end ensync namespace

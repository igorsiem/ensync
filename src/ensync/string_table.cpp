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
    , L"unknown error"                  // 2 - unknown_error
    , L"runtime assertion failure: {}"  // 3 - assertion_failure
};

std::wstring get(message_code_t mc)
{
    return error_messages[static_cast<int>(mc)];
}   // end get method

}   // end ensync namespace

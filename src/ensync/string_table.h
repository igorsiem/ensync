/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <string>

#ifndef _ensync_string_table_h_included
#define _ensync_string_table_h_included

namespace ensync {

/**
 * \brief Enumerate all the message codes used by enSync
 * 
 * This enumerator is part of a simply string-table implementation used for
 * human-readable strings in enSync. It used for any message that the User
 * might read, apart from log messages.
 */
enum class message_code_t
{

    none = 0            ///< No message
    , unknown = 1       ///< Unknown / unrecognised message

    // --- Error Messages ---

    , unknown_err = 2       ///< Unknown error condition

    /**
     * \brief A runtime assertion has failed
     * 
     * This string has three placeholders for details of the failed
     * condition, the file and the line number.
     */
    , runtime_assertion_err = 3

    // --- Type Identifiers ---

    , integer_type = 4
    , string_type = 5
    , double_type = 6
    , unknown_type = 7

};  // end message_code_t enum

/**
 * \brief Retrieve the human-readable message indicated by the given code
 * 
 * \param mc The message code
 */
extern std::wstring get(message_code_t mc);

}   // end ensync namespace

#endif

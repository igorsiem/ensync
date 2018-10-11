/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "copy_move_helpers.h"
#include "string_table.h"
#include "strutils.h"

#ifndef _ensync_error_h_included
#define _ensync_error_h_included

namespace ensync {

/**
 * \brief Base class for all error exceptions that are specific to enSync
 * 
 * This is an abstract class. Its primary purpose is to declare the `message`
 * method, which retrieves a human-readable description of the problem.
 */
class error
{
    public:

    /**
     * \brief Trivial virtual destructor
     */
    virtual ~error(void) noexcept {}

    // Standard copy / move semantics
    EN_DECLARE_DEFAULT_CONSTRUCTOR(error)
    EN_DECLARE_DEFAULT_COPY(error)
    EN_DECLARE_DEFAULT_MOVE(error)

    /**
     * \brief Retrieve a human-readable description of the error condition
     * as a wide string
     * 
     * \return A human-readable description of the error as a wide-string
     */
    virtual std::wstring message(void) const noexcept = 0;

};  // end error class

/**
 * \brief A template for error exceptions that have a single message (from
 * the message table)
 * 
 * This allows efficient declaration of error classes based purely on message
 * code, but is not suitable for error conditions where more than a standard
 * message is necessary (e.g. a file-open error that also needs to carry a
 * file name).
 * 
 * See the declaration for \ref ensync::unknown_error for an example of how
 * single message error classes are declared.
 * 
 * \tparam mc The message code
 */
template <message_code_t mc>
class single_message_error : public error
{

    public:

    EN_DECLARE_DEFAULT_ALL(single_message_error)

    /**
     * \brief Retrieve a human-readable description of the error condition
     * as a wide string
     * 
     * \return A human-readable description of the error as a wide-string
     */
    virtual std::wstring message(void) const noexcept
        { return get(mc); }

};  // end single_message_error

/**
 * \brief Signals an unknown or unrecognised error condition within enSync
 */
using unknown_error = single_message_error<message_code_t::unknown_err>;

}   // end ensync namespace

#endif

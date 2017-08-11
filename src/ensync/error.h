/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <memory>
#include <locale>
#include "messages.h"
#include "logger.h"
#include "strutils.h"

/**
 * \brief Whether or not to log exceptions when they are thrown
 */
#define ENSYNC_LOG_EXCEPTIONS

#ifndef enSync_error_h_included
#define enSync_error_h_included

namespace sync {

/**
 * \brief A base-class for all exceptions in *enSync*
 *
 * Exception classes copyable, cloneable (using shared_ptr), and thread-safe.
 *
 * See the \ref errors page for more information on error-handling.
 */
class error
{
    // --- Exernal Interface ---

    public:

    // -- Public Sub-types --

    /**
     * \brief A shared pointer to an error object
     */
    using error_ptr = std::shared_ptr<error>;

    // -- Methods --

    /**
     * \brief Initialise the error with its message code
     *
     * \param mc The message code to use     
     */
    error(message_code mc) : m_msg_code(mc) {}

    /**
     * \brief Retrieve the message code
     */
    virtual message_code msg_code(void) const { return m_msg_code; }

    /**
     * \brief Retrieve a human-readable message for error condition
     */
    const std::wstring& msg(void) const;

    /**
     * \brief Clone the error object as a shared pointer
     *
     * \return A shared_pointer to a clone of self
     */
    virtual error_ptr clone(void) const;

    // --- Internal Declarations ---

    protected:

    /**
     * \brief The message code from which the error message is generated.
     */
    message_code m_msg_code;

};  // end error class

/**
 * \brief A shared pointer to an error object
 */
using error_ptr = error::error_ptr;

}   // end sync namespace

// --- Helper Macros ---

/**
 * \brief A wide-string version of the __FILE__ macro
 *
 * \todo Note that this macro uses the ::sync::strutils::to_wstring method,
 * which creates a temporary along the way, so it's not very efficient. A
 * better way is probably to do something with the "string-ize" macro
 * operator...
 */
#define ENSYNC_WFILE ::sync::strutils::to_wstring(__FILE__)

#ifdef ENSYNC_LOG_EXCEPTIONS

    #define ENSYNC_RAISE_ERROR( msg_code ) do { \
        ENSYNC_LOG( sync::logger::ch_error, "throwing ::sync::error " \
            "exception with message code " << to_int(msg_code) << " (\"" << \
            ::sync::message(msg_code) << "\") at \"" << ENSYNC_WFILE << \
            "\":" << __LINE__ ); \
        throw ::sync::error(msg_code); \
    } while (false)

#else

    #define ENSYNC_RAISE_ERROR( msg_code ) do { \
        throw ::sync::error(msg_code); \
    } while (false)

#endif

/**
 * \page errors Error Handling in enSync
 *
 * Signalling error conditions in enSync is done using exceptions that
 * carry a `message` method for a human-readable description of the problem
 * (generated internally from the message table).
 *
 * Exceptions classes are non-copyable, but are cloneable (for passing across
 * thread-boundaries) and thread-safe.
 */

#endif

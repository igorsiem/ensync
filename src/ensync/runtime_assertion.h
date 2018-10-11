/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <fmt/format.h>
#include "error.h"
#include "logger.h"
#include "strutils.h"

#ifndef _ensync_runtime_assertion_h_included
#define _ensync_runtime_assertion_h_included

namespace ensync {

/**
 * \brief An exception class to signal runtime assertion failures
 */
class runtime_assertion_error : public error
{

    public:

    /**
     * \brief Constructor - sets up the error object with its message
     * 
     * \param fail_msg The reason for the failure; this may simply be the
     * code that evaluated to `false` in the `ENSYNC_ASSERT` macro
     * 
     * \param file_name The file name at which the error occurred
     * 
     * \param line_number The line number at which the error occurred
     */
    runtime_assertion_error(
            std::string fail_msg
            , std::string file_name
            , int line_number) :
        error()
        , m_message(
            fmt::format(
                get(message_code_t::runtime_assertion_err)
                , boost::lexical_cast<std::wstring>(fail_msg)
                , boost::lexical_cast<std::wstring>(file_name)
                , line_number))
    {        
    }   // end constructor

    /**
     * \brief Retrieve a human-readable description of the error condition
     * as a wide string
     * 
     * \return A human-readable description of the error as a wide-string
     */
    virtual std::wstring message(void) const noexcept
        { return m_message; }

    protected:

    /**
     * \brief The fully assembled, human-readable error message
     * 
     * This is created on construction.
     */
    std::wstring m_message;

};  // end runtime_assertion_error

/**
 * \brief Runtime assertion macro
 * 
 * If the condition fails, an ::ensync::runtime_assertion_error exception is
 * thrown, and a debug message is logged.
 * 
 * \param condition The condition to check; must evaluate to a boolean
 */
#define ENSYNC_ASSERT( condition ) do { \
    if (!(condition)) \
    { \
        ::ensync::runtime_assertion_error e( \
            #condition \
            , __FILE__ \
            , __LINE__ ); \
        ::ensync::logger::instance().log( \
            ensync::logger::level_t::debug, e.message()); \
        throw e; \
    } \
} while (false)

}   // end ensync namespace

#endif

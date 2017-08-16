/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <string>
#include <sqlite/sqlite3.h>
#include "error.h"
#include "messages.h"

#ifndef ensync_sqlite_wrapper_h_included
#define ensync_sqlite_wrapper_h_included
 
namespace sync {

/**
 * \brief A wrapper for the SQLite 3 C API
 */
namespace sqlite {

/**
 * \brief Convert an integer SQL result code to a message code
 *
 * Note that an invalid SQLite code will return `unrecognised_sqlite_result`,
 * rather than throwing an exception, so this function is safe for exception-
 * handling code.
 */
extern sync::message_code to_message_code(int sql_result_code);

/**
 * \brief An exception class for signalling errors from the SQLite library
 *
 * This class identifies errors from the SQLite result code, and translates
 * these into human-readable messages from the message table.
 */
class error : public ::sync::error
{
    // --- Public Interface ---

public:

    // -- Public Sub-types --

    /**
     * \brief A shared pointer to a sqlite error
     */
    using error_ptr = std::shared_ptr<error>;

    // -- Methods --

    /**
     * Constructor - accepts the SQLite error code and, optionally, the
     * SQLite database file path
     *
     * \param result The SQLite error code; as an integer, this could
     * potentially be any value, and no check is made to ensure that a
     * legitimate SQLite error code is passed; in such a case, the message
     * would indicate a invalid SQLite result code; note that this result
     * is translated into an *enSync* message code using the
     * `to_messsage_code` function
     *
     * \param file_name This will be attached to the message if it is not
     * blank
     */
    explicit error(int result, const std::string& file_name = "");

    /**
     * \brief Retrieve a human-readable message for error condition
     */
    virtual std::wstring msg(void) const;

    /**
     * \brief Clone the error object as a shared pointer
     *
     * \return A shared_pointer to a clone of self; note that this is
     */
    virtual ::sync::error_ptr clone(void) const
        { return std::make_shared<sqlite::error>(*this); }

    /**
     * \brief Retrieve the SQLite result code
     */
    int result(void) const { return m_result; }

    /**
     * \brief Retrieve the file name
     */
    const std::string& file_name(void) const { return m_file_name; }

    // --- Internal Declarations ---

protected:

    /**
     * \brief SQLite result code
     */
    int m_result;

    /**
     * \brief File name of the SQLite DB (may be unavailable)
     */
    std::string m_file_name;

};  // end error class

#ifdef ENSYNC_INCLUDE_FILE_AND_LINE_IN_ERROR_LOG

    #define ENSYNC_RAISE_SQLITE_ERROR( result_code, file_name ) do { \
        ::sync::sqlite::error e(result_code, file_name); \
        ENSYNC_LOG(::sync::logger::ch_error, \
            ::sync::message(::sync::message_code::fragment_error) << \
            L" - " << e.msg() << L" - \"" << \
            ENSYNC_WFILE << L"\":" << __LINE__ ); \
        throw e; \
    } while (false)

#else

    /**
     * \brief Raise a SQLite error exception, passing the integer result
     * code and database filename (possibly empty).
     */
    #define ENSYNC_RAISE_SQLITE_ERROR( result_code, file_name ) do { \
        ::sync::sqlite::error e(result_code, file_name); \
        ENSYNC_LOG(::sync::logger::ch_error, \
            ::sync::message(::sync::message_code::fragment_error) << \
            L" - " << e.msg(); \
        throw e; \
    } while (false)

#endif

/**
 * \brief A shared pointer to a sqlite error
 */
using error_ptr = error::error_ptr;

}}  // end sync::sqlite namespace

#endif

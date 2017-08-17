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
 *
 * See the \ref sqlitewraper page for more information.
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
class liberror : public ::sync::error
{
    // --- Public Interface ---

public:

    // -- Public Sub-types --

    /**
     * \brief A shared pointer to a sqlite liberror
     */
    using liberror_ptr = std::shared_ptr<liberror>;

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
    explicit liberror(int result, const std::string& file_name = "");

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
        { return std::make_shared<liberror>(*this); }

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

};  // end liberror class

/**
 * \brief A shared pointer to a sqlite error
 */
using liberror_ptr = liberror::liberror_ptr;

#ifdef ENSYNC_INCLUDE_FILE_AND_LINE_IN_ERROR_LOG

    /**
     * \brief Raise a SQLite liberror exception, passing the integer result
     * code and database filename (possibly empty), along with the file and
     * line number.
     */
    #define ENSYNC_RAISE_SQLITE_LIBERROR( result_code, file_name ) do { \
        ::sync::sqlite::liberror e(result_code, file_name); \
        ENSYNC_LOG(::sync::logger::ch_error, \
            ::sync::message(::sync::message_code::fragment_error) << \
            L" - " << e.msg() << L" - \"" << \
            ENSYNC_WFILE << L"\":" << __LINE__ ); \
        throw e; \
    } while (false)

#else

    /**
     * \brief Raise a SQLite liberror exception, passing the integer result
     * code and database filename (possibly empty).
     */
    #define ENSYNC_RAISE_SQLITE_LIBERROR( result_code, file_name ) do { \
        ::sync::sqlite::liberror e(result_code, file_name); \
        ENSYNC_LOG(::sync::logger::ch_error, \
            ::sync::message(::sync::message_code::fragment_error) << \
            L" - " << e.msg(); \
        throw e; \
    } while (false)

#endif

/**
 * \brief A little used error exception class for signalling problems that
 * may occur in the SQLite wrapper functionality (rather than the SQLite
 * library itself).
 *
 * This method adds no functioanality to the base class, and exists purely
 * for the purpose of separation.
 */
class wrappererror : public ::sync::error
{

public:

    /**
     * \brief A shared pointer to a wrappererror object
     */
    using wrappererror_ptr = std::shared_ptr<wrappererror>;

    /**
     * \brief Constructor - takes a message code
     */
    wrappererror(message_code mc) : sync::error(mc) {}

    /**
     * \brief Clone the error object as a shared pointer
     *
     * \return A shared_pointer to a clone of self
     */
    virtual error_ptr clone(void) const
    {
        return std::make_shared<wrappererror>(*this);
    }
    
};  // end wrappererror

/**
 * \brief A shared pointer to a wrapper error
 */
using wrappererror_ptr = wrappererror::wrappererror_ptr;

#ifdef ENSYNC_INCLUDE_FILE_AND_LINE_IN_ERROR_LOG

    /**
     * \brief Raise an *enSync* generic error, logging its information in the
     * error log, and appending the source code file name and line number.
     */
    #define ENSYNC_RAISE_SQLITE_WRAPPERERROR( msg_code ) do { \
        ::sync::sqlite::wrappererror e(msg_code); \
        ENSYNC_LOG(::sync::logger::ch_error, \
            ::sync::message(::sync::message_code::fragment_error) << \
            L" - " << e.msg() << L" - \"" << \
            ENSYNC_WFILE << L"\":" << __LINE__ ); \
        throw e; \
    } while (false)
     
#else

    /**
     * \brief Raise an *enSync* generic error, adding an entry to the error
     * log.
     */
    #define ENSYNC_RAISE_SQLITE_WRAPPERERROR( msg_code ) do { \
        ::sync::sqlite::wrappererror e(msg_code); \
        ENSYNC_LOG(::sync::logger::ch_error, \
            ::sync::message(::sync::message_code::fragment_error) << \
            L" - " << e.msg(); \
        throw e; \
    } while (false)

#endif

// Forward declaration of the database class
class database;

/**
 * \brief A shared pointer to a database object
 */
using database_ptr = std::shared_ptr<database>;

/**
 * \brief A managed connection to a SQLite databade
 *
 * The `database` class manages a SQLite database connection handle (an
 * internal pointer to a `sqlite3` struct). Objects of this class are created
 * using the `create_new` static method, which opens the database file, and
 * reeturns the object as a shared pointer (`db_ptr`). The destructor closes
 * the database connection (using `sqlite3_close`).
 *
 * This class generates preapred statements (also as shared pointers). An
 * important aspect of the life-cycle of this class is that statements
 * retain a shared pointer to the `database` object that created them, so
 * the `databadr` object will not be deleted (and its connection closed)
 * until the last statement object is destroyed.
 *
 * In other respects, this is a normal C++ class. It has no copyy semantics.
 *
 * \todo Usage examples
 */
class database final : public std::enable_shared_from_this<database>
{
    // --- External Interface ---

public:

    // -- Public Sub-types --

    // -- Methods --

    /**
     * \brief Destructor - shuts down the database connection (using
     * `sqlite3_close`)
     *
     * Note that if the shutdown returns an error (e.g. because not all
     * statements have been finalised) an exception is *not* thrown (because
     * this is a destructor), but an error is logged.
     */
    ~database(void);

    /**
     * \brief Create / open a new database object, connecting to the given
     * database file
     *
     * This method opens the given SQLite database file. If this is
     * successful, a new `database` object is created for it (on the heap),
     * and returned as a shared pointer. An exception is thrown if there is
     * a problem.
     * 
     * \param file_name The name of the MySQL file - this is created if it
     * does not already exist
     *
     * \return A shared pointer to the new database object
     *
     * \throws ::sync::sqlite::error There was a problem opening the database
     *
     * \todo Support for different opening modes
     */
    static database_ptr create_new(const std::string& file_name);

    // --- Internal Declarations ---

private:

    /**
     * \brief Constructor - initialises attributes
     *
     * Note that the database is not actually opened by the constructor. This
     * is done in the `create_new` method, and the parameters are passed in
     * for initialisation.
     *
     * \param connection The fully created sqlite3 connection
     *
     * \param file_name The name of the database file
     */
    database(sqlite3* connection, const std::string& file_name);

    /**
     * \brief The internal SQLite3 object
     *
     * This attribute is initialised by the constructor, (passed from the
     * `create_new` method) and shut down (via `sqlite_close`) in the
     * destructor.
     */
    sqlite3* m_connection;

    /**
     * \brief The file name of the database
     *
     * This should always be a valid path.
     */
    std::string m_file_name;

};  // end database class

/**
 * \page sqlitewrapper SQLite Wrapper
 *
 * *enSync* includes a very 'thin' C++ wrapper for the SQLite 3 library. This
 * provides little more than C++ classes and objects for managing the SQLite
 * connection, statement and error structures.
 *
 * \todo Expand on this
 *
 * \todo Provide usage examples
 */

}}  // end sync::sqlite namespace

#endif

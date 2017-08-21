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
 * \brief A based class for errors related to SQLite and its wrapper
 *
 * This class adds file name and SQL statement string attributes to the base
 * error class definition, but does makes the `msg` and `clone` methods pure
 * virtual, so it cannot be instantiated or thrown directly. It is intended
 * purely as a base class for other error classes.
 */
class error  : public ::sync::error
{
    // --- Public Interface ---

public:

    /**
     * \brief Construcotr, initialising all attributes
     *
     * \param c The message code for the error text
     *
     * \param db_file_name The file name of the SQLite database (may be
     * empty)
     *
     * \param sql The SQL statement being executed (may be empty)
     */
    error(
            message_code c,
            const std::string& fn,
            const std::string& s) :
        ::sync::error(c),
        m_db_file_name(fn),
        m_sql(s)
    {        
    }   // end constructor

    /**
     * \brief Retrieve a human-readable message for error condition
     */
    virtual std::wstring msg(void) const = 0;
    
    /**
     * \brief Clone the error object as a shared pointer
     *
     * \return A shared_pointer to a clone of self
     */
    virtual ::sync::error_ptr clone(void) const = 0;

    /**
     * \brief Retrieve the DB file name (may be blank)
     */
    const std::string& db_file_name(void) const { return m_db_file_name; }

    /**
     * \brief Retrieve the SQL statement (may be blank)
     */
    const std::string& sql(void) const { return m_sql; }

    // --- Internal Declarations ---

protected:

    /**
     * \brief The name of the database file (may be blank)
     */
    std::string m_db_file_name;

    /**
     * \brief The SQL statement being executed when the error occured (if
     * any)
     */
    std::string m_sql;

};  // end error class

/**
 * \brief An exception class for signalling errors from the SQLite library
 *
 * This class identifies errors from the SQLite result code, and translates
 * these into human-readable messages from the message table.
 */
class liberror : public sqlite::error
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
     * \param db_file_name This will be attached to the message if it is not
     * blank
     *
     * \param sql The SQL statement that was being executed when the error
     * occurred (may be blank); this will also be appended to the message if
     * it is not blank
     */
    explicit liberror(
        int result,
        const std::string& db_file_name = "",
        const std::string& sql = "");

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

    // --- Internal Declarations ---

protected:

    /**
     * \brief SQLite result code
     */
    int m_result;

};  // end liberror class

/**
 * \brief A shared pointer to a sqlite error
 */
using liberror_ptr = liberror::liberror_ptr;

#ifdef ENSYNC_INCLUDE_FILE_AND_LINE_IN_ERROR_LOG

    /**
     * \brief Raise a SQLite liberror exception, passing the integer result
     * code, database filename, and SQL statement, along with the source file
     * and line number.
     */
    #define ENSYNC_RAISE_SQLITE_LIBERROR( result_code, db_file_name, sql ) \
    do { \
        ::sync::sqlite::liberror e(result_code, db_file_name, sql); \
        ENSYNC_LOG(::sync::logger::ch_error, \
            e.msg() << L" - \"" << ENSYNC_WFILE << L"\":" << __LINE__ ); \
        throw e; \
    } while (false)

#else

    /**
     * \brief Raise a SQLite liberror exception, passing the integer result
     * code, database filename and SQL statement.
     */
    #define ENSYNC_RAISE_SQLITE_LIBERROR( result_code, db_file_name, sql ) \
    do { \
        ::sync::sqlite::liberror e(result_code, db_file_name, sql); \
        ENSYNC_LOG(::sync::logger::ch_error, e.msg(); \
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
class wrappererror : public sqlite::error
{

public:

    /**
     * \brief A shared pointer to a wrappererror object
     */
    using wrappererror_ptr = std::shared_ptr<wrappererror>;

    /**
     * \brief Constructor - initialises all internal attributes
     */
    wrappererror(
            message_code mc,
            const std::string& dbfn,
            const std::string& s) : sqlite::error(mc, dbfn, s) {}

    /**
     * \brief Retrieve a human-readable message for error condition
     */
    virtual std::wstring msg(void) const;            

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
     * \brief Raise a SQLite Wrapper error, logging its information in the
     * error log, and appending the source code file name and line number.
     */
    #define ENSYNC_RAISE_SQLITE_WRAPPERERROR( msg_code, dbfn, sql ) do { \
        ::sync::sqlite::wrappererror e(msg_code, dbfn, sql); \
        ENSYNC_LOG(::sync::logger::ch_error, \
            e.msg() << L" - \"" << ENSYNC_WFILE << L"\":" << __LINE__ ); \
        throw e; \
    } while (false)
     
#else

    /**
     * \brief Raise an *enSync* generic error, adding an entry to the error
     * log.
     */
    #define ENSYNC_RAISE_SQLITE_WRAPPERERROR( msg_code, dbfn, sql ) do { \
        ::sync::sqlite::wrappererror e(msg_code, dbfn, sql); \
        ENSYNC_LOG(::sync::logger::ch_error, e.msg(); \
        throw e; \
    } while (false)

#endif

/**
 * \brief A managed connection to a SQLite databade
 *
 * The `database` class manages a SQLite database connection handle (an
 * internal pointer to a `sqlite3` struct). Objects of this class are created
 * using the `create_new` static method, which opens the database file, and
 * reeturns the object as a shared pointer (`db_ptr`). The destructor closes
 * the database connection (using `sqlite3_close`).
 *
 * This class includes a `statement` subclass, which encapsulates SQLite
 * compiled statement structures that are ready for execution. These are
 * also generated as shared pointers, and it is an important aspect of the
 * life-cycle of the `database` and `statement` classes that statements
 * retain a shared pointer to the `database` object that created them, so
 * the `database` object will not be deleted (and its connection closed)
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

    /**
     * \brief A shared pointer to a database object
     */
    using database_ptr = std::shared_ptr<database>;

    /**
     * \brief A 'prepared' (i.e. compiled and ready to execute) SQLite
     * statement
     *
     * As with the `database` class, `statement` has no copy semantics.
     *
     * \todo Expand on this documentation
     *
     * \todo Parameter binding is not (yet) supported. This may not be a very
     * high priority as we will be forming queries in very specify
     * circumstances in *enSync*.
     *
     * \todo Usage examples
     */
    class statement final : public std::enable_shared_from_this<statement> 
    {
        // Database can access non-public items in statement
        friend class database;

        // --- External Interface ---

    public:

        // -- Public Sub-types --

        /**
         * \brief A shared pointer to a statement object
         */
        using statement_ptr = std::shared_ptr<statement>;

        /**
         * \brief Denotes the possible states of the statement
         */
        enum class state
        {
            prepared,       ///< Statement has been prepared but not executed

            /**
             * \brief Statement has executed with no error (see result)
             */
            success,

            error           ///< Statement has executed with an error
        };  // end class state

        /**
         * \brief Enumerates the possible non-error results of execution a
         * statement step (i.e. running the `step` method)
         */
        enum class step_result
        {
            none,               ///< Statement not yet executed
            busy = SQLITE_BUSY, ///< Database currently busy
            row = SQLITE_ROW,   ///< Execution successful, row data available
            done = SQLITE_DONE  ///< Execution complete, no (more) rows
        };  // end class result

        // -- Methods --

        /**
         * \brief Destructor - finalises the SQLite statement
         *
         * Note that finalisation might return an error code, if statement
         * execution has resulted in an error. This is logged, but an
         * exception is *not* thrown, mainly because this is a destructor,
         * but also because the error will already have been signalled in
         * any case.
         *
         * The finalisation itself is also logged in the debug stream.
         */
        ~statement(void);

        /**
         * \brief Execute the next 'step' of a SQLite statement
         *
         * Note that if the database is 'busy' (usually because some other
         * thread or process write-locking the file), this is *not* signalled
         * as an error (because the system can still presumably perform its
         * function), but of course, the statement has not actually been
         * executed.
         *
         * Nornal step execution with its result is logged in the debugging
         * stream. Errors are logged in the error stream prior to being
         * signalled with an exception.
         *
         * \return The non-error result of the operation (including the
         * 'busy' response)
         *
         * \throws liberror Excuting the statement resulted in an error
         * result from the SQLite library
         */
        step_result step(void);

        /**
         * \brief Retrieve the current state of the statement
         */
        state current_state(void) const { return m_current_state; }

        /**
         * \brief Retrieve the most recent step execution result
         */
        step_result last_step_result(void) const
            { return m_last_step_result; }

        // --- Internal Declarations ---

    private:

        /**
         * \brief Create a new prepared statement
         *
         * This method is private, because we only want the `database` class
         * to be able to create new statement objects. If successful, the
         * full text of the statement is logged to the debug stream.
         *
         * \param db The database object
         *
         * \param sql The SQL statement to be executed
         *
         * \return A shared pointer to the prepared statement object
         *
         * \throws liberror The sqlite3 library reported an error while
         * preparing the statement
         *
         * \throws wrappererror The SQLite wrapper library encountered a
         * problem
         */
        static statement_ptr create_new(
            database_ptr db,
            const std::string& sql);

        /**
         * \brief Initialise the attributes of the statement object
         *
         * \param stmt A pointer to the prepared SQLite statement structure
         *
         * \param db The database object
         */
        statement(
            sqlite3_stmt* stmt,
            database_ptr db,
            const std::string& sql);

        /**
         * \brief The raw SQLite prepared statement structure
         */
        sqlite3_stmt* m_stmt;

        /**
         * \brief A reference to the database object that created us
         */
        database_ptr m_db;

        /**
         * \brief The SQL string - this is only retained for logging purposes
         */
        std::string m_sql;

        /**
         * \brief The current state of the statement
         */
        state m_current_state;

        /**
         * \brief The most recent execution result
         */
        step_result m_last_step_result;

        // Disable copy semantics
        statement(const statement&) = delete;
        statement& operator=(const statement&) = delete;

    };  // end statement class

    /**
     * \brief A shared pointer to a statement
     */
    using statement_ptr = statement::statement_ptr;

    // Statement class can access the non-public members of the database
    // class
    friend class statement;

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
     * \param db_file_name The name of the MySQL file - this is created if
     * it does not already exist
     *
     * \return A shared pointer to the new database object
     *
     * \throws liberror The sqlite3 library reported an error opening the
     * database
     *
     * \throws wrappererror The wrapper library encountered a problem
     *
     * \todo Support for different opening modes
     */
    static database_ptr create_new(const std::string& db_file_name);

    /**
     * \brief Prepare a new SQL statement for execution
     *
     * The statement is not actually executed - it is 'compiled' into a
     * binary representation for execution, but does not 'touch' the
     * underlying database, and has no effect until the statement's `step`
     * method is called.
     *
     * \param sql The SQL clause to prepare; this may be comprised of
     * multiple semicolon-delimited statements
     *
     * \throws liberror There was a SQLite library problem preparing the
     * statement; this will be thrown if there is a basic syntax error in the
     * SQL statement
     *
     * \throws wrappererror A problem occurred in the wrapper error
     */
    statement_ptr prepare(const std::string& sql);

    /**
     * \brief Retrieve the database file name
     */
    const std::string& file_name(void) const { return m_file_name; }

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

    // Remove copy semantics
    database(const database&) = delete;
    database& operator=(const database&) = delete;

};  // end database class

/**
 * \brief A shared pointer to a database
 */
using database_ptr = database::database_ptr;

/**
 * \page sqlitewrapper SQLite Wrapper
 *
 * *enSync* includes a very 'thin' C++ wrapper for the SQLite 3 library. This
 * provides little more than C++ classes and objects for managing the SQLite
 * connection, statement, row, value, and error structures.
 *
 * \todo Expand on this
 *
 * \todo Provide usage examples
 */

}}  // end sync::sqlite namespace

#endif

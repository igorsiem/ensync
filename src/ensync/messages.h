/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <string>
#include <vector>

#ifndef enSync_messages_h_included
#define enSync_messages_h_included

namespace sync {

/**
 * \brief A set of enumeration codes for the messages table
 *
 * The messages table and this enumeration *always* should be updated in
 * parallel.
 */
enum class message_code
{
    no_message = 0,     ///< No message (empty string)
    unspec_error = 1,   ///< A generic, unspecified error message

    // --- SQLite Messages ---

    // -- SQLite C Library --

    unrecognised_sqlite_result = 2, ///< Unrecognised SQLite result code

    // Note that these entries correspond to SQLite result codes, but their
    // numeric values are NOT the same. The `to_message_code` method should
    // be used to translate.
    //
    // The enumeration names intentionally correspond to the equivalent
    // SQLite constant definitions.
    sqlite_ok = 3,          ///< SQLite operation completed successfully
    sqlite_error = 4,       ///< Unspecified SQLite error
    sqlite_internal = 5,    ///< Internal SQLite library error
    sqlite_perm = 6,        ///< Access for created SQLite file not provided
    sqlite_abort = 7,       ///< SQLite operation aborted
    sqlite_busy = 8,        ///< SQLite database is currently busy
    sqlite_locked = 9,      ///< SQLite database is currently locked
    sqlite_nomem = 10,      ///< SQLite memory allocation failed
    sqlite_readonly = 11,   ///< SQLite database file cannot be written
    sqlite_interrupt = 12,  ///< Interrupted SQLite operation
    sqlite_ioerr = 13,      ///< IO Error during SQLite operation
    sqlite_corrupt = 14,    ///< SQLite DB file corrupted
    sqlite_notfound = 15,   ///< SQLite 'not found' error (see SQLite docs)
    sqlite_full = 16,       ///< Disk full during SQLite operation
    sqlite_cantopen = 17,   ///< SQLite can't open file
    sqlite_protocol = 18,   ///< Problem with SQLite file locking protocol
    sqlite_empty = 19,      ///< Unused SQLite error code
    sqlite_schema = 20,     ///< SQLite schema has changed
    sqlite_toobig = 21,     ///< SQLite string or BLOB too big
    sqlite_constraint = 22, ///< SQLite database constraint violation
    sqlite_mismatch = 23,   ///< SQLite data type mismatch
    sqlite_misuse = 24,     ///< SQLite is being misused by the application
    sqlite_nolfs = 25,      ///< No large file system support in SQLite
    sqlite_auth = 26,       ///< SQLite authorisation failure
    sqlite_format = 27,     ///< SQLite format error (not currently used)
    sqlite_range = 28,      ///< SQLite parameter our of range
    sqlite_notadb = 29,     ///< File is not a SQLite database
    sqlite_notice = 30,     ///< Notice of unusual SQLite operation (unused)
    sqlite_warning = 31,    ///< Unspecified SQLite warning (unused)
    sqlite_row = 32,        ///< A SQLite row result has been returned
    sqlite_done = 33,       ///< SQLite operation complete

    // -- SQLite Wrapper Messages --

    // These messages are associated with operations in the SQLite Wrapper

    // - Errors -

    /**
     * \brief This message is used in the (theoretical) case that a SQLite
     * function (e.g. `sqlite3_open`) does *not* return an error, but does
     * not return a valid pointer to an expected object either.
     */
    sqlitewrapper_nullobjecterror = 34,

    sqlitewrapper_dbcloserror = 35,     ///< Error closing SQLite database
    sqlitewrapper_stmtfnlerror = 36,    ///< Error finalising SQLite stmt

    /**
     * \brief A requested SQLite data conversion is not supported by the
     * wrapper library at this time, or the requested type would involve
     * SQLite type coercion, and this has been disallowed by the `strict`
     * flag argument of the `value_as` parameter
     */
    sqlitewrapper_typecvterror = 37,

    sqlitewrapper_colrangeerror = 38,   ///< SQLite column index out of range

    /**
     * \brief An attempt was made to retrieve SQLite data from a result that
     * is not a row
     */
    sqlitewrapper_norowerror = 39,

    // - Warnings -

    // Type coercion warning

    // - Status Information -

    sqlitewrapper_dbopened = 40,    ///< A SQLite database was opened
    sqlitewrapper_dbclosed = 41,    ///< A SQLite database was closed
    sqlitewrapper_stmtprp = 42,     ///< SQLite statement prepared
    sqlitewrapper_step = 43,        ///< SQLite statement step-executed

    /**
     * \brief Finalisation of a SQLite statement failed. Note that this
     * may occur if a previous operation on a prepared statement has already
     * returned an error (which would also have been logged)
     */
    sqlitewrapper_stmtfnl = 44,     ///< SQLite statement finalised

    // --- Generic Message Fragments ---

    // These are commonly used message components, such as labels for data
    // (e.g. file-name: <...>); these will typically be used to compose
    // longer messages
    fragment_error = 45,     ///< 'error' message fragment
    fragment_file_name = 46, ///< 'file name' message fragment
    fragment_result = 47     ///< 'result' message fragment
};  // end 

/**
 * \brief Retrieve the string message code associated with a given message
 *
 * \param mc The message code enumerator
 *
 * \return The message string 
 */
extern const std::wstring& message(message_code mc);

/**
 * \brief Transform the given message code into an integer
 */
extern int to_int(message_code mc);

}   // end sync namespace

#endif

/**
 * \page messages Message Strings in enSync
 *
 * *enSync* uses a global string table for all human-readable messages. These
 * are located in the `messages.h` and `messages.cpp` table, making it easy
 * to support multiple languages.
 *
 * Note that this enumeration includes entries for messages *all through* the
 * *enSync* library (including those related to third-party components, such
 * as SQLite). This is a conscious design decision so that all human-readable
 * text resources are gathered into one location for easy management and
 * translation. We acknowledge that it may be somewhat clumsy to use from
 * other perspectives, and may revisit this design decision later.
 */

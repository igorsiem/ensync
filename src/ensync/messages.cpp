/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "messages.h"

namespace sync {

/**
 * \brief Raw wide-string messages for use throughout enSync
 *
 * *These strings should be the only text that a User sees!* Don't update
 * the position of strings in this table without also updating the
 * `messages_code` enum.
 */
const std::vector<std::wstring> messages(
{
    L"",                        // 0 - No message
    L"unspecified error",       // 1 - unspecifed_error

    L"SQLite database library returned unrecognised result code",
                                // 2 - unrecognise SQL result code
    
    L"SQLite database library operation completed successfully",
                                // 3 - SQLite OK

    L"unspecified SQLite database error",
                                // 4 - SQLite Error

    L"internal error in SQLite database library",
                                // 5 - SQLite Internal

    L"requested access mode for new SQLite database could not be provided",
                                // 6 - SQLite Perm

    L"SQLite database operation was aborted prior to completion",
                                // 7 - SQLite abort

    L"SQLite database is busy (file could not be written)",
                                // 8 - SQLite database busy

    L"SQLite database is locked",
                                // 9 - SQLite locked

    L"SQLite memory allocation failure",
                                // 10 - SQLite memalloc failed

    L"SQLite database is read-only",
                                // 11 - SQLite database is read-only

    L"SQLite database operation was interrupted",
                                // 12 - Interrupted SQLite operation

    L"IO error during SQLite database operation",
                                // 13 - SQLite IO error

    L"SQLite database file is corrupted",
                                // 14 - SQLite DB file corrupt

    L"SQLite file control opcode or VFS system call method not found",
                                // 15 - SQLIte not found

    L"disk full during SQLite database operation",
                                // 16 - SQLite Disk full

    L"SQLite file could not be opened",
                                // 17 - SQLite can't open file

    L"problem with SQLite file locking protocol",
                                // 18 - SQLite protocol error

    L"Unused SQLite error code",
                                // 19 - SQLite empty

    L"SQLite database schema has changed",
                                // 20 - SQLite schema has changed

    L"SQLite string, statement, or BLOB too big",
                                // 21 - SQLite string or BLOB too big

    L"SQLite database constraint violation",
                                // 22 - SQLite database constraint

    L"SQLITE data type mismatch",
                                // 23 - SQLite data type mismatch

    L"application is misusing SQLite library",
                                // 24 - SQLite misuse

    L"no large file system support for SQLite",
                                // 25 - SQLite No LFS

    L"SQLite authorisation failure",
                                // 26 - SQLite authorisation failure

    L"SQLite format error (unused)",
                                // 27 - SQLite format error (not used)

    L"SQLite parameter is out of range",
                                // 28 - SQLite parameter out of range

    L"file is not a SQLite database",
                                // 29 - SQLite file is not a DB

    L"unusual SQLite operation",
                                // 30 - SQLite notice of unusual operation

    L"unspecified SQLite warning",
                                // 31 - Unspecified SQLite warning

    L"a SQLite row has been returned",
                                // 32 - SQLite row available

    L"SQLite operation complete",
                                // 33 - A SQLite operation is complete

    L"SQLite null object",      // 34 - SQLite returned a NULL object

    L"error closing SQLite database",
                                // 35 - error closing SQLite database

    L"error finalising SQLite statement (a message related to this error "
        "may have already been logged)",
                                // 36 - problem finalising SQLite statement

    L"SQLite data type conversion error",
                                // 37 - SQLite data type conversion

    L"SQLite column index out of range",
                                // 38 - SQLite column number out of range

    L"attempt to retrieve SQLite data from a column that is not a row",
                                // 39 - retrieve SQL data from non-row

    L"SQLite database opened",  // 40 - a SQLite database was opened

    L"SQLite database closed",  // 41 - a SQLite database was closed

    L"SQLite statement prepared",
                                // 42 - A SQLite statement was prepared

    L"SQLite statement step execution",
                                // 43 - A SQLite statement was step-executed

    L"SQLite statement finalised",
                                // 44 - A SQLite statement was finalised

    L"error",                   // 45 - 'error' keyword fragment
    L"file name",               // 46 - 'file name' label fragment
    L"result"                   // 47 - 'result' label fragment

}); // end messages vector

const std::wstring& message(message_code mc)
{
    return messages[to_int(mc)];
}   // end message function

int to_int(message_code mc)
{
    return static_cast<std::size_t>(mc);
}   // end to_int

}   // end sync namespace

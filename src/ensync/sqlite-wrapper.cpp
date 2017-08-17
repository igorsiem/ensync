/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "sqlite-wrapper.h"

namespace sync { namespace sqlite {

sync::message_code to_message_code(int sql_result_code)
{
    switch (sql_result_code)
    {
        case SQLITE_OK:         return message_code::sqlite_ok;
        case SQLITE_ERROR:      return message_code::sqlite_error;
        case SQLITE_INTERNAL:   return message_code::sqlite_internal;
        case SQLITE_PERM:       return message_code::sqlite_perm;
        case SQLITE_ABORT:      return message_code::sqlite_abort;
        case SQLITE_BUSY:       return message_code::sqlite_busy;
        case SQLITE_LOCKED:     return message_code::sqlite_locked;
        case SQLITE_NOMEM:      return message_code::sqlite_nomem;
        case SQLITE_READONLY:   return message_code::sqlite_readonly;
        case SQLITE_INTERRUPT:  return message_code::sqlite_interrupt;
        case SQLITE_IOERR:      return message_code::sqlite_ioerr;
        case SQLITE_CORRUPT:    return message_code::sqlite_corrupt;
        case SQLITE_NOTFOUND:   return message_code::sqlite_notfound;
        case SQLITE_FULL:       return message_code::sqlite_full;
        case SQLITE_CANTOPEN:   return message_code::sqlite_cantopen;
        case SQLITE_PROTOCOL:   return message_code::sqlite_protocol;
        case SQLITE_EMPTY:      return message_code::sqlite_empty;
        case SQLITE_SCHEMA:     return message_code::sqlite_schema;
        case SQLITE_TOOBIG:     return message_code::sqlite_toobig;
        case SQLITE_CONSTRAINT: return message_code::sqlite_constraint;
        case SQLITE_MISMATCH:   return message_code::sqlite_mismatch;
        case SQLITE_MISUSE:     return message_code::sqlite_misuse;
        case SQLITE_NOLFS:      return message_code::sqlite_nolfs;
        case SQLITE_AUTH:       return message_code::sqlite_auth;
        case SQLITE_FORMAT:     return message_code::sqlite_format;
        case SQLITE_RANGE:      return message_code::sqlite_range;
        case SQLITE_NOTADB:     return message_code::sqlite_notadb;
        case SQLITE_NOTICE:     return message_code::sqlite_notice;
        case SQLITE_WARNING:    return message_code::sqlite_warning;
        case SQLITE_ROW:        return message_code::sqlite_row;
        case SQLITE_DONE:       return message_code::sqlite_done;

        default: return message_code::unrecognised_sqlite_result;
    }
}   // end to_message_code

liberror::liberror(int result, const std::string& file_name) :
    ::sync::error(sqlite::to_message_code(result)),
    m_result(result),
    m_file_name(file_name)
{
}   // end constructor

std::wstring liberror::msg(void) const
{
    // If we have a filename, add it to the standard message
    if (m_file_name.empty()) return message(msg_code());
    else
    {
        std::wstringstream out;
        out << message(msg_code()) << L" - " <<
            message(message_code::fragment_file_name) << L": " <<
            strutils::to_wstring(m_file_name);
        return out.str();
    }
}   // end msg method

database::database(sqlite3* connection, const std::string& file_name) :
    std::enable_shared_from_this<database>(),
    m_connection(connection),
    m_file_name(file_name)
{
}   // end constructor

database::~database(void)
{
    // Close the database.
    auto result = sqlite3_close(m_connection);

    ENSYNC_LOG(logger::ch_information,
        message(message_code::sqlitewrapper_dbclosed) << L" - " <<
        message(message_code::fragment_file_name) << L": " <<
        strutils::to_wstring(m_file_name));

    // If the call returned an error, log a detailed message, but DON'T
    // throw an exception (because we are in a constructor).
    if (result != SQLITE_OK)
        ENSYNC_LOG(logger::ch_error,
            message(message_code::sqlitewrapper_dbcloserror) << L" - " <<
            message(to_message_code(result)) << L" - " <<
            message(message_code::fragment_file_name) << L": " <<
            strutils::to_wstring(m_file_name));
}

database_ptr database::create_new(const std::string& file_name)
{

    // Open the database
    sqlite3* connection = nullptr;
    auto result = sqlite3_open(file_name.c_str(), &connection);

    // Check the result
    if (result != SQLITE_OK)
        ENSYNC_RAISE_SQLITE_LIBERROR(result, file_name);
    
    if (connection == nullptr)
        ENSYNC_RAISE_SQLITE_WRAPPERERROR(
            message_code::sqlitewrapper_nullobjecterror);

    ENSYNC_LOG(logger::ch_information,
        message(message_code::sqlitewrapper_dbopened) << L" - " <<
        message(message_code::fragment_file_name) << L": " <<
        strutils::to_wstring(file_name));

    return database_ptr(new database(connection, file_name));

}   // end create_new method


}}  // end sync::sqlite namespace

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

liberror::liberror(
        int result,
        const std::string& db_file_name,
        const std::string& sql) :
    sqlite::error(sqlite::to_message_code(result), db_file_name, sql),
    m_result(result)
{
}   // end constructor

std::wstring liberror::msg(void) const
{

    // Assemble our message in a stream.
    std::wstringstream out;
    out << message(msg_code());

    // Add the file name if it is not blank
    if (!m_db_file_name.empty())
        out << L" - " << message(message_code::fragment_file_name) <<
            L": \"" << strutils::to_wstring(m_db_file_name) << L"\"";

    // Add the SQL statement if it is not blank; note that we are using the
    // hard-coded string "SQL" here (instead of referencing the message
    // table), because this does not change over different (human) languages.
    if (!m_sql.empty())
        out << L" - SQL: \"" << strutils::to_wstring(m_sql) << L"\"";

    return out.str();

}   // end msg method

std::wstring wrappererror::msg(void) const
{

    // Assemble our message in a stream.
    std::wstringstream out;
    out << message(msg_code());

    // Add the file name if it is not blank
    if (!m_db_file_name.empty())
        out << L" - " << message(message_code::fragment_file_name) <<
            L": \"" << strutils::to_wstring(m_db_file_name) << L"\"";

    // Add the SQL statement if it is not blank; note that we are using the
    // hard-coded string "SQL" here (instead of referencing the message
    // table), because this does not change over different (human) languages.
    if (!m_sql.empty())
        out << L" - SQL: \"" << strutils::to_wstring(m_sql) << L"\"";

    return out.str();

}   // end msg

database::statement::~statement(void)
{

    // Finalise the statement and check the result
    auto result = sqlite3_finalize(m_stmt);

    // Finalisation notification is only logged to the debug stream. We don't
    // normally need to know about it.
    ENSYNC_LOG(logger::ch_debug,
        message(message_code::sqlitewrapper_stmtfnl) << L" - " <<
        message(message_code::fragment_file_name) << L": \"" <<
        strutils::to_wstring(m_db->file_name()) << L"\" - SQL: \"" <<
        strutils::to_wstring(m_sql) << L"\"");

    // If the call returned an error, log a detailed message, but don't throw
    // an exception (because this is a destructor). We note that this would
    // occur if a statement has already caused an execution error, so this
    // error has likely already been logged.
    if (result != SQLITE_OK)
        ENSYNC_LOG(logger::ch_error,
            message(message_code::sqlitewrapper_stmtfnlerror) << L" - " <<
            message(to_message_code(result)) << L" - " <<
            message(message_code::fragment_file_name) << L": \"" <<
            strutils::to_wstring(m_db->file_name()) << L"\" - SQL: \"" <<
            strutils::to_wstring(m_sql) << L"\"");

}   // end destructor

database::statement::step_result database::statement::step(void)
{
    auto result = sqlite3_step(m_stmt);

    // Log what's going on
    ENSYNC_LOG(
        logger::ch_debug,
        message(message_code::sqlitewrapper_step) << L" - " <<
        message(message_code::fragment_result) << L": " <<
        result << L" - " << message(to_message_code(result)) << L" - " <<
        message(message_code::fragment_file_name) << L": \"" <<
        strutils::to_wstring(m_db->file_name()) << "\" - SQL: \"" <<
        strutils::to_wstring(m_sql) << "\"");

    switch (result)
    {
    case SQLITE_BUSY:
        m_current_state = state::success;
        return step_result::busy;

    case SQLITE_ROW:
        m_current_state = state::success;
        return step_result::row;

    case SQLITE_DONE:
        m_current_state = state::success;
        return step_result::done;

    default:
        m_current_state = state::error;
        ENSYNC_RAISE_SQLITE_LIBERROR(result, m_db->file_name(), m_sql);
    }
}   // end step method

database::statement_ptr database::statement::create_new(
        database_ptr db,
        const std::string& sql)
{
    // Prepare the statement, and check for errors.
    sqlite3_stmt* stmt =nullptr;
    auto result = sqlite3_prepare_v2(
        db->m_connection,   // database connection
        sql.c_str(),        // SQL statement string
        -1,                 // Parse the whole stirng
        &stmt,              // pointer for new statement structure
        nullptr);           // don't bother with subsequent parsing

    if (result != SQLITE_OK)
        ENSYNC_RAISE_SQLITE_LIBERROR(result, db->file_name(), sql);

    if (stmt == nullptr)
        ENSYNC_RAISE_SQLITE_WRAPPERERROR(
            message_code::sqlitewrapper_nullobjecterror,
            db->file_name(),
            sql);

    // Log the statement prep to the debug stream - we don't normally need to
    // log every single one.
    ENSYNC_LOG(logger::ch_debug,
        message(message_code::sqlitewrapper_stmtprp) << L" - " <<
        message(message_code::fragment_file_name) << L": \"" <<
        strutils::to_wstring(db->file_name()) << L"\" - SQL: \"" <<
        strutils::to_wstring(sql) << L"\"");

    return statement_ptr(new statement(stmt, db, sql));

}   // end create_new

database::statement::statement(
        sqlite3_stmt* stmt,
        database_ptr db,
        const std::string& sql) :
    m_stmt(stmt),
    m_db(db),
    m_sql(sql),
    m_current_state(state::prepared),
    m_last_step_result(step_result::none)
{
}   // end constructor

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
        message(message_code::fragment_file_name) << L": \"" <<
        strutils::to_wstring(m_file_name) << L"\"");

    // If the call returned an error, log a detailed message, but DON'T
    // throw an exception (because we are in a destructor).
    if (result != SQLITE_OK)
        ENSYNC_LOG(logger::ch_error,
            message(message_code::sqlitewrapper_dbcloserror) << L" - " <<
            message(to_message_code(result)) << L" - " <<
            message(message_code::fragment_file_name) << L": \"" <<
            strutils::to_wstring(m_file_name) << L"\"");
}   // end destructor

database_ptr database::create_new(const std::string& file_name)
{

    // Open the database
    sqlite3* connection = nullptr;
    auto result = sqlite3_open(file_name.c_str(), &connection);

    // Check the result
    if (result != SQLITE_OK)
        ENSYNC_RAISE_SQLITE_LIBERROR(result, file_name, "");
    
    if (connection == nullptr)
        ENSYNC_RAISE_SQLITE_WRAPPERERROR(
            message_code::sqlitewrapper_nullobjecterror,
            file_name,
            "");

    ENSYNC_LOG(logger::ch_information,
        message(message_code::sqlitewrapper_dbopened) << L" - " <<
        message(message_code::fragment_file_name) << L": \"" <<
        strutils::to_wstring(file_name) << L"\"");

    return database_ptr(new database(connection, file_name));

}   // end create_new method

database::statement_ptr database::prepare(const std::string& sql)
{
    return statement::create_new(shared_from_this(), sql);       
}   // end pepare method

}}  // end sync::sqlite namespace

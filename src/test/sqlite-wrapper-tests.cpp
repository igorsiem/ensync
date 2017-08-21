/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

#include <iostream>

// Helper function to set up loggers and return their IDs
std::tuple<int, int, int, int> register_logs(
        std::vector<std::wstring>& err_log,
        std::vector<std::wstring>& war_log,
        std::vector<std::wstring>& inf_log,
        std::vector<std::wstring>& deb_log)
{
    auto& logger = sync::logger::instance();

    auto err_log_id = logger.add(
        {0},
        [&err_log](
                sync::logger::channel,
                const ::sync::logger::label&,
                const std::wstring& msg) {
            err_log.push_back(msg);
        });

    auto war_log_id = logger.add(
        {1},
        [&war_log](
                sync::logger::channel,
                const ::sync::logger::label&,
                const std::wstring& msg) {
            war_log.push_back(msg);
        });

    auto inf_log_id = logger.add(
        {2},
        [&inf_log](
                sync::logger::channel,
                const ::sync::logger::label&,
                const std::wstring& msg) {
            inf_log.push_back(msg);
        });

    auto deb_log_id = logger.add(
        {3},
        [&deb_log](
                sync::logger::channel,
                const ::sync::logger::label&,
                const std::wstring& msg) {
            deb_log.push_back(msg);
        });

    return std::make_tuple(err_log_id, war_log_id, inf_log_id, deb_log_id);
}   // register_logs

// Dereigster logs that were set up using `register_logs`
void deregister_logs(const std::tuple<int, int, int, int> ids)
{
    auto& logger = sync::logger::instance();
    logger.remove(std::get<0>(ids));
    logger.remove(std::get<1>(ids));
    logger.remove(std::get<2>(ids));
    logger.remove(std::get<3>(ids));
}

// All SQLite Wrapper operations are encompassed in this test case.
TEST_CASE("SQLite wrapper operations")
{

    // Test the basic functionality of the exception classes
    SECTION("SQLite error exceptions can be thrown, caught and cloned.")
    {

        // Test the different types of SQLite error

        SECTION("liberror")
        {
            ::sync::sqlite::liberror_ptr ep = nullptr;

            try
            {
                ENSYNC_RAISE_SQLITE_LIBERROR(SQLITE_ERROR, "file name",
                    "SQL");

                // If we get to here, something is wrong!
                FAIL("SQLite liberror exception was NOT thrown");
            }
            catch (const ::sync::sqlite::liberror& e)
            {
                // The caught exception has the details we set.
                REQUIRE(e.result() == SQLITE_ERROR);
                REQUIRE(e.db_file_name() == "file name");
                REQUIRE(e.sql() == "SQL");

                // Clone the error object.
                ep = std::dynamic_pointer_cast<::sync::sqlite::liberror>(
                    e.clone());
            }

            // The exception has been cloned, and has the details we set.
            REQUIRE(ep != nullptr);
            REQUIRE(ep->result() == SQLITE_ERROR);
            REQUIRE(ep->db_file_name() == "file name");
            REQUIRE(ep->sql() == "SQL");
            REQUIRE(ep->msg_code() ==
                ::sync::sqlite::to_message_code(SQLITE_ERROR));

        }

        SECTION("wrappererror")
        {

            ::sync::sqlite::wrappererror_ptr ep = nullptr;

            try
            {
                ENSYNC_RAISE_SQLITE_WRAPPERERROR(
                    ::sync::message_code::sqlitewrapper_nullobjecterror,
                    "file name",
                    "SQL");

                // If we get to here, something is wrong!
                FAIL("SQLite wrappererror exception was NOT thrown");
            }
            catch (const ::sync::sqlite::wrappererror& e)
            {
                // The caught exception has the details we set.
                REQUIRE(e.msg_code() ==
                    ::sync::message_code::sqlitewrapper_nullobjecterror);
                REQUIRE(e.db_file_name() == "file name");
                REQUIRE(e.sql() == "SQL");

                // Clone the error object.
                ep = std::dynamic_pointer_cast<::sync::sqlite::wrappererror>(
                    e.clone());
            }

            // The exception has been cloned, and has the details we set.
            REQUIRE(ep != nullptr);
            REQUIRE(ep->msg_code() ==
                ::sync::message_code::sqlitewrapper_nullobjecterror);
            REQUIRE(ep->db_file_name() == "file name");
            REQUIRE(ep->sql() == "SQL");

        }
    }   // end SQLite error exceptions

    // Database operations are potentially logged on all standard channels.
    //
    // TODO Put this in a helper function
    std::vector<std::wstring> err_log, war_log, inf_log, deb_log;
    auto ids = register_logs(err_log, war_log, inf_log, deb_log);
    
    // Open a database - the file will be created if necessary
    std::string db_file_name = "test-artefacts/sqlite-wrapper-test.db";
    auto db = sync::sqlite::database::create_new(db_file_name);
    REQUIRE(db != nullptr);

    // We should have one entry in the info log to signal the opening of
    // the database, and none in the others.
    REQUIRE(err_log.size() == 0);
    REQUIRE(war_log.size() == 0);
    REQUIRE(inf_log.size() == 1);
    REQUIRE(deb_log.size() == 0);

    // Prepare a legal SQL statement.
    std::string legal_sql = "SELECT name FROM sqlite_master "
        "WHERE type='table';";
    sync::sqlite::database::statement_ptr stmt = nullptr;
    REQUIRE_NOTHROW(stmt = db->prepare(legal_sql));

    // A debug message has been logged.
    REQUIRE(err_log.size() == 0);
    REQUIRE(war_log.size() == 0);
    REQUIRE(inf_log.size() == 1);
    REQUIRE(deb_log.size() == 1);

    // Delete / finalise the statement.
    stmt = nullptr;

    // Another debug message has been logged
    REQUIRE(err_log.size() == 0);
    REQUIRE(war_log.size() == 0);
    REQUIRE(inf_log.size() == 1);
    REQUIRE(deb_log.size() == 2);

    // Delete / close the database
    db = nullptr;

    // At this point, the database should be closed. There should be one
    // more entry in the info log.

    REQUIRE(err_log.size() == 0);
    REQUIRE(war_log.size() == 0);
    REQUIRE(inf_log.size() == 2);
    REQUIRE(deb_log.size() == 2);

    // Remove the logging endpoints we got before
    deregister_logs(ids);
}   // end SQLite wrapper operations test case

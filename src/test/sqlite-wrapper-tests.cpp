/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <ensync/ensync.h>
#include <CATCH/catch.hpp>
#include "test-helpers.h"

// All SQLite Wrapper operations are encompassed in this test case.
TEST_CASE("SQLite wrapper operations")
{

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

    // Prepare a legal SQL statement that creates a new table in the
    // database.
    std::string sql =
            "CREATE TABLE People ( "
                "Id INT PRIMARY KEY, "
                "Name NVARCHAR(100) NOT NULL)";
    sync::sqlite::database::statement_ptr stmt = nullptr;
    REQUIRE_NOTHROW(stmt = db->prepare(sql));

    // A debug message has been logged.
    REQUIRE(err_log.size() == 0);
    REQUIRE(war_log.size() == 0);
    REQUIRE(inf_log.size() == 1);
    REQUIRE(deb_log.size() == 1);

    // Execute the statement - it should be completed in a single step.
    auto r = sync::sqlite::database::statement::step_result::none;
    REQUIRE_NOTHROW(r = stmt->step());
    REQUIRE(r == sync::sqlite::database::statement::step_result::done);

    // A debug message has been logged.
    REQUIRE(err_log.size() == 0);
    REQUIRE(war_log.size() == 0);
    REQUIRE(inf_log.size() == 1);
    REQUIRE(deb_log.size() == 2);

    // Delete / finalise the statement.
    stmt = nullptr;

    // Another debug message has been logged
    REQUIRE(err_log.size() == 0);
    REQUIRE(war_log.size() == 0);
    REQUIRE(inf_log.size() == 1);
    REQUIRE(deb_log.size() == 3);    

    // Prepare and execute another statement to insert some rows into the
    // table
    sql =
        "INSERT INTO People (Id, Name) "
        "VALUES "
            "(1, 'John'), "
            "(2, 'Paul'), "
            "(3, 'George'), "
            "(4, 'Ringo')";

    REQUIRE_NOTHROW(stmt = db->prepare(sql));

    // Execute the statement and then finalise it - it should be completed
    // in a single step.
    r = sync::sqlite::database::statement::step_result::none;
    REQUIRE_NOTHROW(r = stmt->step());
    REQUIRE(r == sync::sqlite::database::statement::step_result::done);

    stmt = nullptr;

    // Three more debug messages have been added
    REQUIRE(deb_log.size() == 6);              

    // Delete / close the database
    db = nullptr;

    // At this point, the database should be closed. There should be one
    // more entry in the info log.

    REQUIRE(err_log.size() == 0);
    REQUIRE(war_log.size() == 0);
    REQUIRE(inf_log.size() == 2);
    REQUIRE(deb_log.size() == 6);

    // Remove the logging endpoints we got before
    deregister_logs(ids);
}   // end SQLite wrapper operations test case

// SQLite error exceptions can be thrown, caught and cloned.
TEST_CASE("SQLite Wrapper exception classes")
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

    }   // end liberror section

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

    }   // end wrappererror section

}   // end SQLite Wrapper exception classes test case

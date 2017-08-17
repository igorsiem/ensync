/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <sqlite/sqlite3.h>

// Test very basic, 'unvarnished' SQLite operations
TEST_CASE("'Raw' SQLite operations")
{
    
    sqlite3 *db_handle = nullptr;
    std::string db_file_name = "test-artefacts/sqlite-test.db";

    // Open the database, creating the file if it does not exist (which it
    // doesn't)
    REQUIRE(sqlite3_open(db_file_name.c_str(), &db_handle) == SQLITE_OK);

    // Create a table - first we prepare the statement...
    std::string create_query =
        "CREATE TABLE People ( "
            "Id INT PRIMARY KEY, "
            "Name NVARCHAR(100) NOT NULL)";

    sqlite3_stmt *stmt = nullptr;
    REQUIRE(
        sqlite3_prepare_v2(
            db_handle,
            create_query.c_str(),
            -1,
            &stmt,
            nullptr) == SQLITE_OK);

    // Now we execute it.
    REQUIRE(sqlite3_step(stmt) == SQLITE_DONE);

    // Finalise the statement.
    REQUIRE(sqlite3_finalize(stmt) == SQLITE_OK);
    stmt = nullptr;

    // Now insert some data
    std::string insert_query =
        "INSERT INTO People (Id, Name) "
        "VALUES "
            "(1, 'John'), "
            "(2, 'Paul'), "
            "(3, 'George'), "
            "(4, 'Ringo')";

    REQUIRE(
        sqlite3_prepare_v2(
            db_handle,
            insert_query.c_str(),
            -1,
            &stmt,
            nullptr) == SQLITE_OK);
    REQUIRE(sqlite3_step(stmt) == SQLITE_DONE);
    REQUIRE(sqlite3_finalize(stmt) == SQLITE_OK);
    stmt = nullptr;

    // Retrieve some data
    std::string select_query = "SELECT * FROM People WHERE Name = 'George'";
    REQUIRE(
        sqlite3_prepare_v2(
            db_handle,
            select_query.c_str(),
            -1,
            &stmt,
            nullptr) == SQLITE_OK);
    
    // We should receive a single row.
    REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);

    REQUIRE(sqlite3_column_type(stmt, 0) == SQLITE_INTEGER);
    REQUIRE(sqlite3_column_int(stmt, 0) == 3);
    REQUIRE(sqlite3_column_type(stmt, 1) == SQLITE_TEXT);
    REQUIRE((const char*)sqlite3_column_text(stmt, 1) ==
        std::string("George"));

    REQUIRE(sqlite3_finalize(stmt) == SQLITE_OK);
    stmt = nullptr;

    // Close the database connection
    REQUIRE(sqlite3_close(db_handle) == SQLITE_OK);

}   // end Raw SQLite operations test

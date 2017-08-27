/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

// A basic tess of the messages table - note that verification is (human)
// language-specific!
TEST_CASE("Message operations")
{

    // Pick a few message codes to check
    REQUIRE(
        sync::message(sync::message_code::unspec_error) ==
        L"unspecified error");

    REQUIRE(
        sync::message(sync::message_code::sqlite_corrupt) ==
        L"SQLite database file is corrupted");

    REQUIRE(
        sync::message(sync::message_code::sqlite_schema) ==
        L"SQLite database schema has changed");

    REQUIRE(
        sync::message(sync::message_code::sqlite_nolfs) ==
        L"no large file system support for SQLite");

    REQUIRE(
        sync::message(sync::message_code::sqlite_done) ==
        L"SQLite operation complete");

    REQUIRE(
        sync::message(sync::message_code::fragment_result) ==
        L"result");

    REQUIRE(
        sync::message(sync::message_code::data_type_string) ==
        L"string");

    // This line should not compile - no raw ints!
    // REQUIRE(sync::message(5) == L"");

}   // end Message operations test case

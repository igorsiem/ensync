/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

// All SQLite Wrapper operations are encompassed in this test case.
TEST_CASE("SQLite wrapper operations")
{

    SECTION("SQLite error exceptions can be thrown, caught and cloned.")
    {

        ::sync::sqlite::error_ptr ep = nullptr;

        try
        {
            ENSYNC_RAISE_SQLITE_ERROR(SQLITE_ERROR, "filename");

            // If we get to here, something is wrong!
            FAIL("SQLite error exception was NOT thrown");
        }
        catch (const ::sync::sqlite::error& e)
        {
            // The caught exception has the details we set.
            REQUIRE(e.result() == SQLITE_ERROR);
            REQUIRE(e.file_name() == "filename");

            // Clone the error object.
            ep = std::dynamic_pointer_cast<::sync::sqlite::error>(e.clone());
        }

        // The exception has been cloned, and has the details we set.
        REQUIRE(ep != nullptr);
        REQUIRE(ep->result() == SQLITE_ERROR);
        REQUIRE(ep->file_name() == "filename");
        REQUIRE(ep->msg_code() ==
            ::sync::sqlite::to_message_code(SQLITE_ERROR));
    }   // end SQLite error exceptions

}   // end SQLite wrapper operations test case

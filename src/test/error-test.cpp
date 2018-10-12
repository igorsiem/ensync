/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

// Verify basic errors / exceptions
TEST_CASE("basic errors", "[unit]")
{

    // Test the 'unknown' error
    try
    {
        throw ensync::unknown_error();
        FAIL("ensync::unknown_error exception should have been thrown");
    }
    catch (const ensync::error& e)
    {
        // Exception was caught, and has the correct message
        REQUIRE(e.message() ==
            ensync::get(ensync::message_code_t::unknown_err));
    }
    catch (...)
    {
        FAIL("some exception other than ensync::unknown_error was thrown");
    }

    // Test runtime assertions
    // ensync::logger::instance().set_for_console();
    try
    {
        ENSYNC_ASSERT( false == true );
        FAIL("ensync::runtime_assertion_error should have been thrown");
    }
    catch (const ensync::runtime_assertion_error& e)
    {
        // Exact wording of message indeterminate.
        REQUIRE(e.message().empty() == false);
    }
    catch (...)
    {
        FAIL("some exception other than ensync::runtime_assertion_error was "
            "thrown");
    }
    // ensync::logger::instance().clear();

}   // end basic error handling test

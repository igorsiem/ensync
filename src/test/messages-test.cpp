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

    REQUIRE(
        sync::message(sync::message_code::unspec_error) ==
        L"unspecified error");

    // This line should not compile - no raw ints!
    // REQUIRE(sync::message(5) == L"");

}   // end Message operations test case

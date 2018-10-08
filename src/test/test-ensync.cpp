/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <string>
#define CATCH_CONFIG_MAIN
#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

TEST_CASE("version", "[unit]")
{
    REQUIRE(std::wstring(ensync::version) == L"0.0");
}   // end version test

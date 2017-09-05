/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

// Simple exercise of basic ID operations
TEST_CASE("ID operations")
{
    SECTION("Basic ID")
    {

        // form valid and invalid IDs
        sync::id
            valid_id = L"abc",
            invalid_id = std::wstring(L"abc") +
                sync::id_separator + std::wstring(L"xyz");

        REQUIRE(sync::is_valid_id(valid_id) == true);
        REQUIRE(sync::is_valid_id(invalid_id) == false);
    }   // end basic ID section

    SECTION("ID path")
    {
        sync::id_path_string idp_str = sync::id(L"abc") +
            sync::id_separator + sync::id(L"xyz");

        REQUIRE(sync::to_id_path(idp_str) ==
            sync::id_path({L"abc", L"xyz"}));

        REQUIRE(
            sync::to_id_path_string(sync::id_path({L"abc", L"xyz"})) ==
            idp_str);
    }   // end ID path section

}   // end ID operations test case

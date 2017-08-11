/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

using namespace sync;

// This test verifies string utility functionality.
TEST_CASE("String Utilities operations")
{
	// To standard string
	REQUIRE(strutils::to_string(std::wstring(L"abc")) == "abc");
	REQUIRE(strutils::to_string(1) == "1");
	REQUIRE(strutils::to_string(2.2) == "2.2");

	// To wide string
	REQUIRE(strutils::to_wstring(std::string("abc")) == L"abc");
	REQUIRE(strutils::to_wstring(1) == L"1");
	REQUIRE(strutils::to_wstring(2.2) == L"2.2");

    // Concatenation
    //
    // First, a straight concatenate with delimiter
    std::vector<std::string> v1({"abc", "def", "ghi"});
    REQUIRE(strutils::concatenate(v1, std::string("-")) == "abc-def-ghi");

    // ... now, without a delimiter
    REQUIRE(strutils::concatenate(v1) == "abcdefghi");

    // ... now with wide strings
    std::vector<std::wstring> wv1({L"abc", L"def", L"ghi"});
    REQUIRE(strutils::concatenate(wv1, std::wstring(L"-")) == L"abc-def-ghi");
    REQUIRE(strutils::concatenate(wv1) == L"abcdefghi");

    // Concatenate with just one element - delimiter / non-delimiter cases
    // should be the same.
    std::vector<std::string> v2({"abc"});
    REQUIRE(strutils::concatenate(v2, std::string("-")) == "abc");
    REQUIRE(strutils::concatenate(v2) == "abc");

    std::vector<std::wstring> wv2({L"abc"});
    REQUIRE(strutils::concatenate(wv2, std::wstring(L"-")) == L"abc");
    REQUIRE(strutils::concatenate(wv2) == L"abc");

    // Concatenate with just zero elements - delimiter / non-delimiter cases
    // should be the same.
    std::vector<std::string> v3;
    REQUIRE(strutils::concatenate(v3, std::string("-")) == "");
    REQUIRE(strutils::concatenate(v3) == "");

    std::vector<std::wstring> wv3;
    REQUIRE(strutils::concatenate(wv3, std::wstring(L"-")) == L"");
    REQUIRE(strutils::concatenate(wv3) == L"");

}	// end strutils test

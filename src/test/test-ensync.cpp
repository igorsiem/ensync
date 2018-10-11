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
#include <fmt/format.h>

// Verify that our version number is all correct
TEST_CASE("version", "[unit]")
{
    REQUIRE(std::wstring(ensync::version) == L"0.0");
}   // end version test

// Verify the working of the fmt library
TEST_CASE("fmt", "[external]")
{

    std::wstring answer = fmt::format(L"the answer is {}.", 42);
    REQUIRE(answer == L"the answer is 42.");

}   // end fmt test

// Verify simple logging
TEST_CASE("logger", "[unit]")
{

    ensync::logger::instance().set_for_console();

    // Un-comment these lines to output test log messages
    // ensync::logger::instance().log(
    //     ensync::logger::level_t::error,
    //     L"an error");
    // ensync::logger::instance().log(
    //     ensync::logger::level_t::warning,
    //     L"a warning");
    // ensync::logger::instance().log(
    //     ensync::logger::level_t::info,
    //     L"information");
    // ensync::logger::instance().log(
    //     ensync::logger::level_t::debug,
    //     L"debugging");

    ensync::logger::instance().clear();

}   // end logger test

// Verify our extension to boost's lexical cast that supports conversions
// between strings and wide-strings
TEST_CASE("wide-string lexical cast", "[unit]")
{
    using namespace boost;
    REQUIRE(lexical_cast<std::wstring>(std::string("abc")) == L"abc");
    REQUIRE(lexical_cast<std::string>(std::wstring(L"abc")) == "abc");
}   // end wide-string lexical cast test

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

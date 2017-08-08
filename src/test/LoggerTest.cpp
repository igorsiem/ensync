/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

TEST_CASE("Logger channels can be labelled")
{

    auto& logger = sync::Logger::instance();

    logger.set_channel_label(0, L"ERR");    // error
    logger.set_channel_label(1, L"WAR");    // warning
    logger.set_channel_label(2, L"INF");    // information
    logger.set_channel_label(3, L"DEB");    // debugging info

    REQUIRE(logger.channel_label(0) == L"ERR");
    REQUIRE(logger.channel_label(1) == L"WAR");
    REQUIRE(logger.channel_label(2) == L"INF");
    REQUIRE(logger.channel_label(3) == L"DEB");

}   // end "Logger channels can be labelled" test case

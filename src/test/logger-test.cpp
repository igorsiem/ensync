/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

// Testing the Logger can be problematic, because it is a singleton, so its
// state is retained across test cases. For this reason, we only test
// conventional usage, and don't make any assumptions about existing state.

TEST_CASE("Logger operations")
{

    auto& logger = sync::logger::instance();
        
    SECTION("Logger has four predefined channels")
    {
        REQUIRE(logger.channel_label(0) == L"ERR");
        REQUIRE(logger.channel_label(1) == L"WAR");
        REQUIRE(logger.channel_label(2) == L"INF");
        REQUIRE(logger.channel_label(3) == L"DEB");

        // When the label of a non-labelled channel is requested, a blank
        // string is returned.
        REQUIRE(logger.channel_label(9999) == L"");        
    }   // end channel label checking section

    SECTION("Logging endpoints can be registered, used and deregistered")
    {

        // Set up some test logging endpoints with vectors of strings that
        // we can check.
        std::vector<std::wstring> ep1, ep2;
        auto ep_fn = [](
                std::vector<std::wstring>& ep,
                const sync::logger::label& lbl,
                const std::wstring& msg)
        {
            ep.push_back(lbl + L" - " + msg);
        };

        // Endpoint 1 is just for errors, whereas EP 2 is for all four types
        // of log message.
        auto ep1_id = logger.add(
            {0},
            [&ep1,ep_fn](
                    sync::logger::channel,
                    const sync::logger::label& lbl,
                    const std::wstring& msg)
            {
                ep_fn(ep1, lbl, msg);
            });

        auto ep2_id = logger.add(
            {0, 1, 2, 3},
            [&ep2,ep_fn](
                    sync::logger::channel,
                    const sync::logger::label& lbl,
                    const std::wstring& msg)
            {
                ep_fn(ep2, lbl, msg);
            });

        // Log an error message - this should go to both endpoints.
        ENSYNC_LOG(0, L"error message 1");

        // Both EPs have exactly one message now.
        REQUIRE(ep1.size() == 1);
        REQUIRE(ep1.back() == L"ERR - error message 1");
        REQUIRE(ep2.size() == 1);
        REQUIRE(ep2.back() == L"ERR - error message 1");

        // Log a warning - this should only go to EP2.
        ENSYNC_LOG(1, "warning message 1");

        REQUIRE(ep1.size() == 1);
        REQUIRE(ep1.back() == L"ERR - error message 1");
        REQUIRE(ep2.size() == 2);
        REQUIRE(ep2.back() == L"WAR - warning message 1");

        // Deregister EP2.
        logger.remove(ep2_id);

        // Now, when we log an error message, it is only sent to EP1.
        ENSYNC_LOG(0, L"error message 2");
        REQUIRE(ep1.size() == 2);
        REQUIRE(ep1.back() == L"ERR - error message 2");
        REQUIRE(ep2.size() == 2);
        REQUIRE(ep2.back() == L"WAR - warning message 1");

        // Don't forget to deregister EP1, so that it doesn't affect other
        // tests.
        logger.remove(ep1_id);

        SECTION("Non-existent endpoints can be removed without error")
        {
            // Attempt to remove EP1 again.
            REQUIRE_NOTHROW(logger.remove(ep1_id));
        }   // end non-existent endpoint removal section

    }   // end logging endpoint section

}   // end logger operations endpoint

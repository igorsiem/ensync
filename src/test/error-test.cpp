/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <sstream>
#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

TEST_CASE("Error handling")
{

    // If we are logging exception throwing, then test that.
#ifdef ENSYNC_LOG_EXCEPTIONS
    // Add an enpoint to the logger to catch errors.
    std::vector<std::wstring> errors;
    auto& logger = sync::logger::instance();
    auto error_ep_id = logger.add(
        { sync::logger::ch_error},
        [&errors](
                sync::logger::channel cnl,
                const sync::logger::label& lbl,
                const std::wstring& msg)
        {
            std::wstringstream strm;
            strm << L"[" << lbl << L"] " << msg;
            errors.push_back(strm.str());
        });
#endif

    // Test macro for throwing an error
    REQUIRE_THROWS_AS(
        ENSYNC_RAISE_ERROR(sync::message_code::unspec_error),
        sync::error );

#ifdef ENSYNC_LOG_EXCEPTIONS
    REQUIRE(errors.size() == 1);
    
    // Note: we're not going to check the full message text, because it will
    // include the file path, which will change at lot.

    // Don't forget to remove the logger endpoint.
    logger.remove(error_ep_id);
#endif

    SECTION("Error objects can be cloned after they are caught.")
    {
        ::sync::error_ptr ep = nullptr;

        try
        {
            throw ::sync::error(::sync::message_code::unspec_error);
            
            // If we get to here, something is wrong.
            FAIL("error exception was NOT thrown");
        }
        catch (const ::sync::error& e)
        {
            // The caught exception has the details we set.
            REQUIRE(e.msg_code() == ::sync::message_code::unspec_error);

            // Clone the exception.
            ep = e.clone();
        }

        // The cloned exception object has the details we set.
        REQUIRE(ep != nullptr);
        REQUIRE(ep->msg_code() == ::sync::message_code::unspec_error);
    }

}   // end Error handling test case

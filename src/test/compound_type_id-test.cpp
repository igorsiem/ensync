/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

// Test the basic workings of the compound type ID
TEST_CASE("compound type id", "[unit]")
{

    // Create valid and invalid IDs
    ensync::compound_type_id
        id_valid(ensync::make_random_uuid(), L"my id")
        , id_nil_uuid(ensync::make_nil_uuid(), L"a nil id")
        , id_empty_name(ensync::make_random_uuid(), L"")
        , id_default;

    REQUIRE(id_valid.is_valid());
    REQUIRE(!id_nil_uuid.is_valid());
    REQUIRE(!id_empty_name.is_valid());
    REQUIRE(!id_default.is_valid());

}   // end compound type ID test

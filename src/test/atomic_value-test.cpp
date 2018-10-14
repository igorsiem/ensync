/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

// Test the core value type
TEST_CASE("atomic value", "[unit]")
{

    using namespace ensync;

    atomic_value
        v1{1}
        , v2{L"2"}
        , v3{3.3}
        ;
    
    REQUIRE(type_of(v1) == atomic_type_id::int_avt);
    REQUIRE(type_of(v2) == atomic_type_id::wstring_avt);
    REQUIRE(type_of(v3) == atomic_type_id::double_avt);

}   //end value test

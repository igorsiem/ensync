/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

TEST_CASE("type id", "[unit]")
{

    using namespace ensync;

    compound_type_store cts;

    // Create an uninitialised type - should be a compound type that is
    // nullptr.
    type_id unknown_tid;
    REQUIRE(unknown_tid.index() == 0);
    REQUIRE(std::get<compound_type_id>(unknown_tid).is_nil());
    REQUIRE(is_unknown(unknown_tid));
    REQUIRE(name_of(unknown_tid, cts) == get(message_code_t::unknown_type));

    // Create valid atomic and compound types, and check them
    type_id
        atomic_tid = atomic_type_id::wstring_avt
        , compound_tid =
            generate_new_compound_type_id(L"my-type", cts)
        ;

    REQUIRE(atomic_tid.index() == 1);
    REQUIRE(std::get<atomic_type_id>(atomic_tid) ==
        atomic_type_id::wstring_avt);
    REQUIRE(is_unknown(atomic_tid) == false);
    REQUIRE(name_of(atomic_tid, cts) == get(message_code_t::string_type));

    REQUIRE(compound_tid.index() == 0);
    REQUIRE(std::get<compound_type_id>(compound_tid).is_nil() == false);
    REQUIRE(is_unknown(compound_tid) == false);
    REQUIRE(name_of(compound_tid, cts) == L"my-type");

}   // end type id test
  
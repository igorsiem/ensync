/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

 // Very VERY basic test of YAML parsing. We assume that the library performs
 // as advertised -- we are only really verifying that it is integrated
 // correctly.

 #include <CATCH/catch.hpp>
 #include <yaml-cpp/yaml.h>

 TEST_CASE("YAML read operations", "[YAML][Unit]")
 {
     // Test parsing of a simple sequence.
     YAML::Node node = YAML::Load("[1, 2, 3]");
     REQUIRE(node.Type() == YAML::NodeType::Sequence);
     REQUIRE(node.IsSequence());
     REQUIRE(node.size() == 3);
     REQUIRE(node[0].as<int>() == 1);
     REQUIRE(node[1].as<int>() == 2);
     REQUIRE(node[2].as<int>() == 3);
 }	// end YAML read operations test
 
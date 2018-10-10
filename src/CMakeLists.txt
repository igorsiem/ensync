# This file is part of the enSync project, written by Igor Siemienowicz.
#
# Copyright (c) 2017 Igor Siemienowicz All rights reserved Distributed under
# the Boost Software License, Version 1.0 (See
# hytp://www.boost.org/LICENSE_1_0.txt).

# Top-level CMake settings for enSync
cmake_minimum_required(VERSION 3.10)
project(enSync)

SET(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(
    .
    third-party
)

# Sub-projects
add_subdirectory(ensync)
add_subdirectory(test)
add_subdirectory(third-party)

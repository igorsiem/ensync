# This file is part of the enSync project, written by Igor Siemienowicz.
#
# Copyright (c) 2017 Igor Siemienowicz All rights reserved Distributed under
# the Boost Software License, Version 1.0 (See
# hytp://www.boost.org/LICENSE_1_0.txt).

# CMake settings for the test-ensync test executable

file(GLOB ENSYNC_SRC *.cpp)

add_library(
    ensync
    ${ENSYNC_SRC})

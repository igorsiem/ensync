/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <ensync/ensync.h>

 #ifndef enSync_test_helpers_h_included
 #define enSync_test_helpers_h_included

// Helper function to set up loggers and return their IDs
extern std::tuple<int, int, int, int> register_logs(
        std::vector<std::wstring>& err_log,
        std::vector<std::wstring>& war_log,
        std::vector<std::wstring>& inf_log,
        std::vector<std::wstring>& deb_log);

// Dereigster logs that were set up using `register_logs`
extern void deregister_logs(const std::tuple<int, int, int, int> ids);

 #endif

/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "test-helpers.h"

std::tuple<int, int, int, int> register_logs(
        std::vector<std::wstring>& err_log,
        std::vector<std::wstring>& war_log,
        std::vector<std::wstring>& inf_log,
        std::vector<std::wstring>& deb_log)
{
    auto& logger = sync::logger::instance();

    auto err_log_id = logger.add(
        {0},
        [&err_log](
                sync::logger::channel,
                const ::sync::logger::label&,
                const std::wstring& msg) {
            err_log.push_back(msg);
        });

    auto war_log_id = logger.add(
        {1},
        [&war_log](
                sync::logger::channel,
                const ::sync::logger::label&,
                const std::wstring& msg) {
            war_log.push_back(msg);
        });

    auto inf_log_id = logger.add(
        {2},
        [&inf_log](
                sync::logger::channel,
                const ::sync::logger::label&,
                const std::wstring& msg) {
            inf_log.push_back(msg);
        });

    auto deb_log_id = logger.add(
        {3},
        [&deb_log](
                sync::logger::channel,
                const ::sync::logger::label&,
                const std::wstring& msg) {
            deb_log.push_back(msg);
        });

    return std::make_tuple(err_log_id, war_log_id, inf_log_id, deb_log_id);
}   // register_logs

// Dereigster logs that were set up using `register_logs`
void deregister_logs(const std::tuple<int, int, int, int> ids)
{
    auto& logger = sync::logger::instance();
    logger.remove(std::get<0>(ids));
    logger.remove(std::get<1>(ids));
    logger.remove(std::get<2>(ids));
    logger.remove(std::get<3>(ids));
}

/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "logger.h"

namespace sync {

logger& logger::instance(void)
{
    static logger l;
    return l;
}   // end instance method

void logger::set_channel_label(channel c, const label& l)
{
    write_lock wl(m_mutex);
    m_channel_labels[c] = l;
}   // end set_channel_label method

const logger::label logger::channel_label(channel c) const
{
    read_lock rl(m_mutex);
    auto itr = m_channel_labels.find(c);
    if (itr == m_channel_labels.end()) return label();
    else return itr->second;
}   // end channel_label method

logger::endpoint_id logger::add(const channel_set& channels, endpoint e)
{
    if (e == nullptr) return 0;

    write_lock wl(m_mutex);

    // Assign a new ID for the endpoint (note post-increment) and add it to
    // the endpoints collection.
    auto id = m_next_endpoint_id++;
    m_endpoints[id] = e;

    // For each of the requested channels, add the endpoint. Note that, if
    // any requested channel has not been previously specified (and
    // therefore) has no corresponding set, one will be created 'on the fly'.
    for (auto c : channels) m_channel_endpoint_map[c].insert(id);

    return id;
}   // end add method

void logger::log(channel c, const std::wstring& msg)
{
    // Write lock the mutex - we're not actually changing the state of the
    // logger in any meaningful way, but a log operation notionally writes
    // to the logs (by calling the endpoints), and the logs are notionally
    // part of the logger.
    write_lock wl(m_mutex);

    // Get the channel label, and the channel collection (if they exist).
    auto lbl_itr = m_channel_labels.find(c);
    auto channel_endpoint_set_itr = m_channel_endpoint_map.find(c);

    if (channel_endpoint_set_itr != m_channel_endpoint_map.end())
    {
        // Loop through the endpoint IDs in this channel set.
        for (auto eid : channel_endpoint_set_itr->second)
        {
            // Get the endpoint callable. We check that the ID has been
            // properly registered, and does not point to a null function.
            auto endpoint_itr = m_endpoints.find(eid);
            if (endpoint_itr != m_endpoints.end())
            {
                auto ep_fn = endpoint_itr->second;
                if (ep_fn != nullptr)
                {
                    // At this point, we're pretty sure we have a valid EP
                    // function. Do we have a label?
                    if (lbl_itr == m_channel_labels.end())
                        ep_fn(c, label(), msg);
                    else ep_fn(c, lbl_itr->second, msg);
                }
            }

        }   // endpoint ID set loop
    }   // end if we have a channel endpoint ID set

}   // end log method

void logger::remove(endpoint_id id)
{
    write_lock wl(m_mutex);

    // Loop through all the channels first, looking for references to the
    // endpoint, and removing them.
    for (auto& ep_set : m_channel_endpoint_map) ep_set.second.erase(id);

    // Now erase the endpoint itself.
    m_endpoints.erase(id);
}   // end remove method

logger::logger(void) :
    m_mutex(),
    m_channel_labels(),
    m_next_endpoint_id(1),
    m_endpoints(),
    m_channel_endpoint_map()
{
    // Label the 'conventional' channels
    set_channel_label(ch_error, L"ERR");
    set_channel_label(ch_warning, L"WAR");
    set_channel_label(ch_information, L"INF");
    set_channel_label(ch_debug, L"DEB");
}   // end constructor

}   // end sync namespace

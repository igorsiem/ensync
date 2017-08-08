/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "Logger.h"

namespace sync {

Logger& Logger::instance(void)
{
    static Logger l;
    return l;
}   // end instance method

void Logger::set_channel_label(channel c, const label& l)
{
    write_lock wl(m_mutex);
    m_channel_labels[c] = l;
}   // end set_channel_label method

const Logger::label Logger::channel_label(channel c) const
{
    read_lock rl(m_mutex);
    auto itr = m_channel_labels.find(c);
    if (itr == m_channel_labels.end()) return label();
    else return itr->second;
}   // end channel_label method

Logger::Logger(void) : m_mutex(), m_channel_labels()
{    
}   // end constructor

}   // end sync namespace

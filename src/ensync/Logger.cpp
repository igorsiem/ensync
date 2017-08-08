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

Logger::Logger(void) : m_mutex()
{    
}   // end constructor

}   // end sync namespace

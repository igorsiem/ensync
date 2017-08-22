/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "error.h"

namespace sync {

std::wstring error::msg(void) const
{
    return ::sync::message(m_msg_code);
}   // end msg method

error_ptr error::clone(void) const
{
    return std::make_shared<error>(m_msg_code);
}   // end clone method

}   // end namespace sync

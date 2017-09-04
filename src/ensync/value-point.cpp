/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "value-point.h"

namespace sync {

const id_path& value_point::attribute_id_path(void) const
{
    read_lock rl(m_mutex);
    return m_attribute_id_path;
}   // end attribute_id_path

}   // end sync namespace

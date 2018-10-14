/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/random_generator.hpp>

#ifndef _ensync_uuid_h_included
#define _ensync_uuid_h_included

namespace ensync {

/**
 * \brief Unique identifier for use in multiple contexts
 */
using uuid_t = boost::uuids::uuid;

/**
 * \brief Generate a nil UUID (all zeros)
 * 
 * \return A nil UUID object
 */
inline uuid_t make_nil_uuid(void) { return boost::uuids::nil_generator()(); }

/**
 * \brief Generate a random UUID
 * 
 * \return A randomly-generated UUID
 */
inline uuid_t make_random_uuid(void)
    { return boost::uuids::random_generator()(); }

}   // end ensync namespace

#endif

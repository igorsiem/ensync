/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <shared_mutex>

#ifndef enSync_thread_safety_h_included
#define enSync_thread_safety_h_included

/**
 * \brief The main namespace for enSync declarations
 */
namespace sync {

/**
 * \brief The main mutex type used within enSync
 *
 * The main reason we want this type is to enable read (shared) vs.
 * read/write (exclusive) access.
 */
using mutex = std::shared_mutex;

/**
 * \brief A shared lock that we can use for reading thread-protected data
 */
using read_lock = std::shared_lock<mutex>;

/**
 * \brief An exclusive lock for writing to thread-protected data
 */
using write_lock = std::unique_lock<mutex>;

}   // end sync namespace

#endif

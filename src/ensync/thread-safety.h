/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

// We need to check for GCC here, because of issue #12 - at the time of
// writing, GCC doesn't support shared_mutex or shared_lock.
#ifdef __GNUC__
    #define ENSYNC_NO_SHARED_MUTEX 1
#endif

#ifdef ENSYNC_NO_SHARED_MUTEX
    #include <mutex>
#else
    #include <shared_mutex>
#endif

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

#ifdef ENSYNC_NO_SHARED_MUTEX
    using mutex = std::mutex;
#else
    using mutex = std::shared_mutex;
#endif

/**
 * \brief A shared lock that we can use for reading thread-protected data
 */
#ifdef ENSYNC_NO_SHARED_MUTEX
    using read_lock = std::unique_lock<mutex>;
#else
    using read_lock = std::shared_lock<mutex>;
#endif

/**
 * \brief An exclusive lock for writing to thread-protected data
 */
using write_lock = std::unique_lock<mutex>;

}   // end sync namespace

#endif


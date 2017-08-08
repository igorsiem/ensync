/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "thread-safety.h"

#ifndef enSync_Logger_h_included
#define enSync_Logger_h_included

namespace sync {

/**
 * \brief The core logging object for enSync
 *
 * The Logger object uses one or more log outputs to send different types of
 * log messages to logging endpoints. Logging endpoints may be C++ output
 * streams (including the console), files, or some GUI object.
 *
 * Log operations are thread-safe, and the singleton Logger object is
 * guaranteed to exist from the first time it is used until the end of the
 * process.
 *
 * This class is instantiated the first time it is used. To enable logging,
 * at least one log must be added with a logging type (using the `add`
 * method. For very low-level logs (e.g. error logs) this should be done as
 * early as possible.
 *
 * See the \ref logging page for an overview of logging in enSync.
 *
 * \todo A code example
 */
class Logger final
{

    // --- External Interface ---

public:

    /**
     * \brief Retrieve the single instance of the Logger
     *
     * \return The single Logger instance
     */
    static Logger& instance(void);

    // --- Internal Declarations ---

private:

    /**
     * Standard constructor, only accessed by the `instance` method
     */
    Logger(void);

    /**
     * \brief A single mutex for protecting logging operations
     */
    mutable ::sync::mutex m_mutex;

    // Disable copy semantics
    Logger(const Logger&) = delete;
    Logger operator=(const Logger&) = delete;

};  // end Logger class

}   // end sync namespace

#endif

/**
 * \page logging Logging in enSync
 *
 * Logging in this libary is implemented using a singleton object (the only
 * singleton!) accessed by all components that require logging services. The
 * core logging functionality is atomic, thread-safe, and relies only on core
 * functionality.
 *
 * The logging functionality is intended to be light-weight, and usable for
 * many purposes. These purposes include:
 *
 *  *   Error reporting
 *  *   Status reporting
 *  *   Debugging
 *
 * Where logging is turned off, logging operations have a low (non-zero)
 * cost.
 */

/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */



#ifndef enSync_error_h_included
#define enSync_error_h_included

namespace sync {
}   // end sync namespace

/**
 * \page errors Error Handling in enSync
 *
 * Signalling error conditions in enSync is done using exceptions that
 * reference coded string resource tables (for language independence). An
 * enSync error exception includes the following elements:
 *
 *  *   An integer error code
 *  *   Additional, error-specific data (encoded in derived classes)
 *  *   A `message` method that returns a human-readable description of the
 *      problem as a wide-string
 *
 * It is worth noting that the `message` is always formed 'on demand' from
 * string fragments provided by the string table.
 */

#endif

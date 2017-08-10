/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <string>
#include <vector>

#ifndef enSync_messages_h_included
#define enSync_messages_h_included

namespace sync {

/**
 * \brief A set of enumeration codes for the messages table
 *
 * The messages table and this enumeration *always* should be updated in
 * parallel.
 */
enum class message_code
{
    unspec_error = 0    ///< A generic, unspecified error message
};  // end 

/**
 * \brief Retrieve the string message code associated with a given message
 *
 * \param mc The message code enumerator
 *
 * \return The message string 
 */
extern const std::wstring& message(message_code mc);

}   // end sync namespace

#endif

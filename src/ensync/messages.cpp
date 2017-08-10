/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "messages.h"

namespace sync {

/**
 * \brief Raw wide-string messages for use throughout enSync
 *
 * *These strings should be the only text that a User sees!* Don't update
 * the position of strings in this table without also updating the
 * `messages_code` enum.
 */
const std::vector<std::wstring> messages(
{
    L"unspecified error"        // 0 - unspec_error
}); // end messages vector

const std::wstring& message(message_code mc)
{
    return messages[static_cast<std::size_t>(mc)];
}   // end message function

}   // end sync namespace

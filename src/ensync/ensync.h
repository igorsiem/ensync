/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "error.h"
#include "logger.h"
#include "runtime_assertion.h"
#include "strutils.h"

#ifndef _ensync_ensync_h_included
#define _ensync_ensync_h_included

/**
 * \brief All declarations related to the core enSync library
 */
namespace ensync {

/**
 * \brief Hard version string for the ensync library
 */
const wchar_t* version = L"0.0";

}   // end ensync namespace

#endif

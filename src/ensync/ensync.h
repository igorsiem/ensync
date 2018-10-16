/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "atomic_value.h"
#include "compound_type_id.h"
#include "error.h"
#include "logger.h"
#include "runtime_assertion.h"
#include "string_table.h"
#include "strutils.h"
#include "type_id.h"
#include "uuid.h"

#ifndef _ensync_ensync_h_included
#define _ensync_ensync_h_included

/**
 * \brief All declarations related to the core enSync library
 */
namespace ensync {

/**
 * \brief Hard version string for the ensync library
 */
extern const wchar_t* version;

}   // end ensync namespace

#endif

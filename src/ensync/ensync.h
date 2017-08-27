/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "atom.h"
#include "attribute.h"
#include "error.h"
#include "id.h"
#include "logger.h"
#include "messages.h"
#include "node.h"
#include "sqlite-wrapper.h"
#include "strutils.h"

/**
 * \mainpage enSync Library Documentation
 *
 * This is the low-level documentation for *enSync*, a generic, open C++
 * library for performing data syncronisation operations. See the `README.md`
 * file and the documentation in the `/docs/high-level` directory for an
 * overview of Project.
 *
 * \todo Expand on this intro, with some simple code samples
 */

/**
 * \page corealgorithm The Core enSync Algorithm
 *
 * Data synchronisation is impemented based on the following essential
 * scheme:
 *
 *  1.  The data to be synchronised is modelled as a set of items (e.g.
 *      fields in database records) whose value changes over time.
 *
 *  2.  Each data item is modelled as an element of an information schema
 *      (the hierarchy of `node` (and derived) objects, and a stream of
 *      of values (`value_point` objects).
 *
 *  3.  Synchronisation is essentially the process of:
 *
 *      a.  Determining the corresponding attributes of each datasource
 *
 *      b.  Reconciling each attribute such that a given point in its value
 *          stream is reproduced on each synchronised datasource.
 *
 * \todo Develop this idea some more
 */
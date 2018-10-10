/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#ifndef _ensync_copy_move_helpers_h_included
#define _ensync_copy_move_helpers_h_included

/**
 * \brief Explicitly declare a default constructor for the given type
 * 
 * \param classname The name of the class
 */
#define EN_DECLARE_DEFAULT_CONSTRUCTOR( classname ) \
    classname(void) = default;

#define EN_DECLARE_DEFAULT_DESTRUCTOR( classname ) \
    ~classname(void) = default;

#define EN_DECLARE_DEFAULT_LIFE_CYCLE( classname ) \
    EN_DECLARE_DEFAULT_CONSTRUCTOR( classname ) \
    EN_DECLARE_DEFAULT_DESTRUCTOR( classname )

#define EN_DECLARE_DEFAULT_COPY( classname ) \
    classname(const classname&) = default; \
    classname& operator=(const classname&) = default;

#define EN_DECLARE_DEFAULT_MOVE( classname ) \
    classname(classname&&) = default; \
    classname& operator=(classname&&) = default;

#define EN_DECLARE_DEFAULT_ALL( classname ) \
    EN_DECLARE_DEFAULT_LIFE_CYCLE( classname ) \
    EN_DECLARE_DEFAULT_COPY( classname ) \
    EN_DECLARE_DEFAULT_MOVE( classname )

#endif

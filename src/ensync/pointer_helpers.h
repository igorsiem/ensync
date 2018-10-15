/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <memory>

#ifndef _ensync_pointer_helpers_h_included
#define _ensync_pointer_helpers_h_included

/**
 * \brief Declare shared pointer types for instances of the given class
 * 
 * \param classname The name of the class
 */
#define EN_DECLARE_SHARED_PTRS_FOR( classname ) \
    using classname##_spr = std::shared_ptr<classname>; \
    using const_##classname##_spr = std::shared_ptr<const classname>;

/**
 * \brief Declare unique pointer types for instances of the given class
 * 
 * \param classname The name of the class
 */
#define EN_DECLARE_UNIQUE_PTRS_FOR( classname ) \
    using classname##_upr = std::unique_ptr<classname>; \
    using const_##classname##_upr = std::unique_ptr<const classname>;

/**
 * \brief Declare shared and unique pointer types for instances of the given
 * class
 * 
 * \param classname The name of the class
 */
#define EN_DECLARE_PTRS_FOR( classname ) \
    EN_DECLARE_SHARED_PTRS_FOR(classname) \
    EN_DECLARE_UNIQUE_PTRS_FOR(classname)

#endif

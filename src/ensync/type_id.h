/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <string>
#include <variant>
#include "atomic_type_id.h"
#include "compound_type_id.h"

#ifndef _ensync_type_id_h_included
#define _ensync_type_id_h_included

namespace ensync {

/**
 * \brief Universal type identifier
 * 
 * Objects of this class identify either an atomic type (as an enumerator) or
 * a compound type (as a UUID). When the object is a compound type UUID that
 * is 'nil', it is considered an 'unknown' type.
 */
using type_id = std::variant<compound_type_id, atomic_type_id>;

/**
 * \brief Determine whether the type is an 'unknown'
 * 
 * The type is considered unknown if it is the compound type pointer that is
 * set to `nullptr`
 * 
 * \return `true` if the type is considered unknown
 */
inline bool is_unknown(const type_id& t)
{
    if ((t.index() == 0) && std::get<0>(t).is_nil()) return true;
    else return false;
}   // end is_unknown method

/**
 * \brief Retrieve a human-readable name for the given type identifier
 * 
 * If the type is unknown, the 'unknown type' message string is returned. If
 * it is an atomic type, then its standard string representation is returned
 * (also from the message table). If it is a compound type, the name of the
 * type is returned.
 * 
 * \param t The type whose name is to be derived
 * 
 * \param cts The store of compound types to use in the case where t is
 * compound
 * 
 * \return A human-readable name for the type; this will be the 'unknown'
 * message string if t is compound and the UUID is not found in the cts
 */
inline std::wstring name_of(const type_id& t, const compound_type_store& cts)
{
    if (is_unknown(t)) return get(message_code_t::unknown_type);
    else if (t.index() == 0)
    {
        auto itr = cts.first.find(std::get<0>(t));
        if (itr == cts.first.end()) return get(message_code_t::unknown_type);
        else return itr->second;
    }
    else return to_wstring(std::get<1>(t));
}   // end name_of function

}   // end ensync namespace

#endif

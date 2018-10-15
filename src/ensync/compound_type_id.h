/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <string>
#include "copy_move_helpers.h"
#include "pointer_helpers.h"
#include "uuid.h"

#ifndef _ensync_compound_type_id_h_included
#define _ensync_compound_type_id_h_included

namespace ensync {

/**
 * \brief An identifier structure for a compound type
 * 
 * Compound types are identified by a UUID that is generated at definition,
 * and also carry a human-readable name that is User-defined. The UUID is
 * intended to uniquely identify a compound type wherever it is relevant. The
 * human-readable name should be unique within a given collection.
 * 
 * Compound types IDs are usually accessed via unique pointers. This strategy
 * has been deliberately chosen to reinforce the idea that each one should be
 * unique, in the same way that atomic type IDs are unique.
 * 
 * \todo Re-examine the idea of uniqueness for compound type IDs
 */
class compound_type_id
{

    public:

    /**
     * \brief Set up a compound type ID with a UUID and human-readable name
     * 
     * \param uuid The UUID of the type
     * 
     * \param name The human-readable name of the type
     */
    compound_type_id(uuid_t uuid, std::wstring name) :
        m_uuid(std::move(uuid))
        , m_name(std::move(name))
    {}

    /**
     * \brief Default constructor - creates an invalid ID with a nil
     * UUID and an empty name
     */
    compound_type_id(void) :
        m_uuid(std::move(make_nil_uuid()))
        , m_name()
    {}

    EN_DECLARE_DEFAULT_DESTRUCTOR(compound_type_id)
    EN_DECLARE_DEFAULT_COPY(compound_type_id)
    EN_DECLARE_DEFAULT_MOVE(compound_type_id)

    /**
     * \brief Determine whether the type is valid
     * 
     * Valid type IDs have a non-nil UUID and a non-empty name
     * 
     * \return `true` if the ID is valid
     */
    bool is_valid(void) const
    {
        if (m_uuid.is_nil()) return false;
        if (m_name.empty()) return false;
        return true;
    }

    /**
     * \brief Retrieve the UUID of the identifier
     * 
     * \return The UUID object
     */
    uuid_t uuid(void) const { return m_uuid; }

    /**
     * \brief Retrieve the human-readable name of the identifier
     * 
     * \return The name
     */
    std::wstring name(void) const { return m_name; }

    protected:

    uuid_t m_uuid;            ///< Unique identifier
    std::wstring m_name;    ///< Human-readable type name

};  // end compound_type_id class

// Unique pointers
EN_DECLARE_UNIQUE_PTRS_FOR(compound_type_id)

/**
 * \brief Generate a new compound type ID as a unique pointer with a
 * randomly-generated UUID
 * 
 * \param name A human-readable name for the type
 */
inline compound_type_id_upr generate_new_compound_type_id(std::wstring name)
{
    return std::make_unique<compound_type_id>(
        make_random_uuid()
        , std::move(name));
}   // end generate_new_compound_type_id function

}   // end ensync namespace

#endif

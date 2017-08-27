/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "node.h"
#include "messages.h"
  
#ifndef enSync_atom_h_include
#define enSync_atom_h_include
 
namespace sync {

/**
 * \brief An abstract base class for atomic data attributes that are
 * associated with a changing value
 *
 * Note that atom objects *describe* the data item, with concepts such as
 * data type, but they do not directly encaspulate a value. They are
 * essentially metadata.
 *
 * This base class is essentially used to provide information about the
 * value at run-time with a generic interface. The derived `attribute`
 * template declares type-specific functionality.
 *
 * Note also that the `::sync::atom` class has nothing to do with the
 * similarly-named `std::atomic` template - don't confuse them!
 */
class atom : public node
{
    // --- External Interterface ---

    public:

    // -- Public Sub-types --

    /**
     * \brief A shared pointer to an atom object
     */
    using atom_ptr = std::shared_ptr<atom>;

    /**
     * \brief A shared pointer to a const atom object
     */
    using const_atom_ptr = std::shared_ptr<const atom>;

    /**
     * \brief Enumerates the atomic data types support by *enSync*
     */
    enum class data_type
    {

        unknown = 0,    ///< Not (yet) determined
        integer,        ///< Integer, any size
        decimal,        ///< Float, double, any size or precision
        string          ///< Wide-string

    };  // end type enum

    // -- Methods --

    /**
     * \brief Trivial constructor
     */
    atom(void) : node() {}

    /**
     * \brief Retrieve the data type of the atom
     */
    virtual data_type type(void) const = 0;
    
    /**
     * \brief Translate a data type enumerator into a message enumerator for
     * a human-readable description of the type
     */
    static message_code to_message_code(data_type dt)
    {
        switch (dt)
        {
            // If we have an unrecognised data type, don't throw an
            // exception - just call it 'unknown'
            default:
            case data_type::unknown:
                return message_code::data_type_unknown;
            case data_type::integer: return message_code::data_type_integer;
            case data_type::decimal: return message_code::data_type_decimal;
            case data_type::string: return message_code::data_type_string;
        }
    }   // end message_code method

    // Disable copy semantics
    atom(const atom&) = delete;
    atom& operator=(const atom&) = delete;
};  // end atom class

/**
 * \brief A shared pointer to an atom object
 */
using atom_ptr = atom::atom_ptr;

/**
 * \brief A shared pointer to a const atom object
 */
using const_atom_ptr = atom::const_atom_ptr;

/**
 * \brief A collection of atoms, indexed by ID
 */
using atom_map_by_id = std::map<id, atom_ptr>;

/**
 * \brief A collection of const atoms, indexed by ID
 */
using const_atom_map_by_id = std::map<id, const_atom_ptr>;

}   // end sync namespace 

#endif

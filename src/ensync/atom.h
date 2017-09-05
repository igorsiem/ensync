/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "node.h"
#include "messages.h"
#include "data-type.h"
#include "value-point.h"
  
#ifndef enSync_atom_h_include
#define enSync_atom_h_include
 
namespace sync {

/**
 * \brief An abstract base class for atomic data attributes that are
 * associated with a changing value
 *
 * Note that atom objects *describe* the data item, with concepts such as
 * data type, but they do not directly encapsulate a value. They are
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
     * \brief Create a new value-point object for the attribute's current
     * value
     *
     * This is a type-generic way of retrieving an actual data value from the
     * repository, which is implemented in derived classes.
     *
     * Note that this method is intentionally *not* declared `const`. This is
     * because the attribute interface is implemented in a
     * repository-specific way, and some repositories may conceivably have
     * their state modified by accessing their value points.
     */
    virtual value_point_ptr generate_new_value_point(void) = 0;
    
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

/**
 * \brief A collection of atoms, indexed by their full ID path string
 *
 * We acknowledge that, because IDs are themselves a string, this is no
 * different from the `atom_map_by_id` definition. The difference is purely
 * cosmetic, and intended for completeness and readability.
 */
using atom_map_by_full_id_path_string = std::map<id_path_string, atom_ptr>;

/**
 * \brief A collection of const atoms, indexed by their full ID path string
 *
 * We acknowledge that, because IDs are themselves a string, this is no
 * different from the `const_atom_map_by_id` definition. The difference is
 * purely cosmetic, and intended for completeness and readability.
 */
using const_atom_map_by_full_id_path_string =
    std::map<id_path_string, const_atom_ptr>;
 
/**
 * \brief Generate a collection of value points from the given collection of
 * atoms
 *
 * \param atoms The collection of atoms; note that these are *not* const
 * because the `generate_new_value_point` operation is not declared const
 * (intentionally)
 *
 * \param value_points The container into which the values points will be
 * placed; note that this is *not* emptied prior to filling
 *
 * \return A reference to `value_points` 
 */
extern value_point_map_by_attribute_id_path_string& generate_value_points(
        atom_map_by_full_id_path_string& atoms,
        value_point_map_by_attribute_id_path_string& value_points);

}   // end sync namespace 

#endif

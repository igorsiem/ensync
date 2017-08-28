/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "atom.h"
  
#ifndef enSync_attribute_h_included
#define enSync_attribute_h_included
 
namespace sync {

/**
 * \brief A typed data atom, describing a changing value
 *
 * The `attribute` template class adds type-specific information and
 * functionality to the generic `atom` interface.
 * 
 */
template <typename T>
class attribute : public atom
{

    // --- External Interface ---

    public:

    // -- Public Sub-types --

    /**
     * \brief A shared pointer to an attribute
     */
    using attribute_ptr = std::shared_ptr<attribute>;

    /**
     * \brief A shared pointer to a const attribute
     */
    using const_attribute_ptr = std::shared_ptr<const attribute>;

    // -- Methods --

    /**
     * \brief Trivial constructor
     */
     attribute(void) : atom() {}

    // virtual data_type type(void) const = 0;
     
    // Disable copy semantics
    attribute(const attribute&) = delete;
    attribute& operator=(const attribute&) = delete;

};  // end attribute class

/**
 * \brief A shared pointer to an attribute
 */
template <typename T>
using attribute_ptr = std::shared_ptr<attribute<T> >;

/**
 * \brief A shared pointer to a const attribute
 */
template <typename T>
using const_attribute_ptr = std::shared_ptr<const attribute<T> >;

// --- Attribute Specialisations ---

/**
 * \brief An integer attribute specialisation
 */
template <>
class attribute<int> : public atom
{   
    public:

    /**
     * \brief Retrieve the attribute type (integer)
     */
    virtual data_type type(void) const override
        { return data_type::integer; }
};  // end attribute<int> class

/**
 * \brief A double attribute specialisation
 */
template <>
class attribute<double> : public atom
{   
    public:
 
    /**
     * \brief Retrieve the attribute type (decimal)
     */
    virtual data_type type(void) const override
        { return data_type::decimal; }

};  // end attribute<double> class

/**
 * \brief A (wide) string attribute specialisation
 */
template <>
class attribute<std::wstring> : public atom
{   
    public:
 
    /**
     * \brief Retrieve the attribute type (string)
     */
    virtual data_type type(void) const override
        { return data_type::string; }

};  // end attribute<double> class
 
}  // end sync namespace

#endif

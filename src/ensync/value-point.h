/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <memory>
#include <atomic>
#include <map>
#include "data-type.h"
#include "thread-safety.h"
#include "id.h"

#ifndef enSync_value_point_h_included
#define enSync_value_point_h_included
 
namespace sync {

/**
 * \brief A base class for all value point types in *enSync*
 *
 * The `value_point` class declares the 'type independent' interface to
 * values in *enSync*. Instances (as shared pointers) can be placed in
 * containers and manipulated generically.
 *
 * The `value_data` template declares and implements type-specific
 * functionality.
 *
 * \todo Expand on this documentation
 */
class value_point : public std::enable_shared_from_this<value_point>
{

    // --- External Interface ---

    public:

    // -- Public Sub-types --

    /**
     * \brief A shared pointer to a value point
     */
    using value_point_ptr = std::shared_ptr<value_point>;

    /**
     * \brief A shared pointer to a const value point
     */
    using const_value_point_ptr = std::shared_ptr<const value_point>;

    // -- Methods --

    /**
     * \brief Retrieve the data type of the value
     */
    virtual data_type type(void) const = 0;

    /**
     * \brief Retrieve the ID path of the attribute for this value
     */
    virtual const id_path& attribute_id_path(void) const;

    /**
     * Create a new value_point, based on a given data type
     */
    // template <typename T>
    // static value_point_ptr create_new(const T& data);

    // Remove copy semantics
    value_point(const value_point&) = delete;
    value_point& operator=(const value_point&) = delete;

    // --- Internal Declarations ---

    protected:

    /**
     * \brief A mutex for thread-protection
     *
     * Note that derived classes may or may not use this mutex for protecting
     * additional attributes.
     */
    mutable mutex m_mutex;

    /**
     * \brief The ID path of the attribute associated with this value
     */
    id_path m_attribute_id_path;

    /**
     * Constructor, initialising all attributes
     *
     * \param attribute_id_path The ID path of the attribute associated with
     * this value point
     */
    explicit value_point(const id_path& attribute_id_path) :
        std::enable_shared_from_this<value_point>(),
        m_mutex(),
        m_attribute_id_path(attribute_id_path)
    {}

};  // end value_point class

/**
 * \brief A shared pointer to a value point
 */
using value_point_ptr = value_point::value_point_ptr;
     
/**
 * \brief A shared pointer to a const value point
 */
using const_value_point_ptr = value_point::const_value_point_ptr;

/**
 * \brief A collection of value points, indexed by the ID path of their
 * attribute
 */
using value_point_map_by_attribute_id_path_string =
    std::map<id_path_string, value_point_ptr>;

/**
 * \brief A template implementing the value_point interface for specific
 * data types
 *
 * The generic type does not implement any functionality directly, but
 * type-specific specialisations are supplied that take care of:
 *
 *  *   Instantiation
 *  *   Thread-safe access to value data
 *
 * These functions are not implemented in the generic interface, because we
 * want to take advantage of some tricks like atomics for integral types.
 */
template <typename T>
class value_data : public value_point
{

    // --- External Interface ---

public:

    // Disable copy semantics
    value_data(const value_data&) = delete;
    value_data& operator=(const value_data&) = delete;

};  // end value_data template

/**
 * \brief A shared pointer to a value_data object
 */
template <typename T>
using value_data_ptr = std::shared_ptr<value_data<T> >;

/**
 * \brief Integer instantiation of the value_data template
 */
template <>
class value_data<int> : public value_point
{

    // --- External Interface ---

    public:

    // -- Sub-types --

    /**
     * \brief A shared pointer to a integer value data object
     */
    using int_value_data_ptr = std::shared_ptr<value_data>;

    /**
     * \brief A shared pointer to a const integer value data object
     */
     using const_int_value_data_ptr = std::shared_ptr<value_data>;
     
    // -- Methods --

    /**
     * \brief Create a new value data object as a shared pointer
     */
    static int_value_data_ptr create_new(
            const id_path& attribute_id_path,
            int data)
    {
        return int_value_data_ptr(new value_data(attribute_id_path, data));
    }

    /**
     * \brief Retrieve the data type of the value
     */
     virtual data_type type(void) const override
        { return data_type::integer; }

    /**
     * \brief Retrieve the data value
     */
    int data(void) const { return m_data.load(); }

    // --- Internal Declarations ---

    protected:

    /**
     * \brief Constructor, initialising the data
     *
     * \param attribute_id_path The ID path of the attribute associated with
     * this value point
     *
     * \param d The integer data value
     */
    value_data(
        const id_path& attribute_id_path,
        int data) : value_point(attribute_id_path), m_data(data) {}

    /**
     * \brief The integer data value, as an atomic
     */
    std::atomic<int> m_data;

};  // end value_data<int> class

/**
 * \brief The value data template specialised for integers
 */
using int_value_data = value_data<int>;

/**
 * \brief A shared pointer to a integer value data object
 */
using int_value_data_ptr = std::shared_ptr<int_value_data>;
     
/**
 * \brief A shared pointer to a const integer value data object
 */
using const_int_value_data_ptr = std::shared_ptr<const int_value_data>;

/**
 * \brief A `double` implementation of the value_data template
 */
template <>
class value_data<double> : public value_point
{

    // --- External Interface ---

    public:

    // -- Sub-types --

    /**
     * \brief A shared pointer to a double value data object
     */
    using double_value_data_ptr = std::shared_ptr<value_data>;

    /**
     * \brief A shared pointer to a const double value data object
     */
     using const_double_value_data_ptr = std::shared_ptr<value_data>;
     
    // -- Methods --

    /**
     * \brief Create a new value data object as a shared pointer
     */
    static double_value_data_ptr create_new(
            const id_path& attribute_id_path,
            double data)
    {
        return double_value_data_ptr(
            new value_data(attribute_id_path, data));
    }   // end create_new method

    /**
     * \brief Retrieve the data type of the value
     */
     virtual data_type type(void) const override
        { return data_type::decimal; }

    /**
     * \brief Retrieve the data value
     */
     double data(void) const { return m_data.load(); }

    // --- Internal Declarations ---

    protected:

    /**
     * \brief Constructor, initialising the data
     *
     * \param attribute_id_path The ID path of the attribute associated with
     * this value point
     *
     * \param d The double data value
     */
    value_data(
        const id_path& attribute_id_path,
        double data) : value_point(attribute_id_path), m_data(data) {}

    /**
     * \brief The double-precision data value, as an atomic
     */
    std::atomic<double> m_data;

};  // end value_data<int> class

/**
 * \brief The value_data template, specialised for doubles
 */
using double_value_data = value_data<double>;

/**
 * \brief A shared pointer to a integer value data object
 */
using double_value_data_ptr = std::shared_ptr<double_value_data>;
     
/**
 * \brief A shared pointer to a const integer value data object
 */
using const_double_value_data_ptr = std::shared_ptr<const double_value_data>;

/**
 * \brief A `string` implementation of the value_data template
 *
 * Note that this instantiation of the template uses a mutex to protect the
 * internal string data, instread of an atomic (as with integers).
 */
template <>
class value_data<std::wstring> : public value_point
{
 
     // --- External Interface ---
 
     public:
 
     // -- Sub-types --
 
     /**
      * \brief A shared pointer to a string value data object
      */
    using wstring_value_data_ptr = std::shared_ptr<value_data>;
 
     /**
      * \brief A shared pointer to a const wstring value data object
      */
    using const_wstring_value_data_ptr = std::shared_ptr<value_data>;
      
     // -- Methods --
 
     /**
      * \brief Create a new value data object as a shared pointer
      */
    static wstring_value_data_ptr create_new(
            const id_path& attribute_id_path,
            const std::wstring& data)
    {
        return wstring_value_data_ptr(
            new value_data(attribute_id_path, data));
    }   // end create_new method
 
    /**
     * \brief Retrieve the data type of the value
     */
    virtual data_type type(void) const override
        { return data_type::string; }
 
     /**
      * \brief Retrieve the data value
      */
    const std::wstring& data(void) const
    {
        read_lock rl(m_data_mutex);
        return m_data;
    }
 
    // --- Internal Declarations ---

    protected:
 
     /**
      * \brief Constructor, initialising the data
      *
      * \param attribute_id_path The ID path of the attribute associated with
      * this value point
      *
      * \param d The double data value
      */
    value_data(
           const id_path& attribute_id_path,
           const std::wstring& data) :
       value_point(attribute_id_path),
       m_data_mutex(),
       m_data(data) {}

    /**
     * \brief The mutex protecting multi-threaded access to the string data
     */
    mutable mutex m_data_mutex;
 
     /**
      * \brief The string data value, as an atomic
      */
    std::wstring m_data;
 
 };  // end value_data<int> class
 
/**
 * \brief The value data template, specialised for strings
 */
using wstring_value_data = value_data<std::wstring>;

 /**
  * \brief A shared pointer to a integer value data object
  */
 using wstring_value_data_ptr = std::shared_ptr<wstring_value_data>;
      
 /**
  * \brief A shared pointer to a const integer value data object
  */
 using const_wstring_value_data_ptr =
    std::shared_ptr<const wstring_value_data>;
 
}   // end sync namespace
    
#endif

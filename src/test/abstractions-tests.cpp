/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

// Test implementation of an attribute template
template <typename T>
class test_attribute : public sync::attribute<T>
{

    public:

    // A shared pointer to a test attribute object
    using test_attribute_ptr = std::shared_ptr<test_attribute>;

    // A function for deriving the value of the attribute at any given time -
    // this allows us to test value generation
    using value_fn = std::function<T(void)>;

    // Create a new test attribute object
    static test_attribute_ptr create_new(
            const sync::id& i,
            const sync::id_path& pidp,
            value_fn get_fn)
    {
        return test_attribute_ptr(new test_attribute(i, pidp, get_fn));
    }

    // Generate a new value_data object for the attribute
    virtual sync::value_data_ptr<T> generate_new_value_data(void) override
    {
        return sync::value_data<T>::create_new(
            this->full_id_path(),
            get_value());
    }

    // Retrieve the ID of the attribute
    virtual sync::id id(void) const override { return m_id; }

    // Retrieve the ID of the parent of the attribute
    virtual sync::id_path parent_id_path(void) const
        { return m_parent_id_path; }

    protected:

    // Constructor, initialising all members - this is called by the
    // `create_new` method
    test_attribute(
            const sync::id& i,
            const sync::id_path& pidp,
            value_fn get_fn) :
        sync::attribute<T>(),
        m_id(i),
        m_parent_id_path(pidp),
        get_value(get_fn)
    {}
        
    // The local ID of the attribute
    sync::id m_id;

    // The path of the parent of the attribute
    sync::id_path m_parent_id_path;

    // The value function for the attribute
    value_fn get_value;
    
};  // end test_attribute

// Convenience definitions for macro instantiations
using test_int_attribute = test_attribute<int>;
using test_double_attribute = test_attribute<double>;
using test_string_attribute = test_attribute<std::wstring>;

// This test case demonstrates some basic derivation of the atom / attribute
// interfaces.
TEST_CASE("Operations on attributes and value data")
{
    // Test basic type reporting.
    auto int_attr = test_int_attribute::create_new(
            L"int_attr",
            sync::id_path({L"parent"}),
            [](){ return 2; });
    auto double_attr = test_double_attribute::create_new(
            L"double_attr",
            sync::id_path({L"parent"}),
            [](){ return 2.2; });
    auto string_attr = test_string_attribute::create_new(
            L"string_attr",
            sync::id_path({L"parent"}),
            [](){ return L"2"; });
            
    // Verify all attribute objects were created successfully.
    REQUIRE(int_attr != nullptr);
    REQUIRE(double_attr != nullptr);
    REQUIRE(string_attr != nullptr);

    // Verify that attributes all report their types correctly.
    REQUIRE(int_attr->type() == sync::data_type::integer);
    REQUIRE(double_attr->type() == sync::data_type::decimal);
    REQUIRE(string_attr->type() == sync::data_type::string);

    // Verify that attributes all report their full ID path correctly.
    REQUIRE(int_attr->full_id_path() ==
        sync::id_path({L"parent", L"int_attr"}));
    REQUIRE(double_attr->full_id_path() ==
        sync::id_path({L"parent", L"double_attr"}));
    REQUIRE(string_attr->full_id_path() ==
        sync::id_path({L"parent", L"string_attr"}));

    // Verify that an attribute can create its corresponding value data.
    auto int_vd = int_attr->generate_new_value_data();
    auto double_vd = double_attr->generate_new_value_data();
    auto string_vd = string_attr->generate_new_value_data();

    // Value data objects created successfully
    REQUIRE(int_vd != nullptr);
    REQUIRE(double_vd != nullptr);
    REQUIRE(string_vd != nullptr);

    // Value data objects have their attributes' ID path
    REQUIRE(int_vd->attribute_id_path() == int_attr->full_id_path());
    REQUIRE(double_vd->attribute_id_path() == double_attr->full_id_path());
    REQUIRE(string_vd->attribute_id_path() == string_attr->full_id_path());

    // Value data object report their type correctly
    REQUIRE(int_vd->type() == sync::data_type::integer);
    REQUIRE(double_vd->type() == sync::data_type::decimal);
    REQUIRE(string_vd->type() == sync::data_type::string);
    
    // Value data objects have the anticipated values
    REQUIRE(int_vd->data() == 2);
    REQUIRE(double_vd->data() == 2.2);
    REQUIRE(string_vd->data() == L"2");

}   // end abstraction operations test case

// Group operations on atom.
TEST_CASE("Group operations on atoms")
{
    // Create some attributes, and put them into an atom collection.
    auto int_attr = test_int_attribute::create_new(
        L"int_attr",
        sync::id_path({L"parent"}),
        [](){ return 3; });
    auto double_attr = test_double_attribute::create_new(
        L"double_attr",
        sync::id_path({L"parent"}),
        [](){ return 3.3; });
    auto string_attr = test_string_attribute::create_new(
        L"string_attr",
        sync::id_path({L"parent"}),
        [](){ return L"3"; });

    sync::atom_map_by_full_id_path_string atoms;

    atoms[sync::to_id_path_string(int_attr->full_id_path())] = int_attr;
    atoms[sync::to_id_path_string(double_attr->full_id_path())] =
        double_attr;
    atoms[sync::to_id_path_string(string_attr->full_id_path())] =
        string_attr;

    // Generate the value points in one hit, and check them.
    sync::value_point_map_by_attribute_id_path_string values;
    REQUIRE_NOTHROW(sync::generate_value_points(atoms, values));

    REQUIRE(
        std::dynamic_pointer_cast<sync::int_value_data>(
            values[L"parent/int_attr"])->data() == 3);
    REQUIRE(
        std::dynamic_pointer_cast<sync::double_value_data>(
            values[L"parent/double_attr"])->data() == 3.3);
    REQUIRE(
        std::dynamic_pointer_cast<sync::wstring_value_data>(
            values[L"parent/string_attr"])->data() == L"3");
                
}   // end Group operations on atoms test

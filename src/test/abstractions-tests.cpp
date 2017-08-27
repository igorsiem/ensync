/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <CATCH/catch.hpp>
#include <ensync/ensync.h>

template <typename T>
class test_attribute : public sync::attribute<T>
{
    public:

    using test_attribute_ptr = std::shared_ptr<test_attribute>;

    static test_attribute_ptr create_new(
            const sync::id& i,
            const sync::id_path& pidp)
    {
        return test_attribute_ptr(new test_attribute(i, pidp));
    }

    virtual sync::id id(void) const override { return m_id; }
    virtual sync::id_path parent_id_path(void) const
        { return m_parent_id_path; }

    protected:

    test_attribute(const sync::id& i, const sync::id_path& pidp) :
        sync::attribute<T>(), m_id(i), m_parent_id_path(pidp) {}
        
    sync::id m_id;
    sync::id_path m_parent_id_path;
    
};  // end test_int_attribute

using test_int_attribute = test_attribute<int>;
using test_double_attribute = test_attribute<double>;
using test_string_attribute = test_attribute<std::wstring>;

// This test case demonstrates some basic derivation of the atom / attribute
// interfaces.
TEST_CASE("Operations on atoms and attributes")
{
    // Test basic type reporting.
    //
    // TODO We don't have any ownership or compound structures here.
    auto int_attr = test_int_attribute::create_new(
            L"int_attr",
            sync::id_path({}));
    auto double_attr = test_double_attribute::create_new(
            L"double_attr",
            sync::id_path({}));
    auto string_attr = test_string_attribute::create_new(
            L"string_attr",
            sync::id_path({}));
    
    // Verify all attribute objects were created successfully.
    REQUIRE(int_attr != nullptr);
    REQUIRE(double_attr != nullptr);
    REQUIRE(string_attr != nullptr);

    // Veridy that attribute all report their types correctly.
    REQUIRE(int_attr->type() == sync::atom::data_type::integer);
    REQUIRE(double_attr->type() == sync::atom::data_type::decimal);
    REQUIRE(string_attr->type() == sync::atom::data_type::string);

}   // end abstraction operations test case

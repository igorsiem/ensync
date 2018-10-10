/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <string>
#include <codecvt>
#include <boost/lexical_cast.hpp>

#ifndef _ensync_strutils_h_included
#define _ensync_strutils_h_included


namespace boost {

/// \cond
//
// The following declarations are some internal type manipulations to
// support the lexical cast operations
namespace ensync_internal {

using convert_type = std::codecvt<wchar_t, char, mbstate_t>;

template <class facet_base>
struct deletable_facet : public facet_base
{
    template <class ...arg_types>
    deletable_facet(arg_types&& ...args) :
        facet_base(std::forward<arg_types>(args)...) {}
        
    virtual ~deletable_facet(void) {}
};

}
/// \endcond

using namespace ensync_internal;

/**
 * \brief Convert a wide string to a string, as an extension to Boost's
 * `lexical_cast` system
 * 
 * \param arg The wide string to convert
 * 
 * \return The  string string version of the wide string
 */
template<>
std::string lexical_cast(const std::wstring& arg)
{
    std::wstring_convert<deletable_facet<convert_type> > string_converter;
    return string_converter.to_bytes(arg);
}   // end lexical cast

/**
 * \brief Convert a string to a wide string, as an extension to Boost's
 * `lexical_cast` system
 * 
 * \param arg The string to convert
 * 
 * \return The wide string version of the string
 */
template<>
std::wstring lexical_cast(const std::string& arg)
{
    std::wstring_convert<deletable_facet<convert_type> > string_converter;
    return string_converter.from_bytes(arg);
}   // end lexical cast

}   // end boost namespace

#endif

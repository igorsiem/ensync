/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "strutils.h"

namespace boost {

template<>
std::string lexical_cast(const std::wstring& arg)
{
    std::wstring_convert<deletable_facet<convert_type> > string_converter;
    return string_converter.to_bytes(arg);
}   // end lexical cast

template<>
std::wstring lexical_cast(const std::string& arg)
{
    std::wstring_convert<deletable_facet<convert_type> > string_converter;
    return string_converter.from_bytes(arg);
}   // end lexical cast

}   // end boost

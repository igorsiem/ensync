/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "strutils.h"

namespace sync {

template<>
std::string& strutils::to_string<std::wstring>(
		std::string& str,
		const std::wstring& wstr)
{
    std::wstring_convert<deletable_facet<convert_type> >
        string_converter;
    str = string_converter.to_bytes(wstr);
    return str;
}	// end to_string function

template <>
std::wstring& strutils::to_wstring<std::string>(
		std::wstring& wstr,
		const std::string& str)
{
    std::wstring_convert<deletable_facet<convert_type> >
        string_converter;
    wstr = string_converter.from_bytes(str);
    return wstr;
}   //end to_wstring function

}   // end sync namespace

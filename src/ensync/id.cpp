/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include "id.h"
#include "strutils.h"

namespace sync {

const wchar_t id_separator = L'/';

bool is_valid_id(const id& str)
{
    // All we do is check for the presence of the separator.
    if (str.find_first_of(id_separator) == std::wstring::npos)
        return true;
    else return false;
}   // end is_valid_id
    
id_path_string& to_id_path_string(
        id_path_string& str,
        const id_path& idp)
{
    std::wstring delim;
    delim += id_separator;
    return strutils::concatenate(str, idp, delim);
}   // end 

id_path_string to_id_path_string(const id_path& idp)
{
    id_path_string result;
    return to_id_path_string(result, idp);
}   // end to_id_path_string function

id_path& to_id_path(id_path& idp, const id_path_string& str)
{
    return strutils::split(idp, str, id_separator);
}   // end to_id_path function

id_path to_id_path(const id_path_string& str)
{
    id_path idp;
    return to_id_path(idp, str);
}   // end to_id_path function 

}   // end sync namespace

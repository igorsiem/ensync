/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <vector>
#include <string>

#ifndef enSync_id_h_included
#define enSync_id_h_included

namespace sync {

/**
 * \brief A common identifier for use in datasources, attributes, records,
 * etc.
 *
 * Identifiers distinguish the entity within a collection. IDs may be
 * concatenated with a separator to describe hierarchical relationships (e.g.
 * <datasource>/<table>/<record>/<attribute>).
 *
 * IDs are subject to the rules imposed by the `is_valid_id` method.
 */
using id = std::wstring;

/**
 * \brief The single character (L'/') that is used to separate enSync
 * identifiers
 */
extern const wchar_t id_separator;

/**
 * \brief Determine whether the given string is valid for use as an enSync
 * ID.
 *
 * The only rule for a string being a valid ID is that it does not include
 * the `id_separator` character.
 *
 * \param str The string to check
 *
 * \return `true` if the string is a valid ID
 */
extern bool is_valid_id(const id& str);

/**
 * \brief A sequence of IDs comprising a 'path' to an entity
 */
using id_path = std::vector<id>;

/**
 * \brief An ID path in string form (just a string concatenated separators)
 *
 * Of course, an id_path_string is the same as an id, but we make the
 * distinction to aid with code readability.
 */
using id_path_string = std::wstring;

/**
 * \brief Convert an id_path to an id_path_string
 *
 * \param idp The id_path to convert
 *
 * \oaram str The path string into which the fragments will be concatenated
 *
 * \return A reference to `str`
 */
extern id_path_string& to_id_path_string(
    id_path_string& str,
    const id_path& idp);

/**
 * \brief Convert an id_path to an id_path_string (using a temporary)
 *
 * \param idp The id_path to convert
 *
 * \return The concatenated path string
 */
extern id_path_string to_id_path_string(const id_path& idp);

/**
 * \brief Split an id_path_string into a id_path vector
 *
 * \param idp The ID path vector into which the string path will be split
 * (cleared before being filled)
 *
 * \param str The string path to convert
 *
 * \return A reference to `idp`
 */
extern id_path& to_id_path(id_path& idp, const id_path_string& str);

/**
 * \brief Split an id_path_string into a id_path vector (using a temporary)
 *
 * \param str The string path to convert
 *
 * \return A reference to `idp`
 */
 extern id_path to_id_path(const id_path_string& str);
 
}   // end sync namespace

#endif

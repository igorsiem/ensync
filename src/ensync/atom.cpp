/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

 #include "atom.h"
 
namespace sync {

value_point_map_by_attribute_id_path_string& generate_value_points(
        atom_map_by_full_id_path_string& atoms,
        value_point_map_by_attribute_id_path_string& value_points)
{
    for (auto a : atoms)
        value_points[to_id_path_string(a.second->full_id_path())] =
            a.second->generate_new_value_point();

    return value_points;
}   // end generate_value_points function

}   // end namespace sync

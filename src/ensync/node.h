/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <memory>
#include <string>
#include <map>
#include "id.h"
 
#ifndef enSync_node_h_include
#define enSync_node_h_include

namespace sync {

/**
 * \brief A pure abstract base class for all metadata items in enSync
 *
 * Nodes are arranged in a hierarchy of atomic data items and compounds
 * (which are derived classes). These nodes describe the information
 * structure published by a datasource.
 *
 * The `node` class (and its derived classes) declare a generic interface
 * to the core *enSync* synchronisation engine. Concrete subclasses of these
 * generic interfaces are implemented for each type of datasource, and the
 * instantiated node-derived objects encapsulate specific data schemata.
 *
 * All nodes (and derived class objects) are instantiated using a static
 * `create_new` method, which returns a shared pointer. Nodes are not
 * copyable, but they do have an identifier and an optional 'owner'.
 */
class node
{

    // --- External Interface ---

    public:

    // -- Public Sub-types --

    /**
     * \brief A shared pointer to a node
     */
    using node_ptr = std::shared_ptr<node>;

    /**
     * \brief A shared pointer to a const node
     */
    using const_node_ptr = std::shared_ptr<const node>;

    /**
     * \brief Retrieve the identifier of the node
     */
    virtual sync::id id(void) const = 0;

    /**
     * \brief Retrieve the ID path of the parent of this node (may be empty)
     */
    virtual id_path parent_id_path(void) const = 0;

    /**
     * \brief Retrieve the fully-qualified ID path for the node
     *
     * This is implemented composing the parent ID path with the node ID
     */
    virtual id_path full_id_path(void) const
    {
            id_path idp = parent_id_path();
            idp.push_back(id());
            return idp;
    }   // end full_id_path

    // -- Methods --

    /**
     * \brief Trivial constructor
     */
    node(void) {}

    // Disable copy semantics
    node(const node&) = delete;
    node& operator=(const node&) = delete;

};  // end node class

/**
 * \brief A shared pointer to a node object
 */
using node_ptr = node::node_ptr;

/**
 * \brief A shared pointer to a const node object
 */
using const_node_ptr = node::const_node_ptr;

/**
 * \brief A collection of nodes, indexed by local ID
 */
using node_map_by_id = std::map<id, node_ptr>;

/**
 * \brief A collection of const nodes, indexed by local ID
 *
 * It is worth noting that the objects are const in this collection, but the
 * collection itself is not.
 */
using const_node_map_by_id = std::map<id, const_node_ptr>;

}   // end sync namespace

#endif
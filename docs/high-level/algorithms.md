# enSync Algorithms

## Introduction

TBD

## Atomic Syncronisation

The *enSync* synchronisation algorithm is based on the concept of atomic data
attributes, each of which is synchronised independently. It uses the
following model and assumptions:

* A group of *n* repositories containing *m* atomic attribute definitions in
  common -- This loses no generality in the case where different repositories
  contain different numbers of attributes. In this case, we refer to the *m*
  attributes that the respositories being synchronised have in common.
  Repositories may be synchronised in multiple groups.

* Each attribute describes an atomic item of data in each repository, with a
  *value* that changes over time. Attribute values are synchronised according
  to the following definitions:

  * Some repositories are synchronisation *sources* - changes to the
    attribute in these repositories are emitted to other repositories.

  * Some repositories are synchronisation *sinks* - they consume changes to
    the attribute emitted by synchronisation sources.

  * A repository may be a source for some of its attributes and sink for
    others.

  * A repository may be both a source and a sink for a given attribute.

The synchronisation algorithm is described as follows:

1. A sink repository enters into a synchronisation relationship with a
    source repository at a given point in time. At that time:

  a. The attribute's current value is recorded as a *value point*, which is
     comprised of:

    * An identifier for the value point - this must be unique with the the
      value points of the source repository, and part of a monotonically
      increasing sequence of identifiers.

      *Note that the same value point identifier may be used for multiple
      attributes and multiple sinks being checked at the same time, provided
      that the values of the attributes in question will not change in the
      repository during the processing of the request. The value point
      identifier is essentially a 'bookmark' in the stream of values of the
      synchronised attributes at that point in time.*

    * An identifier for the attribute being synchronised

    * An identifier for the sink repository that has requested the
      synchronisation

    * The current attribute value itself

    The value point object is stored and owned by the source repository.

  b. The sink retains the value point identifier, along with an identifier
     for the source repository. The sink stores the emitted value from the
     source, unless it already has a value for the attribute, and the
     value is identical to that of the source.

2. Subsequent to entering into a synchronisation relationship, a sink may
   request a synchronisation update from a source. It submits the most
   recent value point identifier received from the source with the request.
   This is handled as follows:

    a. The source checks its stored value point identifiers, and locates the
       value points that have been associated with that identifier *for
       the requesting sink*.

    b. New value point objects are created in the same manner as in step
       (1), for all the attributes associated with the value points in step
       (2.a). These are 'tagged' with a new value point identifier from the
       source repository.

    c. Values of any changed attributes are emitted from the source to the
       sink, along with the new value point identifier. These are processed
       as in step (1).

    d. The source erases the 'old' value points from storage. If the
       value point identifier has no more value points associated with it
       (i.e. associated with other respositories), then it is also erased.
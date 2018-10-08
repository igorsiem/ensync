# enSync Product Vision Statement

## Overview

*enSync* is a generic, open C++ library for performing data synchronisation
operations. This document is the *Product Vision Statement*, the
highest-level description of *enSync* and its purpose.

## Users

* *Developers* of software solutions that use diverse data sources

  * Software is C++-based, or can make use of C++ libraries

  * Software can make use of Free Software libraries (e.g. LGPL, Boost*
    licenses)

  * Simplicity of use, informative communication, and reliability are
    key factors, reducing development and support effort

* *Installers* of such software solutions ('semi-technical')

  * Can follow well-written technical instructions

  * Can solve some technical issues, when provided with appropriate
    information

  * Simplicity of operation, informative communication, and reliability are
    key factors, reducing installation and support effort

* *Consumers* of such software solutions

  * *Don't care about technical details

  * *Only interested in reliable, 'set and forget' solutions

  * *May have no technical knowledge at all

## Needs

* A software library for synchronising specified data elements between
  disparate repositories:

  * Ability to synchronise multiple data sources down to the attribute level

  * Capability to reconcile data composites with signficiantly differing
    structures (e.g. relational DBs vs. hierarchical data)

  * Typical usage scenario is an application that accesses online data, but
    which must support offline operations on that data.

* High-quality communication for implementors and integrators, to make it as
  easy as possible to use *enSync* in software products

  * Well-designed API

  * Minimal setup config with 'smart' defaults

  * Robust operation with 'sensible' fallback and reporting

* High-quality implementation:

  * Reliable, and robust in the face of 'real world' usage and circumstances

  * Scalable from mobile platforms to multi-core servers, across multiple
    platforms and architectures

  * Core engine fully portable, thread-safe, standard C++ 14 code

* Operational aspects that 'touch' Consumers / End-users are user-friendly,
  and easy to understand

## Features

* Functional capabilities:

  * Synchronisation functionality down to the 'attribute' level

  * N-source data synchronisation with one-way and two-way relationshps

  * 'Plugin' architecture (compile-time) for data storage technologies

  * Full journalling

* Implementation

  * Efficient, light-weight implementation

  * Automated regression tests based on real-world usage cases, and
    illustrating usage and implementation patterns

  * Detailed and usable reporting mechanisms for task monitoring and error
    conditions, including journalling, logging and an exception class
    hierarchy

* High-quality documentation for:

  * Software developers using the library

  * Advice to softare developers on how to document synchronisation
    capabilities for installers and users

  * Contributors (design philosophy and choices, coding standards, etc.)

## Competitors

* [Simego Data Synchronisation Studio](https://www.simego.com/products/data-synchronisation-studio)

  * Commercial

  * Tool (not a library)

  * Strongly SQL-oriented

* [RedGate SQL Compare](https://www.red-gate.com/products/sql-development/sql-compare/?utm_source=bing&utm_term=%2Bdatabase%20%2Bsynchronisation&utm_campaign=BS+%7C+US+%7C+Generic+%7C+Database+Compare+%7C+BMM&utm_medium=cpc&utm_content=QKRSvERj|pcrid|6790390559|pkw|%2Bdatabase%20%2Bsynchronisation|pmt|bb|pdv|c|)

  * Commercial product

  * Tool (not a library)

  * Only supports relational (SQL) databases

* [Microsoft Sync Framework](https://msdn.microsoft.com/en-us/library/mt490616)

  * Library / toolkit

  * Commercial product

## KPIs

The *enSync* project will be considered a success if / when:

* A Minimum Viable Produce (MVP) is reasonably defined and delivered

* The Product is used by at least one 'real world' project

## License

This file is part of the enSync project, written by Igor Siemienowicz.

Copyright (c) 2017 Igor Siemienowicz All rights reserved Distributed under
the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).
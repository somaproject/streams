/*-----------------------------------------------------------------------------+
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

Interval Template Library (Boost.Itl) 3.1.0 Review Version 
==========================================================
PLUS 
Itl_xt    Extended by generalized cross tables (cubes) AND
validate  A law based test automaton (LaBatea).

Abstract:
=========

The Interval Template Library (Itl) provides intervals 
and two kinds of interval containers: Interval_sets and 
interval_maps. Interval_sets and maps can be used just
as the sets or maps of elements. Yet they are much more 
space and time efficient when the elements occur in 
contiguous chunks. This is obviously the case in many 
problem domains, particularly in fields that deal with 
problems related to date and time.

In addition to common set and map semantics, interval
containers allow to compute with segmentation.
A segmentation, e.g. a grid of months, can be intersected
into other interval containers and then be iterated over.

Finally interval_maps allow for aggregations on 
associated values, if added intervals overlap with
intervals, that are stored in the interval_map. This 
feature is called aggregate on overlap.

Itl_xt's cubes and the law based test automaton (LaBatea)
are not yet proposed and prepared as Boost libraries.
But LaBatea has been used in the development of the
itl as test automaton and for checking sematical 
concepts.


Release Notes:
==============

* Boost.Itl 3.1.0 (Review Version)
------------------------------
+ Tested Compilers: 
  Itl: gcc-3.4.4, gcc-4.1.0, gcc-4.3.2, msvc-8.0, msvc-9.0.
  Validate (LaBatea): gcc-4.3.2, msvc-8.0, msvc-9.0.
  LaBatea does not compile with gcc-4.1.0 or below.
+ Improved efficiency of algorithms.
+ Documentation of complexity guarantees.
+ Completion of quickbook documentation.
+ LaBatea now measures time per test.

* Boost.Itl 3.0.0 (Preview 4)
------------------------------
+ Tested Compilers: gcc-3.4.4, gcc-4.3.0 and msvc-9.0.
+ Interoperability between interval containers,
  segments and elements.
+ Full operator support for addition, subtraction,
  intersection and symmetric difference.
+ Boost.Quickbook documentation.

Content:
========

=== Itl =======================================================================						
itl.html                html-documentation referring to 
                        libs/itl/doc/html/index.html
doc/html/images         Images for the html docs. You may delete this, if you 
                        integrate itl_plus into an existing boost installation.

boost/itl               Interval containers, operators, algorithms
boost/itl/type_traits   Metafunctions for types
boost/itl/detail        Implementation details

libs/itl/example        Example programs
libs/itl/test           Test suite
libs/itl/doc            Quickbook documentatons
libs/itl/doc/html       html documentation
libs/itl/build/win32    vc9_all.sln. solution file for msvc-9 including LaBatea 
                        programs.
						
=== Itl extended ==============================================================						
boost/itl_xt            Headers of extended itl classes like cubes.
libs/itl_xt/example     Example programs
libs/itl_xt/test        Test programs

=== LaBatea ===================================================================
boost/validate          Headers of the law based test automaton (LaBatea).
libs/validate/example   Projects for validating Boost.Itl with LaBatea.
libs/validate/src       Sourcefiles.


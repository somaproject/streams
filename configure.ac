#
# This is partly based on
# http://www.openismus.com/documents/linux/building_libraries/building_libraries.shtml
# and 
# Prelude . 
AC_INIT([streams], [1.0], [bugs@soma.mit.edu])
AC_CONFIG_SRCDIR([src/streamsmain.cc])


AM_CONFIG_HEADER(config.h)

dnl -----------------------------------------------
dnl Package name and version number (user defined)
dnl -----------------------------------------------


#release versioning
GENERIC_MAJOR_VERSION=1
GENERIC_MINOR_VERSION=3
GENERIC_MICRO_VERSION=0

#API version (often = GENERIC_MAJOR_VERSION.GENERIC_MINOR_VERSION)
GENERIC_API_VERSION=1.0
AC_SUBST(GENERIC_API_VERSION)

#shared library versioning
GENERIC_LIBRARY_VERSION=1:2:0
#                       | | |
#                +------+ | +---+
#                |        |     |
#             current:revision:age
#                |        |     |
#                |        |     +- increment if interfaces have been added
#                |        |        set to zero if interfaces have been removed
#                                  or changed
#                |        +- increment if source code has changed
#                |           set to zero if current is incremented
#                +- increment if interfaces have been added, removed or changed


dnl --------------------------------
dnl Package name and version number
dnl --------------------------------


GENERIC_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION.$GENERIC_MICRO_VERSION
GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)

VERSION=$GENERIC_VERSION

AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_MACRO_DIR([m4]) # not strictly needed yet, but let's be future-proof


# -----------------------------------------------
# Checks for programs.
# -----------------------------------------------

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

# glib for benchmark
PKG_CHECK_MODULES(GTKDEPS, gtkmm-2.4 >= 2.10 glibmm-2.4 >= 2.13.0 gtkglext-1.0 >= 1.2.0 gtkglextmm-1.2 >= 1.2.0 libglademm-2.4 >= 2.6 )
AC_SUBST(GTKDEPS_CFLAGS)
AC_SUBST(GTKDEPS_LIBS)

PKG_CHECK_MODULES(SOMADEPS,   somanetwork-1.0 >= 1.0 )
AC_SUBST(SOMADEPS_CFLAGS)
AC_SUBST(SOMADEPS_LIBS)

# Boost!!! 
AX_BOOST_BASE([1.34.1])
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_UNIT_TEST_FRAMEWORK
AX_BOOST_THREAD


AM_PROG_LIBTOOL
AM_SANITY_CHECK

AC_LANG_CPLUSPLUS


dnl -----------------------------------------------
dnl Generates Makefile's, configuration files and scripts
dnl -----------------------------------------------

AC_CONFIG_FILES([Makefile src/Makefile ])


AC_OUTPUT

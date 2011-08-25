//
// Created by ZHENG Zhong on 2008-07-03.
//

#ifndef XTREE_CONFIG_HPP_20080703__
#define XTREE_CONFIG_HPP_20080703__

#define XTREE_VERSION 902

//! The 'xtree' library version number as a string. XTREE_VERSION_STR must be defined to be the
//! same as XTREE_VERSION but as a string in the form "x_y_z", where x is the major version
//! number, y is the minor version number, and z is the sub-minor version number.
#define XTREE_VERSION_STR "0_9_2"



////////////////////////////////////////////////////////////////////////////////////////////////////
// Determine build toolset and configurations ...
//

#ifdef _MSC_VER
#  define XTREE_MSVC _MSC_VER
#  if (XTREE_MSVC == 1310)
#    define XTREE_MSVC_VERSION_STR "vc71"
#  elif (XTREE_MSVC == 1400)
#    define XTREE_MSVC_VERSION_STR "vc80"
#  elif (XTREE_MSVC == 1500)
#    define XTREE_MSVC_VERSION_STR "vc90"
#  endif
#endif  // _MSC_VER

#ifdef __GNUC__
#  define XTREE_GNUC __GNUC__
#endif  // __GNUC__


#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)  // win32 platform
#  define XTREE_WIN32 1
#endif  // _WIN32 || __WIN32__ || WIN32

#ifdef _DEBUG
#  define XTREE_DEBUG 1
#endif  // _DEBUG


////////////////////////////////////////////////////////////////////////////////////////////////////
// Determine library type (dynamic or static library) ...
//

#if !defined(XTREE_STATIC_LIB) && !defined(XTREE_SHARED_LIB)
#  define XTREE_SHARED_LIB 1  // by default, build a shared lib.
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
// Define declaration specification for static/dynamic library ...
//

#if defined(XTREE_MSVC) && defined(XTREE_SHARED_LIB)
#  define XTREE_HAS_DECLSPEC 1
// Disable MSVC warnings:
// 4251: class 'X' needs to have dll-interface to be used by clients of class 'Y'.
// 4275: non dll-interface class 'X' used as base for dll-interface class 'Y'.
#  pragma warning(disable: 4251)
#endif  // XTREE_MSVC && XTREE_SHARED_LIB

#ifdef XTREE_HAS_DECLSPEC
#  ifdef XTREE_SOURCE
#    define XTREE_DECL __declspec(dllexport)
#  else
#    define XTREE_DECL __declspec(dllimport)
#  endif  // XTREE_SOURCE
#endif  // XTREE_HAS_DECLSPEC

#ifndef XTREE_DECL
#  define XTREE_DECL
#endif  // XTREE_DECL


////////////////////////////////////////////////////////////////////////////////////////////////////
// Define libxml2-specific macros ...
//

#ifndef XTREE_SHARED_LIB
#  define LIBXML_STATIC   1
#  define LIBXSLT_STATIC  1
#  define LIBEXSLT_STATIC 1
#  define XMLSEC_STATIC   1
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(XTREE_MSVC) && !defined(XTREE_DEBUG)
#  pragma warning(disable: 4702)  // in release: warning of unreachable code, from VC71 STL.
#endif  // defined(XTREE_MSVC) && !defined(XTREE_DEBUG)

#include "xtree/user_config.hpp"


//! The namespace of xtree library.
namespace xtree { }


#endif  // XTREE_CONFIG_HPP_20080703__


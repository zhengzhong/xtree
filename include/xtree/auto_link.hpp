//
// Created by ZHENG Zhong on 2011-06-06.
//

#ifndef XTREE_AUTO_LINK_HPP_20110606__
#define XTREE_AUTO_LINK_HPP_20110606__

#include "xtree/config.hpp"


#if defined(XTREE_MSVC) && defined(XTREE_MSVC_VERSION_STR) && !defined(XTREE_SOURCE)


////////////////////////////////////////////////////////////////////////////////////////////////////
// Link to xtree library
//

#  ifdef XTREE_SHARED_LIB
#    define XTREE_LIB_RUNTIME_TAG ""
#  else
#    define XTREE_LIB_RUNTIME_TAG "-static"
#  endif

#  if defined(_DEBUG) && !defined(NDEBUG)
#    define XTREE_LIB_DEBUG_TAG "-d"
#  elif defined(NDEBUG) && !defined(_DEBUG)
#    define XTREE_LIB_DEBUG_TAG ""
#  else  // _DEBUG and NDEBUG macros conflict.
#    error "[xtree] _DEBUG and NDEBUG macros conflict: only one of them should be defined."
#  endif

#  define XTREE_LIB_NAME  "xtree" XTREE_LIB_RUNTIME_TAG \
                          "-" XTREE_VERSION_STR XTREE_LIB_DEBUG_TAG \
                          "-" XTREE_MSVC_VERSION_STR \
                          ".lib"

#  pragma message("[xtree] auto-linking to: " XTREE_LIB_NAME)
#  pragma comment(lib, XTREE_LIB_NAME)

#  undef XTREE_LIB_RUNTIME_TAG
#  undef XTREE_LIB_DEBUG_TAG
#  undef XTREE_LIB_NAME


////////////////////////////////////////////////////////////////////////////////////////////////////
// Link to dependent libraries as necessary
//

#  ifndef XTREE_SHARED_LIB

#    define XTREE_ZLIB_LIB_NAME     "zlib.lib"
#    define XTREE_ICONV_LIB_NAME    "iconv_a.lib"
#    define XTREE_LIBXML2_LIB_NAME  "libxml2_a.lib"
//#    define XTREE_WS2_LIB_NAME      "ws2_32.lib"
#    define XTREE_WSOCK_LIB_NAME    "wsock32.lib"

#    pragma message("[xtree] auto-linking to: " XTREE_ZLIB_LIB_NAME    ", "  \
                                                XTREE_ICONV_LIB_NAME   ", "  \
                                                XTREE_LIBXML2_LIB_NAME ", "  \
                                                XTREE_WSOCK_LIB_NAME)
#    pragma comment(lib, XTREE_ZLIB_LIB_NAME)
#    pragma comment(lib, XTREE_ICONV_LIB_NAME)
#    pragma comment(lib, XTREE_LIBXML2_LIB_NAME)
#    pragma comment(lib, XTREE_WSOCK_LIB_NAME)

#    undef XTREE_ZLIB_LIB_NAME
#    undef XTREE_ICONV_LIB_NAME
#    undef XTREE_LIBXML2_LIB_NAME
#    undef XTREE_WSOCK_LIB_NAME

#  endif


#endif  // defined(XTREE_MSVC) && defined(XTREE_MSVC_VERSION_STR) && !defined(XTREE_SOURCE)


#endif  // XTREE_AUTO_LINK_HPP_20110606__


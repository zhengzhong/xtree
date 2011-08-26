//
// Created by ZHENG Zhong on 2011-06-07.
//

#if defined(_MSC_VER) && !defined(_DEBUG)
#  pragma warning(disable: 4702)  // in release: warning of unreachable code, from VC71 STL.
#endif

#if defined(__GNUC__) && __GNUC__ >= 4
#  pragma GCC diagnostic ignored "-Wparentheses"
#endif


#include <limits.h>  // workaround for g++: http://svn.boost.org/trac/boost/ticket/4468


#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4127 4511 4512 4535 4701)
#endif

#include <boost/test/included/unit_test_framework.hpp>

#ifdef _MSC_VER
#  pragma warning(pop)
#endif


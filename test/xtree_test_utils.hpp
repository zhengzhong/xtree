//
// Created by ZHENG Zhong on 2011-03-16.
//

#ifndef XTREE_XTREE_TEST_UTILS_20110316__
#define XTREE_XTREE_TEST_UTILS_20110316__

#if defined(_MSC_VER) && !defined(_DEBUG)
#  pragma warning(disable: 4702)  // in release: warning of unreachable code, from VC71 STL.
#endif

#if defined(__GNUC__) && __GNUC__ >= 4
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"  // std::auto_ptr is deprecated.
#endif


#include <limits.h>  // workaround for g++: http://svn.boost.org/trac/boost/ticket/4468


#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4127 4512 4702)
#endif

#include <boost/test/auto_unit_test.hpp>

#ifdef _MSC_VER
#  pragma warning(pop)
#endif

#include <string>


#ifdef XTREE_TEST_DETAIL

#  include <iostream>
#  include <boost/current_function.hpp>

#  define XTREE_LOG_TEST_NAME  { std::cout << ">>> " << BOOST_CURRENT_FUNCTION << "\n"; }
#  define XTREE_LOG(message)   { std::cout << message << "\n"; }

#else

#  define XTREE_LOG_TEST_NAME  // Nothing.
#  define XTREE_LOG(message)   // Nothing.

#endif  // XTREE_TEST_DETAIL


namespace test_utils {


    //! Returns the system variable value by name, or an empty string if the variable is not set.
    //! \param name  the system variable name.
    //! \return the value of the system variable, or an empty string if not set.
    std::string get_sys_var(const std::string& name);

    //! Joins the directory and the name to a path. The path separator depends on the platform.
    //! \param dir   the directory part of the path.
    //! \param name  the base name part of the path.
    //! \return a path concatenating the directory and the name.
    std::string join_path(const std::string& dir, const std::string& name);

    //! Returns the path to the test cases' fixture file. The directory holding fixtures is defined
    //! by the system variable "XTREE_FIXTURE_DIR".
    //! \param name  the base name of the fixture file.
    //! \return the path to the fixture file.
    std::string get_fixture_path(const std::string& name);


}  // namespace test_utils


#endif  // XTREE_XTREE_TEST_UTILS_20110316__


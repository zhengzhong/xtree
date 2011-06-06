//
// Created by ZHENG Zhong on 2011-03-16.
//

#ifndef XTREE_XTREE_TEST_UTILS_20110316__
#define XTREE_XTREE_TEST_UTILS_20110316__

#include <xtree/config.hpp>

#ifdef XTREE_MSVC
#  pragma warning(push)
#  pragma warning(disable: 4127 4512 4702)
#endif

#include <boost/test/auto_unit_test.hpp>

#ifdef XTREE_MSVC
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


    //! Returns the system variable value by name.
    std::string get_sys_var(const std::string& name);

    std::string join_path(const std::string& dir, const std::string& name);

    std::string get_fixture_path(const std::string& name);


}  // namespace test_utils


#endif  // XTREE_XTREE_TEST_UTILS_20110316__


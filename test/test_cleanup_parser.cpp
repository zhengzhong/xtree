//
// Created by ZHENG Zhong on 2011-08-24.
//

#include "xtree_test_utils.hpp"
#include <xtree/libxml2_globals.hpp>


BOOST_AUTO_TEST_CASE(test_cleanup_parser)
{
    // Set libxml2 cleanup parser flag to true to avoid memory leak in unit tests.
    XTREE_LOG_TEST_NAME;
    BOOST_CHECK_EQUAL(xtree::get_libxml2_cleanup_parser(), false);
    xtree::set_libxml2_cleanup_parser(true);
    BOOST_CHECK_EQUAL(xtree::get_libxml2_cleanup_parser(), true);
}


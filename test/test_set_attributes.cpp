//
// Created by ZHENG Zhong on 2011-03-16.
//

#include "xtree_test_utils.hpp"

#include <xtree/xtree.hpp>

#include <memory>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_set_attributes)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root xmlns:x='http://example.com/x' a='A' bc='B &amp; C' d='D'/>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        root->set_attr("a", "AA");
        root->set_attr("x:a", "X:A");
        root->declare_xmlns("y", "http://example.com/y");
        root->set_attr("b", "B");
        root->set_attr("y:b", "Y:B");
        xtree::element_ptr root2 = doc->root();
        BOOST_CHECK_EQUAL(root2->attr("a"), "AA");
        BOOST_CHECK_EQUAL(root2->attr("x:a"), "X:A");
        BOOST_CHECK_EQUAL(root2->attr("a", "http://example.com/x"), "X:A");
        BOOST_CHECK_EQUAL(root2->attr("bc"), "B & C");
        BOOST_CHECK_EQUAL(root2->attr("d"), "D");
        BOOST_CHECK_EQUAL(root2->attr("b"), "B");
        BOOST_CHECK_EQUAL(root2->attr("y:b"), "Y:B");
        BOOST_CHECK_EQUAL(root2->attr("b", "http://example.com/y"), "Y:B");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


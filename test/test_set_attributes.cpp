//
// Created by ZHENG Zhong on 2011-03-16.
//

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4127)
#endif

#include <boost/test/auto_unit_test.hpp>

#ifdef _MSC_VER
#  pragma warning(pop)
#endif

#include <xtree/xtree.hpp>

#include <iostream>
#include <memory>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_set_attributes)
{
    std::cout << "test_set_attributes\n";
    const char* TEST_XML = "<root a='A' bc='B &amp; C' d='D'/>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        root->set_attr("a", "AA");
        root->set_attr("a", "http://www.example.com/xtree", "z:A");
        root->set_attr("x", "X");
        root->set_attr("x", "http://www.example.com/xtree", "z:X");
        xtree::element_ptr root2 = doc->root();
        BOOST_CHECK_EQUAL(root2->attr("a"), "AA");
        BOOST_CHECK_EQUAL(root2->attr("a", "http://www.example.com/xtree"), "z:A");
        BOOST_CHECK_EQUAL(root2->attr("bc"), "B & C");
        BOOST_CHECK_EQUAL(root2->attr("d"), "D");
        BOOST_CHECK_EQUAL(root2->attr("x"), "X");
        BOOST_CHECK_EQUAL(root2->attr("x", "http://www.example.com/xtree"), "z:X");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


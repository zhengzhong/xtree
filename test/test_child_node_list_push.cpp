//
// Created by ZHENG Zhong on 2011-05-09.
//

#include "xtree_test_utils.hpp"

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4127 4511 4512)
#endif

#include <boost/lexical_cast.hpp>

#ifdef _MSC_VER
#  pragma warning(pop)
#endif

#include <xtree/xtree.hpp>

#include <memory>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_child_node_list_push_elements)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root/>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::child_node_list& children = doc->root()->children();
        xtree::element_ptr e1 = children.push_back_element("e1");
        BOOST_CHECK_EQUAL(e1->name(), "e1");
        BOOST_CHECK_EQUAL(e1->uri(), std::string());
        BOOST_CHECK_EQUAL(e1->content(), std::string());
        BOOST_CHECK_EQUAL(children.size(), 1U);
        xtree::element_ptr e2 = children.push_back_element("e2", "http://www.example.com");
        BOOST_CHECK_EQUAL(e2->name(), "e2");
        BOOST_CHECK_EQUAL(e2->uri(), "http://www.example.com");
        BOOST_CHECK_EQUAL(e2->content(), std::string());
        BOOST_CHECK_EQUAL(children.size(), 2U);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


BOOST_AUTO_TEST_CASE(test_child_node_list_push_bad_element)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root/>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::child_node_list& children = doc->root()->children();
        BOOST_CHECK_THROW(children.push_back_element("bad tag"), xtree::bad_dom_operation);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


BOOST_AUTO_TEST_CASE(test_child_node_list_push_texts)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root/>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::child_node_list& children = doc->root()->children();
        xtree::text_ptr text1 = children.push_back_text("hello,");
        BOOST_CHECK_EQUAL(text1->content(), "hello,");
        BOOST_CHECK_EQUAL(children.size(), 1U);
        xtree::text_ptr text2 = children.push_back_text("world!");
        BOOST_CHECK_EQUAL(text1, text2);
        BOOST_CHECK_EQUAL(text2->content(), "hello,world!");
        BOOST_CHECK_EQUAL(text1->content(), "hello,world!");
        BOOST_CHECK_EQUAL(children.size(), 1U);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


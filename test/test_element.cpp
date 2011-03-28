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


BOOST_AUTO_TEST_CASE(test_root_element_no_ns)
{
    std::cout << "test_root_element_no_ns\n";
    const char* TEST_XML =
        "<?xml version='1.0' encoding='UTF-8'?>"
        "<!-- comment before root -->"
        "<root>hello world</root>"
        "<!-- comment after root -->"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        BOOST_CHECK_EQUAL(root->name(), "root");
        BOOST_CHECK_EQUAL(root->uri(), std::string());
        BOOST_CHECK_EQUAL(root->prefix(), std::string());
        BOOST_CHECK_EQUAL(root->qname(), "root");
        BOOST_CHECK_EQUAL(root->tag(), "root");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_root_element_default_ns)
{
    std::cout << "test_root_element_default_ns\n";
    const char* TEST_XML =
        "<?xml version='1.0' encoding='UTF-8'?>"
        "<!-- comment before root -->"
        "<root xmlns='http://www.zhengzhong.net'>hello world</root>"
        "<!-- comment after root -->"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        BOOST_CHECK_EQUAL(root->name(), "root");
        BOOST_CHECK_EQUAL(root->uri(), "http://www.zhengzhong.net");
        BOOST_CHECK_EQUAL(root->prefix(), std::string());
        BOOST_CHECK_EQUAL(root->qname(), "root");
        BOOST_CHECK_EQUAL(root->tag(), "{http://www.zhengzhong.net}root");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_root_element_prefixed_ns)
{
    std::cout << "test_root_element_prefixed_ns\n";
    const char* TEST_XML =
        "<?xml version='1.0' encoding='UTF-8'?>"
        "<!-- comment before root -->"
        "<z:root xmlns:z='http://www.zhengzhong.net'>hello world</z:root>"
        "<!-- comment after root -->"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        BOOST_CHECK_EQUAL(root->name(), "root");
        BOOST_CHECK_EQUAL(root->uri(), "http://www.zhengzhong.net");
        BOOST_CHECK_EQUAL(root->prefix(), "z");
        BOOST_CHECK_EQUAL(root->qname(), "z:root");
        BOOST_CHECK_EQUAL(root->tag(), "{http://www.zhengzhong.net}root");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_root_element_head_tail_1)
{
    std::cout << "test_root_element_head_tail_1\n";
    const char* TEST_XML =
        "<root>"
        "head<![CDATA[before]]>"
        "<item/>"
        "tail<![CDATA[after]]>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        xtree::element_ptr item = root->find_first_elem();
        BOOST_REQUIRE(item != 0);
        BOOST_CHECK_EQUAL(item->name(), "item");
        BOOST_CHECK_EQUAL(item->head(), "headbefore");
        BOOST_CHECK_EQUAL(item->tail(), "tailafter");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_root_element_head_tail_2)
{
    std::cout << "test_root_element_head_tail_2\n";
    const char* TEST_XML =
        "<root>"
        "head<?xmlpi?><![CDATA[before]]>"
        "<item/>"
        "tail<?xmlpi?><![CDATA[after]]>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        xtree::element_ptr item = root->find_first_elem();
        BOOST_REQUIRE(item != 0);
        BOOST_CHECK_EQUAL(item->name(), "item");
        BOOST_CHECK_EQUAL(item->head(), "before");
        BOOST_CHECK_EQUAL(item->tail(), "tail");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}





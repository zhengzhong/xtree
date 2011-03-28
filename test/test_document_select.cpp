//
// Created by ZHENG Zhong on 2011-03-23.
//

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4127 4511 4512)
#endif

#include <boost/test/auto_unit_test.hpp>
#include <boost/lexical_cast.hpp>

#ifdef _MSC_VER
#  pragma warning(pop)
#endif

#include <xtree/xtree.hpp>

#include <iostream>
#include <memory>
#include <string>
#include <utility>


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_select_subelements_no_ns)
{
    std::cout << "test_document_select_subelements_no_ns\n";
    const char* TEST_XML =
        "<root>"
        "  <item>0</item>"
        "  <item>1</item>"
        "  <item>2</item>"
        "</root>"
    ;
    const std::string XPATHS[] = {
        "/root/item",
        "//item",
        "/root/*",
    };
    const unsigned int MAX_SIZE = sizeof(XPATHS) / sizeof(const std::string);
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::node_set nodes;
        for (unsigned int i = 0; i < MAX_SIZE; ++i)
        {
            doc->select_nodes(XPATHS[i], nodes);
            BOOST_CHECK_EQUAL(nodes.empty(), false);
            BOOST_CHECK_EQUAL(nodes.size(), 3U);
            int index = 0;
            for (xtree::node_set::iterator i = nodes.begin(); i != nodes.end(); ++i, ++index)
            {
                BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
                xtree::element_ptr e = xtree::dynamic_node_cast<xtree::element>(i.ptr());
                BOOST_REQUIRE(e != 0);
                BOOST_CHECK_EQUAL(e->name(), "item");
                BOOST_CHECK_EQUAL(e->uri(), std::string());
                BOOST_CHECK_EQUAL(e->prefix(), std::string());
                BOOST_CHECK_EQUAL(e->content(), boost::lexical_cast<std::string>(index));
            }
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_select_subelements_default_ns)
{
    std::cout << "test_document_select_subelements_default_ns\n";
    const char* TEST_XML =
        "<root xmlns='http://www.zhengzhong.net'>"
        "  <item>0</item>"
        "  <item>1</item>"
        "  <item>2</item>"
        "</root>"
    ;
    const std::pair<std::string, std::string> NS_MAPPING("n", "http://www.zhengzhong.net");
    const std::string XPATHS[] = {
        "/n:root/n:item",
        "//n:item",
        "/n:root/*",
    };
    const unsigned int MAX_SIZE = sizeof(XPATHS) / sizeof(const std::string);
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::node_set nodes;
        for (unsigned int i = 0; i < MAX_SIZE; ++i)
        {
            doc->select_nodes(XPATHS[i], NS_MAPPING, nodes);
            BOOST_CHECK_EQUAL(nodes.empty(), false);
            BOOST_CHECK_EQUAL(nodes.size(), 3U);
            int index = 0;
            for (xtree::node_set::iterator i = nodes.begin(); i != nodes.end(); ++i, ++index)
            {
                BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
                xtree::element_ptr e = xtree::dynamic_node_cast<xtree::element>(i.ptr());
                BOOST_REQUIRE(e != 0);
                BOOST_CHECK_EQUAL(e->name(), "item");
                BOOST_CHECK_EQUAL(e->uri(), NS_MAPPING.second);
                BOOST_CHECK_EQUAL(e->prefix(), std::string());
                BOOST_CHECK_EQUAL(e->content(), boost::lexical_cast<std::string>(index));
            }
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_select_subelements_mixed_ns)
{
    std::cout << "test_document_select_subelements_mixed_ns\n";
    const char* TEST_XML =
        "<root xmlns:z='http://www.zhengzhong.net'>"
        "  <z:item>0</z:item>"
        "  <z:item>1</z:item>"
        "  <z:item>2</z:item>"
        "</root>"
    ;
    const std::pair<std::string, std::string> NS_MAPPING("n", "http://www.zhengzhong.net");
    const std::string XPATHS[] = {
        "/root/n:item",
        "//n:item",
        "/root/*",
    };
    const unsigned int MAX_SIZE = sizeof(XPATHS) / sizeof(const std::string);
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::node_set nodes;
        for (unsigned int i = 0; i < MAX_SIZE; ++i)
        {
            doc->select_nodes(XPATHS[i], NS_MAPPING, nodes);
            BOOST_CHECK_EQUAL(nodes.empty(), false);
            BOOST_CHECK_EQUAL(nodes.size(), 3U);
            int index = 0;
            for (xtree::node_set::iterator i = nodes.begin(); i != nodes.end(); ++i, ++index)
            {
                BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
                xtree::element_ptr e = xtree::dynamic_node_cast<xtree::element>(i.ptr());
                BOOST_REQUIRE(e != 0);
                BOOST_CHECK_EQUAL(e->name(), "item");
                BOOST_CHECK_EQUAL(e->uri(), NS_MAPPING.second);
                BOOST_CHECK_EQUAL(e->prefix(), "z");
                BOOST_CHECK_EQUAL(e->content(), boost::lexical_cast<std::string>(index));
            }
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_select_empty)
{
    std::cout << "test_document_select_empty\n";
    const char* TEST_XML =
        "<root xmlns:z='http://www.zhengzhong.net'>"
        "  <z:item>0</z:item>"
        "  <z:item>1</z:item>"
        "  <z:item>2</z:item>"
        "</root>"
    ;
    const std::pair<std::string, std::string> NS_MAPPING("n", "http://www.zhengzhong.net");
    const std::string XPATHS[] = {
        "/root/item",
        "/n:root/item",
        "/n:root/*",
        "//item",
        "/root/whatever",
    };
    const unsigned int MAX_SIZE = sizeof(XPATHS) / sizeof(const std::string);
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::node_set nodes;
        for (unsigned int i = 0; i < MAX_SIZE; ++i)
        {
            doc->select_nodes(XPATHS[i], NS_MAPPING, nodes);
            BOOST_CHECK_EQUAL(nodes.empty(), true);
            BOOST_CHECK_EQUAL(nodes.size(), 0U);
            BOOST_CHECK(nodes.begin() == nodes.end());
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}




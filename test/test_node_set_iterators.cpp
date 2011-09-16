//
// Created by ZHENG Zhong on 2011-08-30.
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

#include <xtree/xtree_dom.hpp>

#include <iterator>
#include <memory>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_default_constructed_empty_node_set)
{
    XTREE_LOG_TEST_NAME;
    try
    {
        xtree::node_set nothing;
        BOOST_CHECK_EQUAL(nothing.empty(), true);
        BOOST_CHECK_EQUAL(nothing.size(), 0U);
        BOOST_CHECK(nothing.begin() == nothing.end());
        BOOST_CHECK(nothing.begin_element() == nothing.end_element());
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


namespace {


    //! The Iterator template parameter could be child_iterator or const_child_iterator.
    template<class NodeSet, class NodeIterator, class ElementIterator>
    void do_test_node_set_iterators(NodeSet& nodes)
    {
        // Test move forward NodeIterator ...
        int index = 0;
        for (NodeIterator i = nodes.begin(); i != nodes.end(); ++i, ++index)
        {
            BOOST_CHECK_EQUAL(i->type(), (index % 2 == 0 ? xtree::text_node : xtree::element_node));
            BOOST_CHECK_EQUAL(i->content(), boost::lexical_cast<std::string>(index));
        }
        BOOST_CHECK_EQUAL(index, 8);

        // Test move backward NodeIterator ...
        index = 8;
        NodeIterator n = nodes.end();
        do 
        {
            --n;
            --index;
            BOOST_CHECK_EQUAL(n->type(), (index % 2 == 0 ? xtree::text_node : xtree::element_node));
            BOOST_CHECK_EQUAL(n->content(), boost::lexical_cast<std::string>(index));
        }
        while (n != nodes.begin());
        BOOST_CHECK_EQUAL(index, 0);

        // Test advance NodeIterator ...
        NodeIterator n2 = nodes.begin();
        std::advance(n2, 5);
        BOOST_CHECK_EQUAL(n2->type(), xtree::element_node);
        BOOST_CHECK_EQUAL(n2->content(), "5");

        // Test move forward ElementIterator ...
        index = 0;
        for (ElementIterator i = nodes.begin_element(); i != nodes.end_element(); ++i, ++index)
        {
            BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(i->name(), "element");
            BOOST_CHECK_EQUAL(i->prefix(), std::string());
            BOOST_CHECK_EQUAL(i->uri(), std::string());
            BOOST_CHECK_EQUAL(i->content(), boost::lexical_cast<std::string>(index * 2 + 1));
        }
        BOOST_CHECK_EQUAL(index, 4);

        // Test move backward ElementIterator ...
        index = 4;
        ElementIterator e = nodes.end_element();
        do 
        {
            --e;
            --index;
            BOOST_CHECK_EQUAL(e->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(e->name(), "element");
            BOOST_CHECK_EQUAL(e->prefix(), std::string());
            BOOST_CHECK_EQUAL(e->uri(), std::string());
            BOOST_CHECK_EQUAL(e->content(), boost::lexical_cast<std::string>(index * 2 + 1));
        }
        while (e != nodes.begin_element());
        BOOST_CHECK_EQUAL(index, 0);

        // Test advance ElementIterator ...
        ElementIterator e2 = nodes.begin_element();
        std::advance(e2, 3);
        BOOST_CHECK_EQUAL(e2->type(), xtree::element_node);
        BOOST_CHECK_EQUAL(e2->name(), "element");
        BOOST_CHECK_EQUAL(e2->prefix(), std::string());
        BOOST_CHECK_EQUAL(e2->uri(), std::string());
        BOOST_CHECK_EQUAL(e2->content(), "7");
    }


}  // anonymous namespace


BOOST_AUTO_TEST_CASE(test_node_set_iterators)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root>"
        "0<element>1</element>2<element>3</element>"
        "4<element>5</element>6<element>7</element>"
        "</root>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        xtree::node_set nodes;
        doc->root()->select_nodes(xtree::xpath("*|text()"), nodes);
        BOOST_CHECK_EQUAL(nodes.empty(), false);
        BOOST_CHECK_EQUAL(nodes.size(), 8U);
        do_test_node_set_iterators<
            xtree::node_set,
            xtree::node_set::iterator,
            xtree::node_set::element_iterator
        >(nodes);
        do_test_node_set_iterators<
            xtree::node_set,
            xtree::node_set::const_iterator,
            xtree::node_set::const_element_iterator
        >(nodes);
        do_test_node_set_iterators<
            const xtree::node_set,
            xtree::node_set::const_iterator,
            xtree::node_set::const_element_iterator
        >(nodes);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_node_set_empty_element_iterator)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root>text</root>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        xtree::node_set nodes;
        doc->root()->select_nodes(xtree::xpath("*|text()"), nodes);
        BOOST_CHECK_EQUAL(nodes.empty(), false);
        BOOST_CHECK_EQUAL(nodes.size(), 1U);
        xtree::node_set::iterator node = nodes.begin();
        BOOST_REQUIRE(node != nodes.end());
        BOOST_CHECK_EQUAL(node->type(), xtree::text_node);
        BOOST_CHECK_EQUAL(node->content(), "text");
        BOOST_CHECK(++node == nodes.end());
        BOOST_CHECK(nodes.begin_element() == nodes.end_element());
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_node_set_empty_iterators)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root></root>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        xtree::node_set nodes;
        doc->root()->select_nodes(xtree::xpath("*|text()"), nodes);
        BOOST_CHECK_EQUAL(nodes.empty(), true);
        BOOST_CHECK_EQUAL(nodes.size(), 0U);
        BOOST_CHECK(nodes.begin() == nodes.end());
        BOOST_CHECK(nodes.begin_element() == nodes.end_element());
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


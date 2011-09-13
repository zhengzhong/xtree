//
// Created by ZHENG Zhong on 2011-03-16.
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

#include <memory>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_dom_parse_string)
{
    XTREE_LOG_TEST_NAME;

    const char* TEST_XML =
        "<?xml version='1.0' encoding='UTF-8'?>"
        "<?processing-instructions 0?>"
        "<!--1-->"
        "<root>2</root>"
    ;
    const xtree::node_t TYPES[] = {
        xtree::instruction_node,
        xtree::comment_node,
        xtree::element_node,
    };
    const unsigned int MAX_SIZE = sizeof(TYPES) / sizeof(const xtree::node_t);

    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        BOOST_CHECK_EQUAL(doc->empty(), false);
        BOOST_CHECK_EQUAL(doc->version(), "1.0");
        BOOST_CHECK_EQUAL(doc->encoding(), "UTF-8");
        BOOST_CHECK_EQUAL(doc->root()->name(), "root");

        unsigned int index = 0;
        for (xtree::child_iterator i = doc->begin(); i != doc->end(); ++i, ++index)
        {
            BOOST_REQUIRE(index < MAX_SIZE);
            BOOST_CHECK_EQUAL(i->type(), TYPES[index]);
            BOOST_CHECK_EQUAL(i->content(), boost::lexical_cast<std::string>(index));
        }

        xtree::element_ptr root = doc->root();
        BOOST_CHECK_EQUAL(root->name(), "root");
        BOOST_CHECK_EQUAL(root->content(), "2");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_dom_parse_url)
{
    XTREE_LOG_TEST_NAME;

    const std::string URL = "http://www.w3schools.com/xml/note.xml";
    const std::string PROXY = std::string();  // no proxy.
    const std::pair<std::string, std::string> SUBELEMENTS[] = {
        std::make_pair("to", "Tove"),
        std::make_pair("from", "Jani"),
        std::make_pair("heading", "Reminder"),
        std::make_pair("body", "Don't forget me this weekend!"),
    };
    const unsigned int MAX_SIZE = sizeof(SUBELEMENTS)
                                / sizeof(const std::pair<std::string, std::string>);

    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_url(URL, PROXY);
        BOOST_REQUIRE(!doc->empty());
        xtree::element_ptr root = doc->root();
        BOOST_CHECK_EQUAL(root->name(), "note");

        unsigned int index = 0;
        for (xtree::child_iterator i = root->begin(); i != root->end(); ++i, ++index)
        {
            BOOST_REQUIRE(index < MAX_SIZE);
            BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(i->name(), SUBELEMENTS[index].first);
            BOOST_CHECK_EQUAL(i->content(), SUBELEMENTS[index].second);
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_dom_parse_bad_xml)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML[] = {
        // invalid element tag...
        "<root><bad element/></root>",
        // attributes with the same name...
        "<root a='A1' b='B' a='A2'/>",
        "<root xmlns:x='http://example.com' xmlns:y='http://example.com' x:a='A' y:a='B'/>",
        // unmatched begin/close tag...
        "<root>hello",
        "<root>hello</Root>",
        
    };
    for (unsigned int i = 0; i < sizeof(TEST_XML) / sizeof(const char*); ++i)
    {
        BOOST_CHECK_THROW(xtree::parse_string(TEST_XML[i]), xtree::dom_error);
    }
}



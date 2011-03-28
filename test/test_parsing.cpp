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


BOOST_AUTO_TEST_CASE(test_parse_string)
{
    std::cout << "test_parse_string\n";
    const char* TEST_XML =
        "<?xml version='1.0' encoding='UTF-8'?>"
        "<!--one-->"
        "<root>two</root>"
        "<?processing-instructions three?>"
        "<!--four-->"
    ;
    const std::pair<xtree::node_t, std::string> EXPECTED_NODES[] = {
        std::make_pair(xtree::comment_node, "one"),
        std::make_pair(xtree::element_node, "two"),
        std::make_pair(xtree::xml_pi_node, "three"),
        std::make_pair(xtree::comment_node, "four"),
    };
    const unsigned int MAX_SIZE = sizeof(EXPECTED_NODES)
                                / sizeof(const std::pair<xtree::node_t, std::string>);

    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        BOOST_CHECK_EQUAL(doc->empty(), false);
        BOOST_CHECK_EQUAL(doc->version(), "1.0");
        BOOST_CHECK_EQUAL(doc->encoding(), "UTF-8");
        BOOST_CHECK_EQUAL(doc->root()->name(), "root");
        xtree::child_node_list& children = doc->children();
        unsigned int index = 0;
        for (xtree::child_iterator i = children.begin(); i != children.end(); ++i, ++index)
        {
            BOOST_REQUIRE(index < MAX_SIZE);
            BOOST_CHECK_EQUAL(i->type(), EXPECTED_NODES[index].first);
            BOOST_CHECK_EQUAL(i->content(), EXPECTED_NODES[index].second);
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_parse_not_well_formed)
{
    std::cout << "test_parse_not_well_formed\n";
    const char* TEST_XML = "<root><bad element/></root>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        BOOST_ERROR("expected dom_error but got nothing when parsing non-well-formed XML");
    }
    catch (const xtree::dom_error&)
    {
        // Ignore.
    }
}



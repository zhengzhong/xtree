//
// Created by ZHENG Zhong on 2011-03-18.
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


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_iterate_child_node_list)
{
    std::cout << "test_iterate_child_node_list\n";
    const char* TEST_XML =
        "<root>"
        "  <item>0</item>"
        "  <item>1</item>"
        "  <item>2</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        xtree::child_node_list& children = root->children();
        unsigned int index = 0;
        for (xtree::child_iterator i = children.begin(); i != children.end(); ++i, ++index)
        {
            BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(i->name(), "item");
            BOOST_CHECK_EQUAL(i->content(), boost::lexical_cast<std::string>(index));
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_iterate_child_node_list_2)
{
    std::cout << "test_iterate_child_node_list_2\n";
    const char* TEST_XML =
        "<root>"
        "one"
        "<b>two</b>"
        "three"
        "<![CDATA[four]]>"
        "<!--five-->"
        "<?processing-instruction six?>"
        "<b>seven</b>"
        "</root>";
    std::pair<xtree::node_t, std::string> EXPECTED_NODES[] = {
        std::make_pair(xtree::text_node, "one"),
        std::make_pair(xtree::element_node, "two"),
        std::make_pair(xtree::text_node, "three"),
        std::make_pair(xtree::cdata_node, "four"),
        std::make_pair(xtree::comment_node, "five"),
        std::make_pair(xtree::xml_pi_node, "six"),
        std::make_pair(xtree::element_node, "seven"),
    };
    const unsigned int MAX_SIZE = sizeof(EXPECTED_NODES)
                                / sizeof(std::pair<xtree::node_t, std::string>);
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        xtree::child_node_list& children = root->children();
        unsigned int index = 0;
        for (xtree::child_iterator i = children.begin(); i != children.end(); ++i, ++index)
        {
            BOOST_REQUIRE(index < MAX_SIZE);
            BOOST_CHECK_EQUAL(i->type(), EXPECTED_NODES[index].first);
            BOOST_CHECK_EQUAL(i->content(), EXPECTED_NODES[index].second);
            if (i->type() == xtree::element_node)
            {
                xtree::element_ptr e = xtree::dynamic_node_cast<xtree::element>(i.ptr());
                BOOST_CHECK(e != 0);
                BOOST_CHECK_EQUAL(e->name(), "b");
            }
            else if (i->type() == xtree::xml_pi_node)
            {
                xtree::xml_pi_ptr pi = xtree::dynamic_node_cast<xtree::xml_pi>(i.ptr());
                BOOST_CHECK(pi != 0);
                BOOST_CHECK_EQUAL(pi->name(), "processing-instruction");
            }
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}



//
// Created by ZHENG Zhong on 2011-03-16.
//

#include "xtree_test_utils.hpp"

#include <xtree/xtree_dom.hpp>

#include <memory>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_node_cast)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root>two</root>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        BOOST_REQUIRE(root != 0);
        // Test const_node_cast.
        xtree::const_element_ptr croot = root;  // implicit cast.
        BOOST_CHECK_EQUAL(croot, root);
        BOOST_CHECK_EQUAL(xtree::const_node_cast<xtree::element>(croot), root);
        BOOST_CHECK_EQUAL(xtree::const_node_cast<const xtree::element>(root), croot);
        // Test dynamic_node_cast.
        xtree::node_ptr node = root;
        BOOST_CHECK_EQUAL(node, root);
        BOOST_CHECK_EQUAL(xtree::dynamic_node_cast<xtree::element>(node), root);
        BOOST_CHECK_EQUAL(xtree::dynamic_node_cast<xtree::child_node>(node), root);
        BOOST_CHECK(xtree::dynamic_node_cast<xtree::attribute>(node) == 0);
        BOOST_CHECK(xtree::dynamic_node_cast<xtree::text>(node) == 0);
        // Test dynamic_node_cast on const pointers.
        xtree::const_node_ptr cnode = root;
        BOOST_CHECK_EQUAL(cnode, root);
        BOOST_CHECK_EQUAL(xtree::dynamic_node_cast<const xtree::element>(cnode), root);
        BOOST_CHECK_EQUAL(xtree::dynamic_node_cast<const xtree::child_node>(cnode), root);
        BOOST_CHECK(xtree::dynamic_node_cast<const xtree::attribute>(cnode) == 0);
        BOOST_CHECK(xtree::dynamic_node_cast<const xtree::instruction>(cnode) == 0);
        BOOST_CHECK(xtree::dynamic_node_cast<const xtree::text>(cnode) == 0);
        BOOST_CHECK(xtree::dynamic_node_cast<const xtree::comment>(cnode) == 0);
        // Following code should not compile.
        //xtree::dynamic_node_cast<xtree::child_node>(cnode);
        //xtree::dynamic_node_cast<xtree::element>(cnode);
        //xtree::dynamic_node_cast<xtree::attribute>(cnode);
        //xtree::dynamic_node_cast<xtree::instruction>(cnode);
        //xtree::dynamic_node_cast<xtree::text>(cnode);
        //xtree::dynamic_node_cast<xtree::comment>(cnode);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}



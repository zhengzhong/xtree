//
// Created by ZHENG Zhong on 2011-08-24.
//

#include "xtree_test_utils.hpp"

#include <xtree/xtree_dom.hpp>

#include <memory>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


namespace {

    void do_test_find_xmlns(xtree::element_ptr elem,
                            const std::string& prefix,
                            const std::string& uri)
    {
        xtree::xmlns_ptr ns1 = elem->find_xmlns_by_uri(uri);
        BOOST_REQUIRE(ns1 != 0);
        BOOST_CHECK_EQUAL(ns1->prefix(), prefix);
        BOOST_CHECK_EQUAL(ns1->uri(), uri);
        xtree::xmlns_ptr ns2 = elem->find_xmlns_by_prefix(prefix);
        BOOST_REQUIRE(ns2 != 0);
        BOOST_CHECK_EQUAL(ns2->prefix(), prefix);
        BOOST_CHECK_EQUAL(ns2->uri(), uri);
        BOOST_CHECK(ns1 == ns2);
    }

}  // anonymous namespace


BOOST_AUTO_TEST_CASE(test_find_xmlns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://example.com' xmlns:x='http://example.com/x'>"
        "  <x:sub xmlns:y='http://example.com/y'>"
        "    <y:test1>one</y:test1>"
        "    <y:test2 xmlns:y='http://example.com/y/bis'>two</y:test2>"
        "  </x:sub>"
        "</root>"
    ;

    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        BOOST_CHECK_EQUAL(root->uri(), "http://example.com");
        do_test_find_xmlns(root, "", "http://example.com");
        do_test_find_xmlns(root, "x", "http://example.com/x");
        BOOST_CHECK(root->find_xmlns_by_prefix("y") == 0);
        BOOST_CHECK(root->find_xmlns_by_uri("http://example.com/y") == 0);
        BOOST_CHECK(root->find_xmlns_by_uri("http://example.com/y/bis") == 0);

        xtree::element_ptr test1 = root->find_elem_by_path("sub/test1");
        BOOST_REQUIRE(test1 != 0);
        BOOST_CHECK_EQUAL(test1->uri(), "http://example.com/y");
        do_test_find_xmlns(test1, "", "http://example.com");
        do_test_find_xmlns(test1, "x", "http://example.com/x");
        do_test_find_xmlns(test1, "y", "http://example.com/y");
        BOOST_CHECK(test1->find_xmlns_by_uri("http://example.com/y/bis") == 0);

        xtree::element_ptr test2 = root->find_elem_by_path("sub/test2");
        BOOST_REQUIRE(test2 != 0);
        BOOST_CHECK_EQUAL(test2->uri(), "http://example.com/y/bis");
        do_test_find_xmlns(test2, "", "http://example.com");
        do_test_find_xmlns(test2, "x", "http://example.com/x");
        do_test_find_xmlns(test2, "y", "http://example.com/y/bis");
        BOOST_CHECK(test2->find_xmlns_by_uri("http://example.com/y") == 0);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_set_xmlns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML_1 = "<root xmlns:ns='http://example.com/x'/>";
    const char* TEST_XML_2 = "<root xmlns:ns='http://example.com/y'/>";
    try
    {
        std::auto_ptr<xtree::document> doc1 = xtree::parse_string(TEST_XML_1);
        xtree::element_ptr root1 = doc1->root();
        BOOST_CHECK_EQUAL(root1->prefix(), std::string());
        BOOST_CHECK_EQUAL(root1->uri(), std::string());
        // Associate namespace declared on this document to the root element.
        xtree::const_xmlns_ptr ns1 = root1->find_xmlns_by_prefix("ns");
        root1->set_xmlns(ns1);
        BOOST_CHECK_EQUAL(root1->prefix(), "ns");
        BOOST_CHECK_EQUAL(root1->uri(), "http://example.com/x");
        // Associate namespace declared on another document to the root element.
        {
            std::auto_ptr<xtree::document> doc2 = xtree::parse_string(TEST_XML_2);
            xtree::element_ptr root2 = doc2->root();
            xtree::const_xmlns_ptr ns2 = root2->find_xmlns_by_prefix("ns");
            root1->set_xmlns(ns2);
        }
        // At this point, doc2 (and all xmlns declared on it) should have been destroyed.
        BOOST_CHECK_EQUAL(root1->uri(), "http://example.com/y");
        // Remove namespace from the root element.
        root1->set_xmlns(xtree::xmlns_ptr());
        BOOST_CHECK_EQUAL(root1->prefix(), std::string());
        BOOST_CHECK_EQUAL(root1->uri(), std::string());
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_declare_xmlns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root/>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        // The first declaration should succeed.
        std::pair<xtree::xmlns_ptr, bool> decl = root->declare_xmlns("x", "http://example.com/x");
        BOOST_CHECK_EQUAL(root->uri(), std::string());
        BOOST_CHECK_EQUAL(decl.first->prefix(), "x");
        BOOST_CHECK_EQUAL(decl.first->uri(), "http://example.com/x");
        BOOST_CHECK_EQUAL(decl.second, true);
        // Declaring namespace with the same prefix should fail.
        decl = root->declare_xmlns("x", "http://example.com/y");
        BOOST_CHECK_EQUAL(root->uri(), std::string());
        BOOST_CHECK_EQUAL(decl.first->prefix(), "x");
        BOOST_CHECK_EQUAL(decl.first->uri(), "http://example.com/x");
        BOOST_CHECK_EQUAL(decl.second, false);
        // Declaring namespace with empty prefix will cause inconsistent namespace.
        decl = root->declare_xmlns(std::string(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(root->uri(), std::string());
        BOOST_CHECK_EQUAL(decl.first->prefix(), std::string());
        BOOST_CHECK_EQUAL(decl.first->uri(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(decl.second, true);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_declare_xmlns_with_same_prefix)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<x:root xmlns:x='http://example.com/x'><x:sub/></x:root>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr sub = doc->root()->find_elem_by_name("sub");
        BOOST_REQUIRE(sub != 0);
        BOOST_CHECK_EQUAL(sub->prefix(), "x");
        BOOST_CHECK_EQUAL(sub->uri(), "http://example.com/x");
        // Declare an xmlns which conflicts with the element prefix.
        std::pair<xtree::xmlns_ptr, bool> decl = sub->declare_xmlns("x", "http://example.com/y");
        // Check the element is adapted automatically.
        BOOST_CHECK(sub->prefix() != "x");
        BOOST_CHECK_EQUAL(sub->uri(), "http://example.com/x");
        BOOST_CHECK_EQUAL(decl.first->prefix(), "x");
        BOOST_CHECK_EQUAL(decl.first->uri(), "http://example.com/y");
        BOOST_CHECK_EQUAL(decl.second, true);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_declare_xmlns_with_conflict)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<x:root xmlns:x='http://example.com/x'>"
        "<sub><x:sub2/></sub>"
        "</x:root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr sub = doc->root()->find_elem_by_name("sub");
        BOOST_REQUIRE(sub != 0);
        // Declare an xmlns which conflicts with the subelement "sub2".
        std::pair<xtree::xmlns_ptr, bool> decl = sub->declare_xmlns("x", "http://example.com/y");
        BOOST_CHECK_EQUAL(sub->uri(), std::string());
        BOOST_CHECK_EQUAL(decl.first->prefix(), "x");
        BOOST_CHECK_EQUAL(decl.first->uri(), "http://example.com/y");
        BOOST_CHECK_EQUAL(decl.second, true);
        // Check the subelement "sub2" is adapted automatically.
        xtree::element_ptr sub2 = sub->find_elem_by_name("sub2");
        BOOST_CHECK_EQUAL(sub2->uri(), "http://example.com/x");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


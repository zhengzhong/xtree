//
// Created by ZHENG Zhong on 2011-08-24.
//

#include "xtree_test_utils.hpp"

#include <xtree/xtree.hpp>

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
        "<?xml version='1.0' encoding='UTF-8'?>"
        "<root xmlns='http://example.com' xmlns:x='http://example.com/x'>"
        "  <x:sub xmlns:y='http://example.com/y'>"
        "    <y:test1>one</y:test1>"
        "    <y:test2 xmlns:y='http://example.com/y/bis'>two</y:test2>"
        "  </x:sub>"
        "</root>"
    ;

    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
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



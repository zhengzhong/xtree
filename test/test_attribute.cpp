//
// Created by ZHENG Zhong on 2011-03-16.
//

#include "xtree_test_utils.hpp"

#include <xtree/xtree_dom.hpp>

#include <memory>
#include <string>


namespace {


    void check_attribute_map_(xtree::attribute_map& attrs,
                              const std::string expected_attrs[][3],
                              unsigned int size)
    {
        unsigned int index = 0;
        for (xtree::attribute_map::iterator i = attrs.begin(); i != attrs.end(); ++i, ++index)
        {
            BOOST_CHECK(i->name() != "xmlns");
            if (index < size)
            {
                BOOST_CHECK_EQUAL(i->name(), expected_attrs[index][0]);
                BOOST_CHECK_EQUAL(i->uri(), expected_attrs[index][1]);
                BOOST_CHECK_EQUAL(i->value(), expected_attrs[index][2]);
            }
            else
            {
                BOOST_ERROR("attribute index out of range");
            }
        }
    }


}  // anonymous namespace


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_get_attributes_no_xmlns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root a='one' b='two &amp; three' c='four'/>"
    ;
    const std::string EXPECTED_ATTRS[][3] = {
        {"a", std::string(), "one"},
        {"b", std::string(), "two & three"},
        {"c", std::string(), "four"},
    };
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        // Direct access to attribute values.
        BOOST_CHECK_EQUAL(root->attr("a"), "one");
        BOOST_CHECK_EQUAL(root->attr("b"), "two & three");
        BOOST_CHECK_EQUAL(root->attr("c"), "four");
        BOOST_CHECK_EQUAL(root->attr("x"), std::string());
        // Iteration over attribute map.
        check_attribute_map_(root->attrs(), EXPECTED_ATTRS, 3);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_get_attributes_default_xmlns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://example.com/xtree' a='one' b='two &amp; three' c='four'/>"
    ;
    const std::string EXPECTED_ATTRS[][3] = {
        {"a", std::string(), "one"},
        {"b", std::string(), "two & three"},
        {"c", std::string(), "four"},
    };
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        // Direct access to attribute values.
        BOOST_CHECK_EQUAL(root->attr("a"), "one");
        BOOST_CHECK_EQUAL(root->attr("b"), "two & three");
        BOOST_CHECK_EQUAL(root->attr("c"), "four");
        BOOST_CHECK_EQUAL(root->attr("x"), std::string());
        // Iteration over attribute map.
        check_attribute_map_(root->attrs(), EXPECTED_ATTRS, 3);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_get_attributes_mixed_xmlns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns:x='http://example.com/x' xmlns:y='http://example.com/y'"
        "      a='one' x:b='two &amp; three' y:b='four' y:c='five'/>"
    ;
    const std::string URI_X = "http://example.com/x";
    const std::string URI_Y = "http://example.com/y";
    const std::string EXPECTED_ATTRS[][3] = {
        {"a", std::string(), "one"},
        {"b", URI_X, "two & three"},
        {"b", URI_Y, "four"},
        {"c", URI_Y, "five"},
    };
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        // Direct access to attribute values.
        BOOST_CHECK_EQUAL(root->attr("a"), "one");
        BOOST_CHECK_EQUAL(root->attr("b"), std::string());
        BOOST_CHECK_EQUAL(root->attr("b", URI_X), "two & three");
        BOOST_CHECK_EQUAL(root->attr("b", URI_Y), "four");
        BOOST_CHECK_EQUAL(root->attr("c", URI_Y), "five");
        BOOST_CHECK_EQUAL(root->attr("x"), std::string());
        // Iteration over attribute map.
        check_attribute_map_(root->attrs(), EXPECTED_ATTRS, 4);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_get_attributes_same_xmlns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns:x='http://example.com/xtree' xmlns:y='http://example.com/xtree' "
        "      x:a='A'/>"
    ;
    const std::string URI = "http://example.com/xtree";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        BOOST_CHECK_EQUAL(root->attr("a"), std::string());
        BOOST_CHECK_EQUAL(root->attr("x:a"), "A");
        BOOST_CHECK_EQUAL(root->attr("y:a"), "A");
        BOOST_CHECK_EQUAL(root->attr("a", URI), "A");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_set_attributes)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns:x='http://example.com/xtree' xmlns:y='http://example.com/xtree' "
        "      a='A' bc='B &amp; C' x:d='X:D'/>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        BOOST_CHECK_EQUAL(root->attr("a"), "A");
        BOOST_CHECK_EQUAL(root->attr("bc"), "B & C");
        BOOST_CHECK_EQUAL(root->attr("x:d"), "X:D");
        BOOST_CHECK_EQUAL(root->attr("y:d"), "X:D");

        root->set_attr("a", "AA");
        root->set_attr("x:a", "X:A");
        BOOST_CHECK_EQUAL(root->attr("a"), "AA");
        BOOST_CHECK_EQUAL(root->attr("x:a"), "X:A");
        BOOST_CHECK_EQUAL(root->attr("a", "http://example.com/xtree"), "X:A");

        root->declare_xmlns("z", "http://example.com/z");
        root->set_attr("b", "B");
        root->set_attr("z:b", "Z:B");
        BOOST_CHECK_EQUAL(root->attr("b"), "B");
        BOOST_CHECK_EQUAL(root->attr("z:b"), "Z:B");
        BOOST_CHECK_EQUAL(root->attr("b", "http://example.com/z"), "Z:B");

        root->set_attr("y:d", "Y:D");
        BOOST_CHECK_EQUAL(root->attr("x:d"), "Y:D");
        BOOST_CHECK_EQUAL(root->attr("y:d"), "Y:D");
        BOOST_CHECK_EQUAL(root->attr("d", "http://example.com/xtree"), "Y:D");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_set_attribute_error)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root xmlns:x='http://example.com/xtree'/>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        BOOST_CHECK_EQUAL(root->attrs().size(), 0U);
        BOOST_CHECK_THROW(root->set_attr("y:a", "Y"), xtree::bad_dom_operation);
        BOOST_CHECK_EQUAL(root->attrs().size(), 0U);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


//
// Created by ZHENG Zhong on 2011-03-16.
//

#include "xtree_test_utils.hpp"

#include <xtree/xtree.hpp>

#include <memory>
#include <string>


namespace {


    void check_attribute_map(xtree::attribute_map& attrs,
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


BOOST_AUTO_TEST_CASE(test_get_attributes_no_ns)
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
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        // Direct access to attribute values.
        BOOST_CHECK_EQUAL(root->attr("a"), "one");
        BOOST_CHECK_EQUAL(root->attr("b"), "two & three");
        BOOST_CHECK_EQUAL(root->attr("c"), "four");
        BOOST_CHECK_EQUAL(root->attr("x"), std::string());
        // Interation over attribute map.
        check_attribute_map(root->attrs(), EXPECTED_ATTRS, 3);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_get_attributes_default_ns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://www.zhengzhong.net' a='one' b='two &amp; three' c='four'/>"
    ;
    const std::string EXPECTED_ATTRS[][3] = {
        {"a", std::string(), "one"},
        {"b", std::string(), "two & three"},
        {"c", std::string(), "four"},
    };
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        // Direct access to attribute values.
        BOOST_CHECK_EQUAL(root->attr("a"), "one");
        BOOST_CHECK_EQUAL(root->attr("b"), "two & three");
        BOOST_CHECK_EQUAL(root->attr("c"), "four");
        BOOST_CHECK_EQUAL(root->attr("x"), std::string());
        // Interation over attribute map.
        check_attribute_map(root->attrs(), EXPECTED_ATTRS, 3);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_get_attributes_mixed_ns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns:x='http://www.zhengzhong.net/x/' xmlns:y='http://www.zhengzhong.net/y/'"
        "      a='one' x:b='two &amp; three' y:b='four' y:c='five'/>"
    ;
    const std::string URI_X = "http://www.zhengzhong.net/x/";
    const std::string URI_Y = "http://www.zhengzhong.net/y/";
    const std::string EXPECTED_ATTRS[][3] = {
        {"a", std::string(), "one"},
        {"b", URI_X, "two & three"},
        {"b", URI_Y, "four"},
        {"c", URI_Y, "five"},
    };
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        // Direct access to attribute values.
        BOOST_CHECK_EQUAL(root->attr("a"), "one");
        BOOST_CHECK_EQUAL(root->attr("b"), std::string());
        BOOST_CHECK_EQUAL(root->attr("b", URI_X), "two & three");
        BOOST_CHECK_EQUAL(root->attr("b", URI_Y), "four");
        BOOST_CHECK_EQUAL(root->attr("c", URI_Y), "five");
        BOOST_CHECK_EQUAL(root->attr("x"), std::string());
        // Interation over attribute map.
        check_attribute_map(root->attrs(), EXPECTED_ATTRS, 4);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}



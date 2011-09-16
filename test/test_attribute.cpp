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


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_attribute_map_find)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://example.com/root' "
        "      xmlns:x='http://example.com/xtree' "
        "      xmlns:y='http://example.com/xtree' "
        "      x:a='A' b='B' />"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::attribute_map& attrs = doc->root()->attrs();
        BOOST_CHECK_EQUAL(attrs.size(), 2U);

        xtree::attribute_iterator a1 = attrs.find("x:a");
        BOOST_REQUIRE(a1 != attrs.end());
        BOOST_CHECK_EQUAL(a1->name(), "a");
        BOOST_CHECK_EQUAL(a1->prefix(), "x");
        BOOST_CHECK_EQUAL(a1->uri(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(a1->value(), "A");
        xtree::attribute_iterator a2 = attrs.find("a", "http://example.com/xtree");
        BOOST_CHECK(a1 == a2);
        xtree::attribute_iterator a3 = attrs.find("y:a");
        BOOST_CHECK(a1 == a3);
        xtree::attribute_iterator a_end = attrs.find("a");
        BOOST_CHECK(a_end == attrs.end());

        xtree::attribute_iterator b = attrs.find("b");
        BOOST_REQUIRE(b != attrs.end());
        BOOST_CHECK_EQUAL(b->name(), "b");
        BOOST_CHECK_EQUAL(b->prefix(), std::string());
        BOOST_CHECK_EQUAL(b->uri(), std::string());
        BOOST_CHECK_EQUAL(b->value(), "B");
        xtree::attribute_iterator b_end = attrs.find("b", "http://example.com/root");
        BOOST_CHECK(b_end == attrs.end());
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_attribute_map_insert_update)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://example.com/root' "
        "      xmlns:x='http://example.com/xtree' "
        "      xmlns:y='http://example.com/xtree' "
        "      x:a='A' b='B' />"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::attribute_map& attrs = doc->root()->attrs();
        BOOST_CHECK_EQUAL(attrs.size(), 2U);
        unsigned int attr_count = 2U;

        std::pair<xtree::attribute_iterator, bool> inserted = attrs.insert("x:b", "B");
        ++attr_count;
        BOOST_CHECK_EQUAL(inserted.second, true);
        BOOST_CHECK_EQUAL(inserted.first->name(), "b");
        BOOST_CHECK_EQUAL(inserted.first->prefix(), "x");
        BOOST_CHECK_EQUAL(inserted.first->uri(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(inserted.first->value(), "B");

        inserted = attrs.insert("y:a", "X");
        BOOST_CHECK_EQUAL(inserted.second, false);
        BOOST_CHECK_EQUAL(inserted.first->name(), "a");
        BOOST_CHECK_EQUAL(inserted.first->prefix(), "x");
        BOOST_CHECK_EQUAL(inserted.first->uri(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(inserted.first->value(), "A");

        inserted = attrs.insert("a", "X");
        ++attr_count;
        BOOST_CHECK_EQUAL(inserted.second, true);
        BOOST_CHECK_EQUAL(inserted.first->name(), "a");
        BOOST_CHECK_EQUAL(inserted.first->prefix(), std::string());
        BOOST_CHECK_EQUAL(inserted.first->uri(), std::string());
        BOOST_CHECK_EQUAL(inserted.first->value(), "X");

        BOOST_CHECK_EQUAL(attrs.size(), attr_count);

        xtree::attribute_iterator updated = attrs.update("y:a", "A2");
        BOOST_CHECK_EQUAL(updated->name(), "a");
        BOOST_CHECK_EQUAL(updated->prefix(), "x");
        BOOST_CHECK_EQUAL(updated->uri(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(updated->value(), "A2");

        updated = attrs.update("x:b", "B2");
        BOOST_CHECK_EQUAL(updated->name(), "b");
        BOOST_CHECK_EQUAL(updated->prefix(), "x");
        BOOST_CHECK_EQUAL(updated->uri(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(updated->value(), "B2");

        updated = attrs.update("c", "C");
        ++attr_count;
        BOOST_CHECK_EQUAL(updated->name(), "c");
        BOOST_CHECK_EQUAL(updated->prefix(), std::string());
        BOOST_CHECK_EQUAL(updated->uri(), std::string());
        BOOST_CHECK_EQUAL(updated->value(), "C");

        updated = attrs.update("y:c", "C2");
        ++attr_count;
        BOOST_CHECK_EQUAL(updated->name(), "c");
        BOOST_CHECK_EQUAL(updated->prefix(), "y");
        BOOST_CHECK_EQUAL(updated->uri(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(updated->value(), "C2");

        BOOST_CHECK_EQUAL(attrs.size(), attr_count);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_attribute_map_erase)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://example.com/root' "
        "      xmlns:x='http://example.com/xtree' "
        "      xmlns:y='http://example.com/xtree' "
        "      x:a='A' y:b='B' c='C' />"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        // Erase an iterator.
        {
            std::auto_ptr<xtree::document> doc_tmp = xtree::clone_document(*doc);
            xtree::attribute_map& attrs = doc_tmp->root()->attrs();
            xtree::attribute_iterator next = attrs.erase(attrs.find("x:a"));
            BOOST_CHECK(next == attrs.find("x:b"));
            next = attrs.erase(next);
            BOOST_CHECK(next == attrs.find("c"));
            next = attrs.erase(next);
            BOOST_CHECK(next == attrs.end());
            BOOST_CHECK_EQUAL(attrs.empty(), true);
        }
        // Erase a range of attributes.
        {
            std::auto_ptr<xtree::document> doc_tmp = xtree::clone_document(*doc);
            xtree::attribute_map& attrs = doc_tmp->root()->attrs();
            xtree::attribute_iterator next = attrs.erase(attrs.begin(), attrs.find("c"));
            BOOST_CHECK(next == attrs.find("c"));
            BOOST_CHECK_EQUAL(attrs.size(), 1U);
            next = attrs.erase(next, attrs.end());
            BOOST_CHECK(next == attrs.end());
            BOOST_CHECK_EQUAL(attrs.empty(), true);
        }
        // Erase by QName or by local name and namespace URI.
        {
            std::auto_ptr<xtree::document> doc_tmp = xtree::clone_document(*doc);
            xtree::attribute_map& attrs = doc_tmp->root()->attrs();
            xtree::size_type erased = attrs.erase("x:b");
            BOOST_CHECK_EQUAL(erased, 1U);
            erased = attrs.erase("a");
            BOOST_CHECK_EQUAL(erased, 0U);
            erased = attrs.erase("a", "http://example.com/xtree");
            BOOST_CHECK_EQUAL(erased, 1U);
            erased = attrs.erase("c", "http://example.com/root");
            BOOST_CHECK_EQUAL(erased, 0U);
            erased = attrs.erase("c");
            BOOST_CHECK_EQUAL(erased, 1U);
            BOOST_CHECK_EQUAL(attrs.empty(), true);
        }
        BOOST_CHECK_EQUAL(doc->root()->attrs().size(), 3U);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


//
// Created by ZHENG Zhong on 2011-03-23.
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
#include <utility>


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_as_node)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root/>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::node_ptr ptr = doc->as_ptr();
        BOOST_CHECK_EQUAL(ptr->type(), xtree::document_node);
        BOOST_CHECK_EQUAL(ptr->name(), "#document");
        BOOST_CHECK_EQUAL(ptr->content(), std::string());
        BOOST_CHECK_EQUAL(ptr->path(), "/");
        BOOST_CHECK(&(ptr->doc()) == doc.get());
        BOOST_CHECK(ptr->parent() == 0);
        BOOST_CHECK_THROW(ptr->set_content("whatever"), xtree::bad_dom_operation);
        BOOST_CHECK_THROW(ptr.delete_(), xtree::bad_dom_operation);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_clone)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://example.com/xtree' attr='some value'>"
        "  <item index='zero'>0</item>"
        "  <item index='one'>1</item>"
        "  <item index='two'>2</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc1 = xtree::parse_string(TEST_XML);
        std::auto_ptr<xtree::document> doc2 = xtree::clone_document(*doc1);
        xtree::element_ptr root1 = doc1->root();
        xtree::element_ptr root2 = doc2->root();
        // Check cloned document and the original one are the same.
        BOOST_CHECK_EQUAL(root1->name(), root2->name());
        BOOST_CHECK_EQUAL(root1->prefix(), root2->prefix());
        BOOST_CHECK_EQUAL(root1->uri(), root2->uri());
        BOOST_CHECK_EQUAL(root1->attr("attr"), root2->attr("attr"));
        xtree::child_iterator i2 = root2->begin();
        for (xtree::child_iterator i1 = root1->begin(); i1 != root1->end(); ++i1)
        {
            BOOST_REQUIRE(i2 != root2->end());
            BOOST_CHECK_EQUAL(i1->type(), i2->type());
            BOOST_CHECK_EQUAL(i1->name(), i2->name());
            BOOST_CHECK_EQUAL(i1->content(), i2->content());
            if (i1->type() == xtree::element_node)
            {
                BOOST_REQUIRE(i2->type() == xtree::element_node);
                xtree::element_ptr e1 = xtree::dynamic_node_cast<xtree::element>(i1.ptr());
                xtree::element_ptr e2 = xtree::dynamic_node_cast<xtree::element>(i2.ptr());
                BOOST_CHECK_EQUAL(e1->prefix(), e2->prefix());
                BOOST_CHECK_EQUAL(e1->uri(), e2->uri());
                BOOST_CHECK_EQUAL(e1->attr("index"), e2->attr("index"));
            }
            ++i2;
        }
        BOOST_CHECK(i2 == root2->end());
        // Check modifying one document will not affect the other one.
        root1->clear();
        BOOST_CHECK_EQUAL(root1->size(), 0U);
        BOOST_CHECK_EQUAL(root2->size(), 3U);
        root2->set_name("not-root-anymore");
        BOOST_CHECK_EQUAL(root1->name(), "root");
        BOOST_CHECK_EQUAL(root2->name(), "not-root-anymore");
        root1->set_attr("attr", "updated value 1");
        root2->set_attr("attr", "updated value 2");
        BOOST_CHECK_EQUAL(root1->attr("attr"), "updated value 1");
        BOOST_CHECK_EQUAL(root2->attr("attr"), "updated value 2");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_reset_root_clone)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://example.com/xtree' attr='some value'>"
        "  <item>content</item>"
        "  <item>content</item>"
        "  <item>content</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document();
        {
            std::auto_ptr<xtree::document> doc2 = xtree::parse_string(TEST_XML);
            xtree::element_ptr root2 = doc2->root();
            xtree::element_ptr root = doc->reset_root_clone(*root2);
            // Check cloned document and the original one are the same.
            BOOST_CHECK_EQUAL(root->name(), root2->name());
            BOOST_CHECK_EQUAL(root->prefix(), root2->prefix());
            BOOST_CHECK_EQUAL(root->uri(), root2->uri());
            BOOST_CHECK_EQUAL(root->attr("attr"), root2->attr("attr"));
            BOOST_CHECK_EQUAL(root->size(), root2->size());
            // Check modifying one document will not affect the other one.
            root2->clear();
            BOOST_CHECK_EQUAL(root2->size(), 0U);
            BOOST_CHECK_EQUAL(root->size(), 3U);
            root->set_name("updated-root");
            BOOST_CHECK_EQUAL(root2->name(), "root");
            BOOST_CHECK_EQUAL(root->name(), "updated-root");
            root2->set_attr("attr", "updated value 2");
            root->set_attr("attr", "updated value");
            BOOST_CHECK_EQUAL(root2->attr("attr"), "updated value 2");
            BOOST_CHECK_EQUAL(root->attr("attr"), "updated value");
        }
        // At this point, doc2 should have been destroyed.
        xtree::element_ptr root = doc->root();
        BOOST_REQUIRE(root != 0);
        BOOST_CHECK_EQUAL(root->name(), "updated-root");
        BOOST_CHECK_EQUAL(root->prefix(), std::string());
        BOOST_CHECK_EQUAL(root->uri(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(root->attr("attr"), "updated value");
        BOOST_CHECK_EQUAL(root->size(), 3U);
        for (xtree::child_iterator i = root->begin(); i != root->end(); ++i)
        {
            BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(i->name(), "item");
            BOOST_CHECK_EQUAL(i->content(), "content");
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_reset_root_adopt)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://example.com/xtree' attr='some value'>"
        "  <item>content</item>"
        "  <item>content</item>"
        "  <item>content</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document();
        {
            std::auto_ptr<xtree::document> doc2 = xtree::parse_string(TEST_XML);
            xtree::element_ptr root2 = doc2->root();
            xtree::element_ptr root = doc->reset_root_adopt(*root2);
            BOOST_CHECK(doc2->root() == 0);
        }
        // At this point, doc2 should have been destroyed.
        xtree::element_ptr root = doc->root();
        BOOST_REQUIRE(root != 0);
        BOOST_CHECK_EQUAL(root->name(), "root");
        BOOST_CHECK_EQUAL(root->prefix(), std::string());
        BOOST_CHECK_EQUAL(root->uri(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(root->attr("attr"), "some value");
        BOOST_CHECK_EQUAL(root->size(), 3U);
        for (xtree::child_iterator i = root->begin(); i != root->end(); ++i)
        {
            BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(i->name(), "item");
            BOOST_CHECK_EQUAL(i->content(), "content");
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_reset_root_clone_with_xmlns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root xmlns='http://example.com/xtree'><sub a='A'>text</sub></root>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document();
        {
            std::auto_ptr<xtree::document> doc2 = xtree::parse_string(TEST_XML);
            xtree::element_ptr sub = doc2->root()->find_elem_by_name("sub");
            xtree::element_ptr root = doc->reset_root_clone(*sub);
            // Check cloned document root and the original element are the same.
            BOOST_CHECK_EQUAL(root->name(), sub->name());
            BOOST_CHECK_EQUAL(root->uri(), sub->uri());
            BOOST_CHECK_EQUAL(root->content(), sub->content());
            BOOST_CHECK_EQUAL(root->attr("a"), sub->attr("a"));
            BOOST_CHECK_EQUAL(root->size(), sub->size());
            // Check modifying one document will not affect the other one.
            sub->clear();
            BOOST_CHECK_EQUAL(sub->size(), 0U);
            BOOST_CHECK_EQUAL(root->size(), 1U);
            root->set_name("updated");
            BOOST_CHECK_EQUAL(sub->name(), "sub");
            BOOST_CHECK_EQUAL(root->name(), "updated");
            sub->set_attr("a", "B");
            root->set_attr("a", "C");
            BOOST_CHECK_EQUAL(sub->attr("a"), "B");
            BOOST_CHECK_EQUAL(root->attr("a"), "C");
        }
        // At this point, doc2 should have been destroyed.
        xtree::element_ptr root = doc->root();
        BOOST_REQUIRE(root != 0);
        BOOST_CHECK_EQUAL(root->name(), "updated");
        BOOST_CHECK_EQUAL(root->uri(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(root->content(), "text");
        BOOST_CHECK_EQUAL(root->attr("a"), "C");
        BOOST_CHECK_EQUAL(root->size(), 1U);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_reset_root_adopt_with_xmlns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root xmlns='http://example.com/xtree'><sub a='A'>text</sub></root>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document();
        {
            std::auto_ptr<xtree::document> doc2 = xtree::parse_string(TEST_XML);
            xtree::element_ptr sub = doc2->root()->find_elem_by_name("sub");
            xtree::element_ptr root = doc->reset_root_adopt(*sub);
            BOOST_CHECK_EQUAL(doc2->root()->empty(), true);
        }
        // At this point, doc2 should have been destroyed.
        xtree::element_ptr root = doc->root();
        BOOST_REQUIRE(root != 0);
        BOOST_CHECK_EQUAL(root->name(), "sub");
        BOOST_CHECK_EQUAL(root->uri(), "http://example.com/xtree");
        BOOST_CHECK_EQUAL(root->content(), "text");
        BOOST_CHECK_EQUAL(root->attr("a"), "A");
        BOOST_CHECK_EQUAL(root->size(), 1U);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_push)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root/>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        doc->push_front_comment("front comment");
        doc->push_front_instruction("target", "front");
        doc->push_back_comment("back comment");
        doc->push_back_instruction("target", "back");
        BOOST_CHECK_EQUAL(doc->size(), 5U);
        unsigned int index = 0;
        for (xtree::child_iterator i = doc->begin(); i != doc->end(); ++i, ++index)
        {
            if (index == 0U || index == 4U)
            {
                BOOST_CHECK_EQUAL(i->type(), xtree::instruction_node);
                BOOST_CHECK_EQUAL(i->name(), "target");
                BOOST_CHECK_EQUAL(i->content(), (index == 0U ? "front" : "back"));
            }
            else if (index == 1U || index == 3U)
            {
                BOOST_CHECK_EQUAL(i->type(), xtree::comment_node);
                BOOST_CHECK_EQUAL(i->content(), (index == 1U ? "front comment" : "back comment"));
            }
            else
            {
                BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
                BOOST_CHECK_EQUAL(i->name(), "root");
            }
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_push_failure)
{
    XTREE_LOG_TEST_NAME;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document();
        BOOST_CHECK_EQUAL(doc->empty(), true);
        BOOST_CHECK_EQUAL(doc->size(), 0U);
        // We should be able to push the very first element.
        doc->push_back_element("root");
        BOOST_CHECK_EQUAL(doc->size(), 1U);
        // But pushing the second element should fail.
        BOOST_CHECK_THROW(doc->push_back_element("root2"), xtree::bad_dom_operation);
        BOOST_CHECK_EQUAL(doc->size(), 1U);
        // After clearing the document, we can start again.
        doc->clear();
        BOOST_CHECK_EQUAL(doc->empty(), true);
        BOOST_CHECK_EQUAL(doc->size(), 0U);
        doc->push_front_element("root");
        BOOST_CHECK_EQUAL(doc->size(), 1U);
        BOOST_CHECK_THROW(doc->push_front_element("root2"), xtree::bad_dom_operation);
        BOOST_CHECK_EQUAL(doc->size(), 1U);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


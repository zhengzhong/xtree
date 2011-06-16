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

#include <xtree/xtree.hpp>

#include <memory>
#include <string>
#include <utility>


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_clone)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://www.example.com/xtree' attr='some value'>"
        "  <item index='zero'>0</item>"
        "  <item index='one'>1</item>"
        "  <item index='two'>2</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc1(xtree::parse_string(TEST_XML));
        std::auto_ptr<xtree::document> doc2(doc1->clone());
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


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_reset_root_clone)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://www.example.com/xtree' attr='some value'>"
        "  <item>0</item>"
        "  <item>1</item>"
        "  <item>2</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc1(xtree::parse_string(TEST_XML));
        std::auto_ptr<xtree::document> doc2(new xtree::document());
        xtree::element_ptr root1 = doc1->root();
        xtree::element_ptr root2 = doc2->reset_root_clone(*root1);
        // Check cloned document and the original one are the same.
        BOOST_CHECK_EQUAL(root1->name(), root2->name());
        BOOST_CHECK_EQUAL(root1->prefix(), root2->prefix());
        BOOST_CHECK_EQUAL(root1->uri(), root2->uri());
        BOOST_CHECK_EQUAL(root1->attr("attr"), root2->attr("attr"));
        BOOST_CHECK_EQUAL(root1->size(), root2->size());
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


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_reset_root_adopt)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://www.example.com/xtree' attr='some value'>"
        "  <item>0</item>"
        "  <item>1</item>"
        "  <item>2</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc1(xtree::parse_string(TEST_XML));
        std::auto_ptr<xtree::document> doc2(new xtree::document());
        xtree::element_ptr root1 = doc1->root();
        xtree::element_ptr root2 = doc2->reset_root_adopt(*root1);
        BOOST_CHECK_EQUAL(root2->name(), "root");
        BOOST_CHECK_EQUAL(root2->prefix(), std::string());
        BOOST_CHECK_EQUAL(root2->uri(), "http://www.example.com/xtree");
        BOOST_CHECK_EQUAL(root2->attr("attr"), "some value");
        BOOST_CHECK_EQUAL(root2->size(), 3U);
        BOOST_CHECK(doc1->root() == 0);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_push)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root/>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        doc->push_front_comment("front comment");
        doc->push_front_xml_pi("target", "front");
        doc->push_back_comment("back comment");
        doc->push_back_xml_pi("target", "back");
        BOOST_CHECK_EQUAL(doc->size(), 5U);
        unsigned int index = 0;
        for (xtree::child_iterator i = doc->begin(); i != doc->end(); ++i, ++index)
        {
            if (index == 0U || index == 4U)
            {
                BOOST_CHECK_EQUAL(i->type(), xtree::xml_pi_node);
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


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_document_push_failure)
{
    XTREE_LOG_TEST_NAME;
    try
    {
        std::auto_ptr<xtree::document> doc(new xtree::document());
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


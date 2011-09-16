//
// Created by ZHENG Zhong on 2011-06-20.
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


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_element_insert_clone)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML_1 =
        "<root>"
        "  <item>3</item>"
        "  <item>2</item>"
        "  <item>1</item>"
        "  <item>0</item>"
        "</root>"
    ;
    const char* TEST_XML_2 =
        "<root>"
        "  <item>4</item>"
        "  <item>5</item>"
        "  <item>6</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document("root");
        xtree::element_ptr root = doc->root();
        {
            // Test insert_clone() to begin position.
            std::auto_ptr<xtree::document> doc1 = xtree::parse_string(TEST_XML_1);
            xtree::element_ptr root1 = doc1->root();
            for (xtree::child_iterator i = root1->begin(); i != root1->end(); ++i)
            {
                xtree::child_iterator pos = root->insert_clone(root->begin(), *i);
                BOOST_CHECK(pos == root->begin());
            }
            BOOST_CHECK_EQUAL(root1->empty(), false);
            BOOST_CHECK_EQUAL(root1->size(), 4U);
            // Test insert_clone() to end position.
            std::auto_ptr<xtree::document> doc2 = xtree::parse_string(TEST_XML_2);
            xtree::element_ptr root2 = doc2->root();
            for (xtree::child_iterator i = root2->begin(); i != root2->end(); ++i)
            {
                xtree::child_iterator pos = root->insert_clone(root->end(), *i);
                BOOST_CHECK(++pos == root->end());
            }
            BOOST_CHECK_EQUAL(root2->empty(), false);
            BOOST_CHECK_EQUAL(root2->size(), 3U);
        }
        // At this point, all temporary documents should have been destroyed.
        BOOST_CHECK_EQUAL(root->size(), 7U);
        unsigned int index = 0;
        for (xtree::child_iterator i = root->begin(); i != root->end(); ++i, ++index)
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


BOOST_AUTO_TEST_CASE(test_element_insert_clone_range)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML_1 =
        "<root>"
        "  <item>0</item>"
        "  <item>1</item>"
        "  <item>2</item>"
        "  <item>3</item>"
        "</root>"
    ;
    const char* TEST_XML_2 =
        "<root>"
        "  <item>4</item>"
        "  <item>5</item>"
        "  <item>6</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document("root");
        xtree::element_ptr root = doc->root();
        {
            // Test insert_clone() to begin position.
            std::auto_ptr<xtree::document> doc1 = xtree::parse_string(TEST_XML_1);
            xtree::element_ptr root1 = doc1->root();
            root->insert_clone(root->begin(), root1->begin(), root1->end());
            BOOST_CHECK_EQUAL(root1->empty(), false);
            BOOST_CHECK_EQUAL(root1->size(), 4U);
            // Test insert_clone() to end position.
            std::auto_ptr<xtree::document> doc2 = xtree::parse_string(TEST_XML_2);
            xtree::element_ptr root2 = doc2->root();
            root->insert_clone(root->end(), root2->begin(), root2->end());
            BOOST_CHECK_EQUAL(root2->empty(), false);
            BOOST_CHECK_EQUAL(root2->size(), 3U);
        }
        // At this point, all temporary documents should have been destroyed.
        BOOST_CHECK_EQUAL(root->size(), 7U);
        unsigned int index = 0;
        for (xtree::child_iterator i = root->begin(); i != root->end(); ++i, ++index)
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


BOOST_AUTO_TEST_CASE(test_element_insert_adopt)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML_1 =
        "<root>"
        "  <item>3</item>"
        "  <item>2</item>"
        "  <item>1</item>"
        "  <item>0</item>"
        "</root>"
    ;
    const char* TEST_XML_2 =
        "<root>"
        "  <item>4</item>"
        "  <item>5</item>"
        "  <item>6</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document("root");
        xtree::element_ptr root = doc->root();
        {
            // Test insert_adopt() to begin position.
            std::auto_ptr<xtree::document> doc1 = xtree::parse_string(TEST_XML_1);
            xtree::element_ptr root1 = doc1->root();
            for (xtree::child_iterator i = root1->begin(); i != root1->end(); )
            {
                xtree::child_iterator pos = root->insert_adopt(root->begin(), *(i++));
                BOOST_CHECK(pos == root->begin());
            }
            BOOST_CHECK_EQUAL(root1->empty(), true);
            // Test insert_adopt() to end position.
            std::auto_ptr<xtree::document> doc2 = xtree::parse_string(TEST_XML_2);
            xtree::element_ptr root2 = doc2->root();
            for (xtree::child_iterator i = root2->begin(); i != root2->end(); )
            {
                xtree::child_iterator pos = root->insert_adopt(root->end(), *(i++));
                BOOST_CHECK(++pos == root->end());
            }
            BOOST_CHECK_EQUAL(root2->empty(), true);
        }
        // At this point, all temporary documents should have been destroyed.
        BOOST_CHECK_EQUAL(root->size(), 7U);
        unsigned int index = 0;
        for (xtree::child_iterator i = root->begin(); i != root->end(); ++i, ++index)
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


BOOST_AUTO_TEST_CASE(test_element_insert_adopt_range)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML_1 =
        "<root>"
        "  <item>0</item>"
        "  <item>1</item>"
        "  <item>2</item>"
        "  <item>3</item>"
        "</root>"
    ;
    const char* TEST_XML_2 =
        "<root>"
        "  <item>4</item>"
        "  <item>5</item>"
        "  <item>6</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document("root");
        xtree::element_ptr root = doc->root();
        {
            // Test insert_adopt() to begin position.
            std::auto_ptr<xtree::document> doc1 = xtree::parse_string(TEST_XML_1);
            xtree::element_ptr root1 = doc1->root();
            root->insert_adopt(root->begin(), root1->begin(), root1->end());
            BOOST_CHECK_EQUAL(root1->empty(), true);
            // Test insert_adopt() to end position.
            std::auto_ptr<xtree::document> doc2 = xtree::parse_string(TEST_XML_2);
            xtree::element_ptr root2 = doc2->root();
            root->insert_adopt(root->end(), root2->begin(), root2->end());
            BOOST_CHECK_EQUAL(root2->empty(), true);
        }
        // At this point, all temporary documents should have been destroyed.
        BOOST_CHECK_EQUAL(root->size(), 7U);
        unsigned int index = 0;
        for (xtree::child_iterator i = root->begin(); i != root->end(); ++i, ++index)
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


BOOST_AUTO_TEST_CASE(test_element_insert_with_xmlns)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML_1 =
        "<root xmlns='http://example.com/xtree/1'>"
        "  <item>3</item>"
        "  <item>2</item>"
        "  <item>1</item>"
        "  <item>0</item>"
        "</root>"
        ;
    const char* TEST_XML_2 =
        "<root xmlns='http://example.com/xtree/2'>"
        "  <item>4</item>"
        "  <item>5</item>"
        "  <item>6</item>"
        "</root>"
        ;
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document("root");
        xtree::element_ptr root = doc->root();
        {
            // Use insert_clone() for the first document.
            std::auto_ptr<xtree::document> doc1 = xtree::parse_string(TEST_XML_1);
            xtree::element_ptr root1 = doc1->root();
            for (xtree::child_iterator i = root1->begin(); i != root1->end(); ++i)
            {
                xtree::child_iterator pos = root->insert_clone(root->begin(), *i);
                BOOST_CHECK(pos == root->begin());
            }
            BOOST_CHECK_EQUAL(root1->size(), 4U);
            // Use insert_adopt for the second document.
            std::auto_ptr<xtree::document> doc2 = xtree::parse_string(TEST_XML_2);
            xtree::element_ptr root2 = doc2->root();
            for (xtree::child_iterator i = root2->begin(); i != root2->end(); )
            {
                xtree::child_iterator pos = root->insert_adopt(root->end(), *(i++));
                BOOST_CHECK(++pos == root->end());
            }
            BOOST_CHECK_EQUAL(root2->empty(), true);
        }
        // At this point, all temporary documents should have been destroyed.
        BOOST_CHECK_EQUAL(root->size(), 7U);
        unsigned int index = 0;
        for (xtree::child_iterator i = root->begin(); i != root->end(); ++i, ++index)
        {
            xtree::element_ptr e = xtree::dynamic_node_cast<xtree::element>(i.ptr());
            BOOST_REQUIRE(e != 0);
            BOOST_CHECK_EQUAL(e->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(e->name(), "item");
            BOOST_CHECK_EQUAL(e->content(), boost::lexical_cast<std::string>(index));
            if (index <= 3)
            {
                BOOST_CHECK_EQUAL(e->uri(), "http://example.com/xtree/1");
            }
            else
            {
                BOOST_CHECK_EQUAL(e->uri(), "http://example.com/xtree/2");
            }
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}




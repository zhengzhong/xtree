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

#include <xtree/xtree.hpp>

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
        std::auto_ptr<xtree::document> doc(new xtree::document());
        xtree::element_ptr root = doc->reset_root("root");
        {
            // Test insert_clone() to begin position.
            std::auto_ptr<xtree::document> doc_tmp1 = xtree::parse_string(TEST_XML_1);
            xtree::element_ptr root_tmp = doc_tmp1->root();
            for (xtree::child_iterator i = root_tmp->begin(); i != root_tmp->end(); ++i)
            {
                xtree::child_iterator pos = root->insert_clone(root->begin(), *i);
                BOOST_CHECK(pos == root->begin());
            }
            BOOST_CHECK_EQUAL(root_tmp->empty(), false);
            BOOST_CHECK_EQUAL(root_tmp->size(), 4U);
            // Test insert_clone() to end position.
            std::auto_ptr<xtree::document> doc_tmp2 = xtree::parse_string(TEST_XML_2);
            root_tmp = doc_tmp2->root();
            for (xtree::child_iterator i = root_tmp->begin(); i != root_tmp->end(); ++i)
            {
                xtree::child_iterator pos = root->insert_clone(root->end(), *i);
                BOOST_CHECK(++pos == root->end());
            }
            BOOST_CHECK_EQUAL(root_tmp->empty(), false);
            BOOST_CHECK_EQUAL(root_tmp->size(), 3U);
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
        std::auto_ptr<xtree::document> doc(new xtree::document());
        xtree::element_ptr root = doc->reset_root("root");
        {
            // Test insert_clone() to begin position.
            std::auto_ptr<xtree::document> doc_tmp1 = xtree::parse_string(TEST_XML_1);
            xtree::element_ptr root_tmp = doc_tmp1->root();
            root->insert_clone(root->begin(), root_tmp->begin(), root_tmp->end());
            BOOST_CHECK_EQUAL(root_tmp->empty(), false);
            BOOST_CHECK_EQUAL(root_tmp->size(), 4U);
            // Test insert_clone() to end position.
            std::auto_ptr<xtree::document> doc_tmp2 = xtree::parse_string(TEST_XML_2);
            root_tmp = doc_tmp2->root();
            root->insert_clone(root->end(), root_tmp->begin(), root_tmp->end());
            BOOST_CHECK_EQUAL(root_tmp->empty(), false);
            BOOST_CHECK_EQUAL(root_tmp->size(), 3U);
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
        std::auto_ptr<xtree::document> doc(new xtree::document());
        xtree::element_ptr root = doc->reset_root("root");
        {
            // Test insert_adopt() to begin position.
            std::auto_ptr<xtree::document> doc_tmp1 = xtree::parse_string(TEST_XML_1);
            xtree::element_ptr root_tmp = doc_tmp1->root();
            for (xtree::child_iterator i = root_tmp->begin(); i != root_tmp->end(); )
            {
                xtree::child_iterator pos = root->insert_adopt(root->begin(), *(i++));
                BOOST_CHECK(pos == root->begin());
            }
            BOOST_CHECK_EQUAL(root_tmp->empty(), true);
            // Test insert_adopt() to end position.
            std::auto_ptr<xtree::document> doc_tmp2 = xtree::parse_string(TEST_XML_2);
            root_tmp = doc_tmp2->root();
            for (xtree::child_iterator i = root_tmp->begin(); i != root_tmp->end(); )
            {
                xtree::child_iterator pos = root->insert_adopt(root->end(), *(i++));
                BOOST_CHECK(++pos == root->end());
            }
            BOOST_CHECK_EQUAL(root_tmp->empty(), true);
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
        std::auto_ptr<xtree::document> doc(new xtree::document());
        xtree::element_ptr root = doc->reset_root("root");
        {
            // Test insert_adopt() to begin position.
            std::auto_ptr<xtree::document> doc_tmp1 = xtree::parse_string(TEST_XML_1);
            xtree::element_ptr root_tmp = doc_tmp1->root();
            root->insert_adopt(root->begin(), root_tmp->begin(), root_tmp->end());
            BOOST_CHECK_EQUAL(root_tmp->empty(), true);
            // Test insert_adopt() to end position.
            std::auto_ptr<xtree::document> doc_tmp2 = xtree::parse_string(TEST_XML_2);
            root_tmp = doc_tmp2->root();
            root->insert_adopt(root->end(), root_tmp->begin(), root_tmp->end());
            BOOST_CHECK_EQUAL(root_tmp->empty(), true);
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


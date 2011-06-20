//
// Created by ZHENG Zhong on 2011-05-09.
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


BOOST_AUTO_TEST_CASE(test_element_push_elements)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root/>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        BOOST_REQUIRE(root != 0);
        xtree::element_ptr e1 = root->push_back_element("e1");
        BOOST_CHECK_EQUAL(e1->name(), "e1");
        BOOST_CHECK_EQUAL(e1->uri(), std::string());
        BOOST_CHECK_EQUAL(e1->content(), std::string());
        BOOST_CHECK_EQUAL(root->size(), 1U);
        xtree::element_ptr e2 = root->push_back_element("e2", "http://www.example.com");
        BOOST_CHECK_EQUAL(e2->name(), "e2");
        BOOST_CHECK_EQUAL(e2->uri(), "http://www.example.com");
        BOOST_CHECK_EQUAL(e2->content(), std::string());
        BOOST_CHECK_EQUAL(root->size(), 2U);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_element_push_texts)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root/>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        BOOST_REQUIRE(root != 0);
        xtree::text_ptr text1 = root->push_back_text("hello,");
        BOOST_CHECK_EQUAL(text1->content(), "hello,");
        BOOST_CHECK_EQUAL(root->size(), 1U);
        xtree::text_ptr text2 = root->push_back_text("world!");
        BOOST_CHECK_EQUAL(text1, text2);
        BOOST_CHECK_EQUAL(text2->content(), "hello,world!");
        BOOST_CHECK_EQUAL(text1->content(), "hello,world!");
        BOOST_CHECK_EQUAL(root->size(), 1U);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_element_push_back_adopt)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root/>";
    const char* TEST_XML_2 = "<item>content</item>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        // Test push_back_adopt().
        const unsigned int MAX_SIZE = 4;
        for (unsigned int i = 0; i < MAX_SIZE; ++i)
        {
            std::auto_ptr<xtree::document> doc_tmp(xtree::parse_string(TEST_XML_2));
            xtree::element_ptr root_tmp = doc_tmp->root();
            root_tmp->set_content(boost::lexical_cast<std::string>(i));
            xtree::child_node_ptr child = root->push_back_adopt(*root_tmp);
            BOOST_CHECK_EQUAL(doc_tmp->empty(), true);
            BOOST_CHECK_EQUAL(child->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(child->name(), "item");
            BOOST_CHECK_EQUAL(child->content(), boost::lexical_cast<std::string>(i));
        }
        // At this point, all the temporary documents should have been destroyed.
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


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_element_push_front_adopt)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root/>";
    const char* TEST_XML_2 = "<item>content</item>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        // Test push_front_adopt().
        const unsigned int MAX_SIZE = 4;
        for (unsigned int i = 0; i < MAX_SIZE; ++i)
        {
            std::auto_ptr<xtree::document> doc_tmp(xtree::parse_string(TEST_XML_2));
            xtree::element_ptr root_tmp = doc_tmp->root();
            root_tmp->set_content(boost::lexical_cast<std::string>(i));
            xtree::child_node_ptr child = root->push_front_adopt(*root_tmp);
            BOOST_CHECK_EQUAL(doc_tmp->empty(), true);
            BOOST_CHECK_EQUAL(child->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(child->name(), "item");
            BOOST_CHECK_EQUAL(child->content(), boost::lexical_cast<std::string>(i));
        }
        // At this point, all the temporary documents should have been destroyed.
        unsigned int index = MAX_SIZE - 1;
        for (xtree::child_iterator i = root->begin(); i != root->end(); ++i, --index)
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


//
// Created by ZHENG Zhong on 2011-05-09.
//

#include "xtree_test_utils.hpp"

#include <xtree/xtree_dom.hpp>

#include <memory>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_bad_names_for_attribute_and_element)
{
    XTREE_LOG_TEST_NAME;
    const std::string BAD_NAMES[] = {
        std::string(),
        "bad name",
        "bad\tname",
        "bad\nname",
        "123",
        "5name",
        "-name",
        ".name",
        " name",
        "name ",
    };
    const unsigned int MAX_SIZE = sizeof(BAD_NAMES) / sizeof(const std::string);
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document();
        for (unsigned int i = 0; i < MAX_SIZE; ++i)
        {
            BOOST_CHECK_THROW(doc->push_back_element(BAD_NAMES[i]), xtree::bad_dom_operation);
        }
        xtree::element_ptr root = doc->push_back_element("root");
        for (unsigned int i = 0; i < MAX_SIZE; ++i)
        {
            BOOST_CHECK_THROW(root->set_name(BAD_NAMES[i]), xtree::bad_dom_operation);
            BOOST_CHECK_THROW(root->set_attr(BAD_NAMES[i], "x"), xtree::bad_dom_operation);
            BOOST_CHECK_THROW(root->push_back_element(BAD_NAMES[i]), xtree::bad_dom_operation);
        }
        BOOST_CHECK_EQUAL(root->name(), "root");
        BOOST_CHECK_EQUAL(root->attrs().empty(), true);
        BOOST_CHECK_EQUAL(root->empty(), true);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_bad_uris)
{
    XTREE_LOG_TEST_NAME;
    const std::string BAD_URIS[] = {
        std::string(),
        "bad uri",
        "bad\turi",
        "bad\nuri",
        " uri",
        "uri ",
    };
    const unsigned int MAX_SIZE = sizeof(BAD_URIS) / sizeof(const std::string);
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::create_document();
        for (unsigned int i = 0; i < MAX_SIZE; ++i)
        {
            BOOST_CHECK_THROW(doc->push_back_element("e", BAD_URIS[i]), xtree::bad_dom_operation);
        }
        xtree::element_ptr root = doc->push_back_element("root");
        for (unsigned int i = 0; i < MAX_SIZE; ++i)
        {
            BOOST_CHECK_THROW(root->push_back_element("e", BAD_URIS[i]), xtree::bad_dom_operation);
            BOOST_CHECK_THROW(root->declare_xmlns("ns", BAD_URIS[i]), xtree::bad_dom_operation);
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}

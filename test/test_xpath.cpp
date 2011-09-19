//
// Created by ZHENG Zhong on 2011-09-16.
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


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_eval_xpath_result)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root><item>100</item><item>foo</item></root>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        xtree::xpath_result result;
        // Evaluate XPath on document.
        doc->eval("count(//item)=2", result);
        BOOST_CHECK_EQUAL(result.type(), xtree::boolean_result);
        doc->eval("number(//item[1])", result);
        BOOST_CHECK_EQUAL(result.type(), xtree::number_result);
        doc->eval("string(//item[2])", result);
        BOOST_CHECK_EQUAL(result.type(), xtree::string_result);
        // Evaluate XPath on element.
        root->eval("count(item)=2", result);
        BOOST_CHECK_EQUAL(result.type(), xtree::boolean_result);
        root->eval("number(item[1])", result);
        BOOST_CHECK_EQUAL(result.type(), xtree::number_result);
        root->eval("string(item[2])", result);
        BOOST_CHECK_EQUAL(result.type(), xtree::string_result);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_eval_typed_results)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root><item>100</item><item>foo</item></root>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        // Evaluate XPath on document.
        BOOST_CHECK_EQUAL(doc->eval_boolean("true()"), true);
        BOOST_CHECK_EQUAL(doc->eval_boolean("false()"), false);
        BOOST_CHECK_EQUAL(doc->eval_boolean("count(//item)=2"), true);
        BOOST_CHECK_EQUAL(doc->eval_number("number(//item[1])"), 100);
        BOOST_CHECK_EQUAL(doc->eval_number("count(//item)"), 2);
        BOOST_CHECK_EQUAL(doc->eval_string("string(//item[2])"), "foo");
        BOOST_CHECK_EQUAL(doc->eval_string("substring(//item[2], 2)"), "oo");
        // Evaluate XPath on element.
        BOOST_CHECK_EQUAL(root->eval_boolean("true()"), true);
        BOOST_CHECK_EQUAL(root->eval_boolean("false()"), false);
        BOOST_CHECK_EQUAL(root->eval_boolean("count(item)=2"), true);
        BOOST_CHECK_EQUAL(root->eval_number("number(item[1])"), 100);
        BOOST_CHECK_EQUAL(root->eval_number("count(//item)"), 2);
        BOOST_CHECK_EQUAL(root->eval_string("string(item[2])"), "foo");
        BOOST_CHECK_EQUAL(root->eval_string("substring(//item[2], 2)"), "oo");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_eval_xpath_error)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root><item>100</item><item>foo</item></root>";
    try
    {
        std::auto_ptr<xtree::document> doc = xtree::parse_string(TEST_XML);
        xtree::element_ptr root = doc->root();
        // Test XPath error on document.
        BOOST_CHECK_THROW(doc->eval_boolean("number(//item[1])"), xtree::xpath_error);
        BOOST_CHECK_THROW(doc->eval_number("//item"), xtree::xpath_error);
        // Test XPath error on element.
        BOOST_CHECK_THROW(root->eval_boolean("number(item[1])"), xtree::xpath_error);
        BOOST_CHECK_THROW(root->eval_number("item"), xtree::xpath_error);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_bad_xpaths)
{
    XTREE_LOG_TEST_NAME;
    try
    {
        BOOST_CHECK_THROW(xtree::xpath("bad XPath expression"), xtree::xpath_error);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


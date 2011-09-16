//
// Created by ZHENG Zhong on 2011-03-18.
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

#include <iterator>
#include <memory>
#include <string>
#include <utility>


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_subelement_iterator)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root>"
        "<item>one</item>two"
        "<item>three</item>four"
        "<item>five</item>six"
        "<item>seven</item>eight"
        "<item>nine</item>ten"
        "</root>"
    ;
    const std::pair<std::string, std::string> EXPECTED[] = {
        std::make_pair("one", "two"),
        std::make_pair("three", "four"),
        std::make_pair("five", "six"),
        std::make_pair("seven", "eight"),
        std::make_pair("nine", "ten"),
    };
    const unsigned int MAX_SIZE = sizeof(EXPECTED)
                                / sizeof(const std::pair<std::string, std::string>);
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        // Test iterating from begin to end.
        unsigned int index = 0;
        xtree::subelement_iterator last;
        xtree::subelement_iterator end;
        for (xtree::subelement_iterator i(root); i != end; ++i, ++index)
        {
            last = i;
            BOOST_REQUIRE(index < MAX_SIZE);
            BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(i->name(), "item");
            BOOST_CHECK_EQUAL(i->content(), EXPECTED[index].first);
            BOOST_CHECK_EQUAL(i->tail(), EXPECTED[index].second);
            if (index == 0)
            {
                BOOST_CHECK_EQUAL(i->head(), std::string());
            }
            else
            {
                BOOST_CHECK_EQUAL(i->head(), EXPECTED[index - 1].second);
            }
        }
        // Test iterating in reversed order.
        xtree::subelement_iterator begin(root);
        xtree::subelement_iterator current = last;
        for (index = MAX_SIZE - 1; static_cast<int>(index) >= 0; --index)
        {
            BOOST_CHECK_EQUAL(current->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(current->name(), "item");
            BOOST_CHECK_EQUAL(current->content(), EXPECTED[index].first);
            BOOST_CHECK_EQUAL(current->tail(), EXPECTED[index].second);
            if (current != begin)
            {
                --current;
            }
            else
            {
                BOOST_CHECK_EQUAL(index, 0U);
            }
        }
        // Test iterator category with std::advance().
        xtree::subelement_iterator x(root);
        std::advance(x, 4);
        BOOST_CHECK_EQUAL(x->content(), EXPECTED[4].first);
        BOOST_CHECK_EQUAL(x->tail(), EXPECTED[4].second);
        std::advance(x, -2);
        BOOST_CHECK_EQUAL(x->content(), EXPECTED[2].first);
        BOOST_CHECK_EQUAL(x->tail(), EXPECTED[2].second);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_subelement_iterator_conversion)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root><item/><item/></root>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        xtree::subelement_iterator i(root);
        xtree::const_subelement_iterator ci1(root);
        xtree::const_subelement_iterator ci2 = i;
        BOOST_CHECK(i == ci1);
        BOOST_CHECK(ci2 == i);
        BOOST_CHECK(ci1 == ci2);
        ++i;
        ++ci1;
        ++ci2;
        BOOST_CHECK(i == ci1);
        BOOST_CHECK(ci2 == i);
        BOOST_CHECK(ci1 == ci2);
        // Following code should not compile.
        //xtree::subelement_iterator i2 = ci1;
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_subelement_iterator_errors)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root><item/><item/></root>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        xtree::subelement_iterator begin(root);
        BOOST_CHECK_THROW(--begin, xtree::bad_dom_operation);
        xtree::subelement_iterator end;
        xtree::subelement_iterator i(root);
        for (; i != end; ++i)
        {
            // Do nothing.
        }
        BOOST_CHECK_THROW(++i, xtree::bad_dom_operation);
        BOOST_CHECK_THROW(--end, xtree::bad_dom_operation);
        BOOST_CHECK_THROW(++end, xtree::bad_dom_operation);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_df_element_iterator)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root>"
        "  <e0>"
        "    <e1/>"
        "    <e2>"
        "      <e3>text</e3>"
        "      <e4>text</e4>"
        "    </e2>"
        "    <e5>text</e5>"
        "    <e6>"
        "      <e7>"
        "        <e8/>"
        "        <e9>text</e9>"
        "      </e7>"
        "    </e6>"
        "  </e0>"
        "  <should-not-reach-here/>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr tree = doc->root()->find_first_elem();
        // Test iterating from begin to end.
        int index = 0;
        xtree::df_element_iterator last;
        xtree::df_element_iterator end;
        for (xtree::df_element_iterator i(tree); i != end; ++i, ++index)
        {
            last = i;
            BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(i->name(), "e" + boost::lexical_cast<std::string>(index));
        }
        // Test iterating in reversed order.
        xtree::df_element_iterator begin(tree);
        xtree::df_element_iterator current = last;
        for (--index; index >= 0; --index)
        {
            BOOST_CHECK_EQUAL(current->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(current->name(), "e" + boost::lexical_cast<std::string>(index));
            if (current != begin)
            {
                --current;
            }
            else
            {
                BOOST_CHECK_EQUAL(index, 0);
            }
        }
        // Test iterator category with std::advance().
        xtree::df_element_iterator x(tree);
        std::advance(x, 5);
        BOOST_CHECK_EQUAL(x->name(), "e5");
        std::advance(x, -2);
        BOOST_CHECK_EQUAL(x->name(), "e3");
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_df_element_iterator_errors)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root><item><item/></item><item/></root>";
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        xtree::df_element_iterator begin(root);
        BOOST_CHECK_THROW(--begin, xtree::bad_dom_operation);
        xtree::df_element_iterator end;
        xtree::df_element_iterator i(root);
        for (; i != end; ++i)
        {
            // Do nothing.
        }
        BOOST_CHECK_THROW(++i, xtree::bad_dom_operation);
        BOOST_CHECK_THROW(--end, xtree::bad_dom_operation);
        BOOST_CHECK_THROW(++end, xtree::bad_dom_operation);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}



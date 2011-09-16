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


namespace {


    //! This function tests the iterator category (required by std::advance).
    //! The Iterator template parameter could be child_iterator or const_child_iterator.
    template<typename Iterator>
    void do_test_advance(Iterator begin)
    {
        BOOST_CHECK_EQUAL(begin->content(), "0");
        std::advance(begin, 4);
        BOOST_CHECK_EQUAL(begin->content(), "4");
        std::advance(begin, -3);
        BOOST_CHECK_EQUAL(begin->content(), "1");
    }


}  // anonymous namespace


///////////////////////////////////////////////////////////////////////////////////////////////////


namespace {


    //! The Iterator template parameter could be child_iterator or const_child_iterator.
    template<typename Iterator>
    void do_test_child_iterater(Iterator begin, Iterator end)
    {
        unsigned int index = 0;
        for (Iterator i = begin; i != end; ++i, ++index)
        {
            BOOST_CHECK_EQUAL(i->type(), xtree::element_node);
            BOOST_CHECK_EQUAL(i->name(), "item");
            BOOST_CHECK_EQUAL(i->content(), boost::lexical_cast<std::string>(index));
        }
    }


}  // anonymous namespace


BOOST_AUTO_TEST_CASE(test_element_child_iterater)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root>"
        "  <item>0</item>"
        "  <item>1</item>"
        "  <item>2</item>"
        "  <item>3</item>"
        "  <item>4</item>"
        "  <item>5</item>"
        "</root>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        do_test_child_iterater<xtree::child_iterator>(root->begin(), root->end());
        do_test_child_iterater<xtree::const_child_iterator>(root->begin(), root->end());
        do_test_advance<xtree::child_iterator>(root->begin());
        do_test_advance<xtree::const_child_iterator>(root->begin());
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


namespace {


    //! The Iterator template parameter could be child_iterator or const_child_iterator.
    template<typename Iterator>
    void do_test_child_iterater_2(Iterator begin,
                                  Iterator end,
                                  const xtree::node_t types[],
                                  unsigned int max_size)
    {
        unsigned int index = 0;
        for (Iterator i = begin; i != end; ++i, ++index)
        {
            BOOST_REQUIRE(index < max_size);
            BOOST_CHECK_EQUAL(i->type(), types[index]);
            BOOST_CHECK_EQUAL(i->content(), boost::lexical_cast<std::string>(index));
            if (i->type() == xtree::element_node)
            {
                BOOST_CHECK_EQUAL(i->name(), "element");
            }
            else if (i->type() == xtree::instruction_node)
            {
                BOOST_CHECK_EQUAL(i->name(), "processing-instruction");
            }
        }
    }


}  // anonymous namespace


BOOST_AUTO_TEST_CASE(test_element_child_iterater_2)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root>"
        "0"
        "<element>1</element>"
        "2"
        "<![CDATA[3]]>"
        "<!--4-->"
        "<?processing-instruction 5?>"
        "<element>6</element>"
        "</root>";
    const xtree::node_t TYPES[] = {
        xtree::text_node,
        xtree::element_node,
        xtree::text_node,
        xtree::cdata_node,
        xtree::comment_node,
        xtree::instruction_node,
        xtree::element_node,
    };
    const unsigned int MAX_SIZE = sizeof(TYPES) / sizeof(xtree::node_t);
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        do_test_child_iterater_2<xtree::child_iterator>(
            root->begin(), root->end(), TYPES, MAX_SIZE
        );
        do_test_child_iterater_2<xtree::const_child_iterator>(
            root->begin(), root->end(), TYPES, MAX_SIZE
        );
        do_test_advance<xtree::child_iterator>(root->begin());
        do_test_advance<xtree::const_child_iterator>(root->begin());
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}



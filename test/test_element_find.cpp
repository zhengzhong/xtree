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


BOOST_AUTO_TEST_CASE(test_element_find_elem_by_path)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns:x='http://www.example.com/xtree'>"
        "  <foo><bar>BAR</bar><zero>0</zero></foo>"
        "  <x:foo><x:bar>BAR</x:bar><x:one>1</x:one></x:foo>"
        "  <x:foo><bar>BAR</bar><two>2</two></x:foo>"
        "  <foo><bar>BAR</bar></foo>"
        "  <foo><three>3</three></foo>"
        "  <foo><three>BAR</three></foo>"
        "</root>"
    ;
    const std::string PATHS[] = {
        "foo/zero",
        "foo/one",
        "foo/two",
        "foo/three",
    };
    const unsigned int MAX_SIZE = sizeof(PATHS) / sizeof(const std::string);
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        for (unsigned int i = 0; i < MAX_SIZE; ++i)
        {
            std::string::size_type pos = PATHS[i].find_last_of('/');
            BOOST_REQUIRE(pos != std::string::npos && pos + 1 < PATHS[i].size());
            std::string name = PATHS[i].substr(pos + 1);
            xtree::element_ptr e = root->find_elem_by_path(PATHS[i]);
            BOOST_REQUIRE(e != 0);
            BOOST_CHECK_EQUAL(e->name(), name);
            BOOST_CHECK_EQUAL(e->content(), boost::lexical_cast<std::string>(i));
        }
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


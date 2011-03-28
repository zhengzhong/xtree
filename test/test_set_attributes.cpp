//
// Created by ZHENG Zhong on 2011-03-16.
//

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4127)
#endif

#include <boost/test/auto_unit_test.hpp>

#ifdef _MSC_VER
#  pragma warning(pop)
#endif

#include <xtree/xtree.hpp>

#include <iostream>
#include <memory>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_set_attributes)
{
    std::cout << "test_set_attributes\n";
    const char* TEST_XML =
        "<root a='dummy' b='tom &amp; jerry' c='mickey'/>"
    ;
    try
    {
        std::auto_ptr<xtree::document> doc(xtree::parse_string(TEST_XML));
        xtree::element_ptr root = doc->root();
        root->set_attr("x", "http://www.google.com", "google");
        std::cout << doc->str() << std::endl;
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}



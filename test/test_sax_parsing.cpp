//
// Created by ZHENG Zhong on 2011-05-12.
//

#include "xtree_test_utils.hpp"

#include <xtree/xtree.hpp>

#include <iostream>
#include <memory>
#include <string>


////////////////////////////////////////////////////////////////////////////////////////////////////


namespace {


    class dummy_content_hander: public xtree::sax_content_handler
    {

    public:

        void begin_document()
        {
            std::cout << "begin document\n";
        }

        void end_document()
        {
            std::cout << "end document\n";
        }

        void begin_xmlns(const std::string& prefix, const std::string& uri)
        {
            std::cout << "begin_xmlns: " << prefix << " -> " << uri << "\n";
        }

        void end_xmlns(const std::string& prefix)
        {
            std::cout << "end_xmlns: " << prefix << "\n";
        }

        void begin_element(const std::string& name,
                           const std::string& uri,
                           const std::string& qname,
                           const xtree::sax_attribute_list& attrs)
        {
            std::cout << "begin_element: " << name << ", " << uri << ", " << qname << "\n";
            for (xtree::sax_attribute_list::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
            {
                std::cout << "  @" << i->name() << ", " << i->uri() << ", " << i->prefix() << ", " << i->qname() << " = " << i->value() << "\n";
            }
        }

        void end_element(const std::string& name,
                         const std::string& uri,
                         const std::string& qname)
        {
            std::cout << "end_element: " << name << ", " << uri << ", " << qname << "\n";
        }

        void characters(const char* chars, int length)
        {
            std::cout << "characters: ";
            for (int i = 0; i < length; ++i)
            {
                std::cout << chars[i];
            }
            std::cout << "\n";
        }

        void cdata_block(const char* chars, int length)
        {
            std::cout << "cdata_block: ";
            for (int i = 0; i < length; ++i)
            {
                std::cout << chars[i];
            }
            std::cout << "\n";
        }

        void ignorable_whitespace(const char* chars, int length)
        {
            chars;  // remove compiler warning.
            std::cout << "ignorable_whitespace: ";
            for (int i = 0; i < length; ++i)
            {
                std::cout << ".";
            }
            std::cout << "\n";
        }

        void comment(const char* chars)
        {
            std::cout << "comment: " << chars << "\n";
        }

    };


}  // anonymous namespace


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_sax_parsing_no_handler)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML = "<root a='A' xmlns='http://www.example.com/xtree'>hello,world!</root>";
    try
    {
        xtree::sax_parser parser;
        parser.parse_string(TEST_XML);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


BOOST_AUTO_TEST_CASE(test_sax_parsing_2)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://www.example.com/xtree' xmlns:x='http://www.example.com/xtree/x'>"
        "  <x:sub xmlns:x='http://www.example.com/xtree/x2'>"
        "    <x:subsub a='A' x:b='XB'>hello,world</x:subsub>"
        "  </x:sub>"
        "</root>"
    ;
    try
    {
        xtree::sax_parser parser;
        dummy_content_hander handler;
        parser.set_content_handler(&handler);
        parser.parse_string(TEST_XML);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


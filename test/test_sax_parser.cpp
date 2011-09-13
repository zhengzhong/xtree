//
// Created by ZHENG Zhong on 2011-05-12.
//

#include "xtree_test_utils.hpp"

#include <xtree/xtree_sax.hpp>

#include <memory>
#include <string>


////////////////////////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_sax_parser_no_handler)
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


////////////////////////////////////////////////////////////////////////////////////////////////////


namespace {


    static const std::string EXPECTED_[] = {
        "start_document: ",
        "start_element: {http://example.com/xtree}:root",
        "start_element: {http://example.com/xtree/x1}x:sub1",
        "start_element: {http://example.com/xtree/x2}x:sub2",
        "start_element: {http://example.com/xtree/x2}x:sub3",
        "attribute: {}@:a=A",
        "attribute: {http://example.com/xtree/x2}@x:b=B",
        "characters: hello,world",
        "end_element: {http://example.com/xtree/x2}x:sub3",
        "end_element: {http://example.com/xtree/x2}x:sub2",
        "end_element: {http://example.com/xtree/x1}x:sub1",
        "end_element: {http://example.com/xtree}:root",
        "end_document: ",
    };

    static const unsigned int MAX_INDEX_ = sizeof(EXPECTED_) / sizeof(const std::string);


    class dummy_content_handler: public xtree::sax_content_handler
    {

    public:

        dummy_content_handler(): index_(0)
        {
            // Do nothing.
        }

        void start_document()
        {
            check_("start_document: ");
        }

        void end_document()
        {
            check_("end_document: ");
            BOOST_CHECK_EQUAL(index_, MAX_INDEX_);
        }

        void start_element(const std::string& name,
                           const std::string& prefix,
                           const std::string& uri,
                           const xtree::sax_attribute_list& attrs)
        {
            check_("start_element: {" + uri + "}" + prefix + ":" + name);
            for (xtree::sax_attribute_list::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
            {
                check_( "attribute: {" + i->uri() + "}@" + i->prefix() + ":" + i->name()
                      + "=" + i->value() );
            }
        }

        void end_element(const std::string& name,
                         const std::string& prefix,
                         const std::string& uri)
        {
            check_("end_element: {" + uri + "}" + prefix + ":" + name);
        }

        void characters(const char* chars, int length)
        {
            check_("characters: " + std::string(chars, length));
        }

        void cdata_block(const char* chars, int length)
        {
            check_("cdata_block: " + std::string(chars, length));
        }

        void comment(const char* chars)
        {
            check_("comment: " + std::string(chars));
        }

    private:

        void check_(const std::string& got)
        {
            BOOST_REQUIRE(index_ < MAX_INDEX_);
            BOOST_CHECK_EQUAL(got, EXPECTED_[index_++]);
        }

    private:

        unsigned int index_;

    };


}  // anonymous namespace


BOOST_AUTO_TEST_CASE(test_sax_parser_with_handler)
{
    XTREE_LOG_TEST_NAME;
    const char* TEST_XML =
        "<root xmlns='http://example.com/xtree'>"
          "<x:sub1 xmlns:x='http://example.com/xtree/x1'>"
            "<x:sub2 xmlns:x='http://example.com/xtree/x2'>"
              "<x:sub3 a='A' x:b='B'>hello,world</x:sub3>"
            "</x:sub2>"
          "</x:sub1>"
        "</root>"
    ;
    try
    {
        xtree::sax_parser parser;
        dummy_content_handler handler;
        parser.set_content_handler(&handler);
        parser.parse_string(TEST_XML);
    }
    catch (const xtree::dom_error& ex)
    {
        BOOST_ERROR(ex.what());
    }
}


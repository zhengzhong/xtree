//
// Created by ZHENG Zhong on 2011-06-08.
//

#ifndef XTREE_DOM_PARSER_EXAMPLE_HPP_20110608__
#define XTREE_DOM_PARSER_EXAMPLE_HPP_20110608__

#include <xtree/xtree.hpp>

#include <iostream>
#include <memory>
#include <string>


void dom_parse_string_example()
{
    // dom_parse_string_example: xml-string-begin
    const char* EXAMPLE_XML_STRING =
        "<?xml version='1.0' encoding='UTF-8'?>"
        "<?processing-instructions dummy-pi?>"
        "<!--this is a comment-->"
        "<root>this is the content of this element</root>"
    ;
    // dom_parse_string_example: xml-string-end

    try
    {
        // dom_parse_string_example: parse-string-begin
        std::auto_ptr<xtree::document> doc(xtree::parse_string(EXAMPLE_XML_STRING));
        // dom_parse_string_example: parse-string-end

        // dom_parse_string_example: access-document-info-begin
        std::cout << "XML document "
                  << "(version " << doc->version() << ", encoding: " << doc->encoding() << ") "
                  << "contains " << doc->size() << " children.\n";
        // dom_parse_string_example: access-document-info-end

        // dom_parse_string_example: access-children-begin
        unsigned int index = 0;
        for (xtree::child_iterator i = doc->begin(); i != doc->end(); ++i, ++index)
        {
            std::cout << "content of child #" << index
                      << " (type ID: " << i->type() << ") : "
                      << i->content() << "\n";
        }
        // dom_parse_string_example: access-children-end

        // dom_parse_string_example: access-root-begin
        xtree::element_ptr root = doc->root();
        std::cout << "root element: " << root->name() << ", content: " << root->content() << "\n";
        // dom_parse_string_example: access-root-end
    }
    catch (const xtree::dom_error& ex)
    {
        std::cout << "DOM error: " << ex.what() << "\n";
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////


void dom_parse_bad_xml_example()
{
    // dom_parse_bad_xml_example: xml-string-begin
    const char* BAD_XML_STRINGS[] = {
        "<root><bad element/></root>",  // invalid element tag.
        "<root a='A1' b='B' a='A2'/>",  // attributes with the same name.
        "<root>hello",                  // unclosed tag.
        "<root>hello</Root>",           // unmatched begin/close tag.
    };
    // dom_parse_bad_xml_example: xml-string-end

    // dom_parse_bad_xml_example: parse-string-begin
    for (int i = 0; i < sizeof(BAD_XML_STRINGS) / sizeof(const char*); ++i)
    {
        try
        {
            std::auto_ptr<xtree::document> doc(xtree::parse_string(BAD_XML_STRINGS[i]));
            std::cerr << "error: expected xtree::dom_error but got nothing.\n";
        }
        catch (const xtree::dom_error& ex)
        {
            std::cout << "got expected xtree::dom_error: " << ex.what() << "\n";
        }
    }
    // dom_parse_bad_xml_example: parse-string-end
}


#endif  // XTREE_DOM_PARSER_EXAMPLE_HPP_20110608__


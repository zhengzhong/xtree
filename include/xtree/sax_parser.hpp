//
// Created by ZHENG Zhong on 2009-01-22.
//

#ifndef XTREE_SAX_PARSER_HPP_20090122__
#define XTREE_SAX_PARSER_HPP_20090122__

#include "xtree/config.hpp"
#include "xtree/xml_base.hpp"
#include "xtree/sax_attribute_list.hpp"
#include "xtree/sax_error_info.hpp"
#include "xtree/sax_handler.hpp"
#include "xtree/sax_xmlns_context.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <map>
#include <set>
#include <string>
#include <vector>


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // libxml2 SAX callbacks
    //


    //! Libxml2 SAX startDocumentSAXFunc.
    void libxml2_begin_document(void* user_data);

    //! Libxml2 SAX endDocumentSAXFunc.
    void libxml2_end_document(void* user_data);

    //! Libxml2 SAX startElementSAXFunc.
    void libxml2_begin_element(void* user_data, const xmlChar* qname, const xmlChar** attrs);

    //! Libxml2 SAX endElementSAXFunc.
    void libxml2_end_element(void* user_data, const xmlChar* qname);

    //! Libxml2 SAX charactersSAXFunc.
    void libxml2_characters(void* user_data, const xmlChar* chars, int length);

    //! Libxml2 SAX cdataBlockSAXFunc.
    void libxml2_cdata_block(void* user_data, const xmlChar* cdata, int length);

    //! Libxml2 SAX ignorableWhitespaceSAXFunc.
    void libxml2_ignorable_whitespace(void* user_data, const xmlChar* spaces, int length);

    //! Libxml2 SAX commentSAXFunc.
    void libxml2_comment(void* user_data, const xmlChar* comment);

    //! Libxml2 SAX xmlStructuredErrorFunc.
    //! See: http://www.xmlsoft.org/html/libxml-xmlerror.html#xmlError
    void libxml2_structured_error(void* user_data, xmlError* err);


    ////////////////////////////////////////////////////////////////////////////////////////////////


    //! This class represents an XML SAX parser. It is used to parse an XML file and invoke SAX
    //! callback functions. For more information about SAX XML parsing, check the SAX project at:
    //! http://www.saxproject.org/
    class XTREE_DECL sax_parser: private xml_base
    {

        ////////////////////////////////////////////////////////////////////////////////////////////
        // friend functions
        //

        friend void libxml2_begin_document(void*);
        friend void libxml2_end_document(void*);
        friend void libxml2_begin_element(void*, const xmlChar*, const xmlChar**);
        friend void libxml2_end_element(void*, const xmlChar*);
        friend void libxml2_characters(void*, const xmlChar*, int);
        friend void libxml2_cdata_block(void*, const xmlChar*, int);
        friend void libxml2_ignorable_whitespace(void*, const xmlChar*, int);
        friend void libxml2_comment(void*, const xmlChar*);
        friend void libxml2_structured_error(void*, xmlError*);

    public:

        //! Default constructor.
        explicit sax_parser();

        //! Destructor.
        ~sax_parser();

        void set_content_handler(sax_content_handler* handler);

        void set_error_handler(sax_error_handler* handler);

        //! Sets the value to a feature flag to enable or disable the feature.
        //! \param name    the name of the feature.
        //! \param enable  the value of the feature to set, true to enable, false to disable.
        void set_feature(const std::string& name, bool enable);

        //! Returns the value of a feature flag (true means enabled, false means disabled).
        //! \param name  the name of the feature.
        //! \return true if this feature is enabled, false otherwise.
        bool get_feature(const std::string& name) const;

        //! Parses an XML file and invokes SAX callbacks.
        //! \param file_name  the XML file name.
        void parse_file(const std::string& file_name);

        //! Parses a string containing the XML and invokes SAX callbacks.
        //! \param str  the XML string to parse.
        void parse_string(const char* str);

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // private callback member functions
        //

        //! Receives notification of the beginning of an XML document.
        void begin_document();

        //! Receives notification of the end of an XML document.
        void end_document();

        //! Receives notification of the beginning of an element.
        void begin_element(const xmlChar* qname, const xmlChar** attrs);

        //! Receives notification of the end of an element.
        void end_element(const xmlChar* qname);

        void characters(const xmlChar* chars, int length);

        void cdata_block(const xmlChar* chars, int length);

        void ignorable_whitespace(const xmlChar* chars, int length);

        //! Receives notification of an XML comment.
        //! \param chars  the XML comment.
        //! \param start  the starting position in the XML comment.
        //! \param length  the length of the XML comment.
        void comment(const xmlChar* chars);

        void structured_error(xmlError* err);

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // private utility member functions
        //

        std::string find_uri(const std::string& prefix) const;

        const std::vector<std::string>& get_deprecated_prefixes() const;

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // private struct and typedef's
        //

        //! This class represents an element in the element stack.
        struct element {

            explicit element(): qname_(), prefixes_() {
                // Do nothing.
            }

            // Use auto-generated copy constructor.
            // Use auto-generated destructor.
            // Use auto-generated copy assignment.

            std::string              qname_;
            std::vector<std::string> prefixes_;

        };  // struct element

        //! The element stack.
        typedef std::vector<element> element_stack;

        //! The namespace prefix mappings: key is the prefix, value is a stack of URIs.
        //typedef std::map<std::string, std::vector<std::string> > prefix_uris_map;

    private:

        //! Non-implemented copy constructor.
        sax_parser(const sax_parser&);

        //! Non-implemented copy assignment.
        sax_parser& operator=(const sax_parser&);

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // private members
        //

        std::set<std::string> features_;         //!< The feature and value map.
        int                   level_;            //!< The level of the current element.
        sax_xmlns_context     context_;          //!< XML namespace mappings.
        sax_content_handler*  content_handler_;  //!< Pointer to content handler.
        sax_error_handler*    error_handler_;    //!< Pointer to error handler.

    };


}  // namespace xtree


#endif  // XTREE_SAX_PARSER_HPP_20090122__


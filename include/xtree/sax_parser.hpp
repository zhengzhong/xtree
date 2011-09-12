//
// Created by ZHENG Zhong on 2009-01-22.
//

#ifndef XTREE_SAX_PARSER_HPP_20090122__
#define XTREE_SAX_PARSER_HPP_20090122__

#include "xtree/config.hpp"
#include "xtree/xml_base.hpp"
#include "xtree/sax_handler.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <set>
#include <string>


namespace xtree {


    //! \cond DEV

    namespace detail {

        //! Initializes a libxml2 SAX2 handler with all the callback functions registered.
        //! See: http://www.xmlsoft.org/html/libxml-tree.html#xmlSAXHandler
        //! \param handler  the libxml2 SAX2 handler to initialize.
        void initialize_libxml2_sax2_handler(xmlSAXHandler& handler);

    }  // namespace xtree::detail

    //! \endcond


    ////////////////////////////////////////////////////////////////////////////////////////////////


    //! This class represents an XML SAX parser. It is used to parse an XML file and invoke SAX
    //! callback functions. For more information about SAX XML parsing, check the SAX project at:
    //! http://www.saxproject.org/
    class XTREE_DECL sax_parser: private xml_base
    {

        //! \cond DEV

        friend void detail::initialize_libxml2_sax2_handler(xmlSAXHandler&);

        //! \endcond

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
        //! \param Libxml2 Callback Functions
        //! \{

        //! Receives notification of the beginning of an XML document.
        static void start_document(void* context);

        //! Receives notification of the end of an XML document.
        static void end_document(void* context);

        static void start_element_ns(void* context,
                                     const xmlChar* name,
                                     const xmlChar* prefix,
                                     const xmlChar* uri,
                                     int nb_xmlns_attrs,
                                     const xmlChar** xmlns_attrs,
                                     int nb_attrs,
                                     int nb_defaulted,
                                     const xmlChar** attrs);

        static void end_element_ns(void* context,
                                   const xmlChar* name,
                                   const xmlChar* prefix,
                                   const xmlChar* uri);

        static void characters(void* context, const xmlChar* chars, int length);

        static void cdata_block(void* context, const xmlChar* chars, int length);

        static void ignorable_whitespace(void* context, const xmlChar* chars, int length);

        //! Receives notification of an XML comment.
        //! \param chars  the XML comment.
        //! \param start  the starting position in the XML comment.
        //! \param length  the length of the XML comment.
        static void comment(void* context, const xmlChar* chars);

        static void structured_error(void* context, xmlError* err);

        //! \}

        //! Casts the context pointer to the SAX parser.
        //! \param context  pointer to the context pointer.
        //! \return the SAX parser.
        static sax_parser& get_sax_parser(void* context);

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Non-Copyable
        //! \{

        //! Non-implemented copy constructor.
        sax_parser(const sax_parser&);

        //! Non-implemented copy assignment.
        sax_parser& operator=(const sax_parser&);

        //! \}

    private:

        std::set<std::string> features_;         //!< SAX2 features.
        sax_content_handler*  content_handler_;  //!< Pointer to content handler.
        sax_error_handler*    error_handler_;    //!< Pointer to error handler.

    };


}  // namespace xtree


#endif  // XTREE_SAX_PARSER_HPP_20090122__


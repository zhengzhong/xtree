//
// Created by ZHENG Zhong on 2009-01-22.
//

#ifndef XTREE_SAX_HANDLER_HPP_20090122__
#define XTREE_SAX_HANDLER_HPP_20090122__

#include "xtree/config.hpp"
#include "xtree/sax_attribute_list.hpp"
#include "xtree/sax_error_info.hpp"
#include "xtree/unused_arg.hpp"

#include <string>


namespace xtree {


    //! This class defines the interface for handling XML content. It defines callback functions
    //! that will be invoked by the XML SAX parser. User should derive from this class to provide
    //! customized handling functions.
    class XTREE_DECL sax_content_handler
    {

    public:

        explicit sax_content_handler();

        virtual ~sax_content_handler() = 0;

        //! Receives notification of the beginning of an XML document. The SAX parser will invoke
        //! this function only once, before any other event callbacks.
        virtual void begin_document()
        {
            // Do nothing.
        }

        //! Receives notification of the end of an XML document. The SAX parser will invoke this
        //! function only once (in case no fatal error occurs), and it will be the last function
        //! invoked during the parse.
        virtual void end_document()
        {
            // Do nothing.
        }

        //! Receives notification of the beginning of a namespace scope. Note that begin_ns/end_ns
        //! events are not guaranteed to be properly nested relative to each other: all begin_ns
        //! events will occur immediately before the corresponding begin_elem event, and all end_ns
        //! events will occur immediately after the corresponding end_elem event, but their order
        //! is not otherwise guaranteed.
        //!
        //! There should never be begin_ns/end_ns events for the "xml" prefix, since it is
        //! predeclared and immutable.
        //!
        //! \param prefix  the namespace prefix being declared. An empty string is used for the
        //!                default element namespace, which has no prefix.
        //! \param uri     the namespace URI the prefix is mapped to.
        virtual void begin_xmlns(const std::string& prefix, const std::string& uri)
        {
            // Do nothing.
            detail::unused_arg(prefix);
            detail::unused_arg(uri);
        }

        //! Receives notification of the end of a namespace scope.
        //! \param prefix  the namespace prefix that was being mapped, or an empty string when a
        //!                default mapping scope ends.
        virtual void end_xmlns(const std::string& prefix)
        {
            // Do nothing.
            detail::unused_arg(prefix);
        }

        //! Receives notification of the beginning of an element.
        //! \param name   the local name of this element.
        //! \param uri    the namespace URI of this element.
        //! \param qname  the qualified (prefixed) name of this element.
        //! \param attrs  the attribute list associated with this element.
        virtual void begin_element(const std::string& name,
                                   const std::string& uri,
                                   const std::string& qname,
                                   const sax_attribute_list& attrs)
        {
            // Do nothing.
            detail::unused_arg(name);
            detail::unused_arg(uri);
            detail::unused_arg(qname);
            detail::unused_arg(attrs);
        }

        //! Receives notification of the end of an element.
        //! \param name   the local name of this element.
        //! \param uri    the namespace URI of this element.
        //! \param qname  the qualified (prefixed) name of this element.
        virtual void end_element(const std::string& name,
                                 const std::string& uri,
                                 const std::string& qname)
        {
            // Do nothing.
            detail::unused_arg(name);
            detail::unused_arg(uri);
            detail::unused_arg(qname);
        }

        virtual void characters(const char* chars, int length)
        {
            // Do nothing.
            detail::unused_arg(chars);
            detail::unused_arg(length);
        }

        virtual void cdata_block(const char* chars, int length)
        {
            // Do nothing.
            detail::unused_arg(chars);
            detail::unused_arg(length);
        }

        virtual void ignorable_whitespace(const char* chars, int length)
        {
            // Do nothing.
            detail::unused_arg(chars);
            detail::unused_arg(length);
        }

        virtual void comment(const char* chars)
        {
            // Do nothing.
            detail::unused_arg(chars);
        }

    };


    class XTREE_DECL sax_error_handler
    {

    public:

        explicit sax_error_handler();

        virtual ~sax_error_handler() = 0;

        virtual void warning(const sax_error_info& info)
        {
            // Do nothing.
            detail::unused_arg(info);
        }

        virtual void error(const sax_error_info& info)
        {
            // Do nothing.
            detail::unused_arg(info);
        }

        virtual void fatal(const sax_error_info& info)
        {
            // Do nothing.
            detail::unused_arg(info);
        }

    };


} // namespace xtree


#endif // XTREE_SAX_HANDLER_HPP_20090122__


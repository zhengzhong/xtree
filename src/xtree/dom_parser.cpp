//
// Created by ZHENG Zhong on 2008-01-17.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/dom_parser.hpp"
#include "xtree/document.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/globals.h>          // used by set_global_variables
#include <libxml/nanohttp.h>         // for xmlNanoHTTPScanProxy()
#include <libxml/parserInternals.h>  // for xmlCreateFileParserCtxt()
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <cassert>
#include <cstdlib>  // for std::getenv()
#include <cstring>
#include <memory>
#include <string>

#include <iostream> // DEBUG:

namespace xtree {


    namespace {


        //! This class manages the libxml2's global variables. On construction, it saves the
        //! original values and sets the values that we want. On destruction, it restores the
        //! original values.
        class set_global_variables
        {

        public:

            explicit set_global_variables(): indent_tree_output_(0), keep_blanks_default_(0)
            {
                indent_tree_output_  = xmlIndentTreeOutput;
                keep_blanks_default_ = xmlKeepBlanksDefault(0);
            }

            //! Destructor: restores the original value to libxml2.
            ~set_global_variables()
            {
                xmlIndentTreeOutput = indent_tree_output_;
                xmlKeepBlanksDefault(keep_blanks_default_);
            }

        private:

            //! Non-implemented copy constructor.
            set_global_variables(const set_global_variables&);

            //! Non-implemented copy assignment.
            set_global_variables& operator=(const set_global_variables&);

        private:

            int indent_tree_output_;   //!< Original value for whether to keep blank nodes.
            int keep_blanks_default_;  //!< Original value for indent tree output.

        };


        class dom_parser_context_wrapper
        {

        public:

            explicit dom_parser_context_wrapper(xmlParserCtxt* px): raw_(px)
            {
                // Do nothing.
            }

            ~dom_parser_context_wrapper()
            {
                if (raw_ != 0)
                {
                    raw_->_private = 0;
                    if (raw_->myDoc != 0)
                    {
                        xmlFreeDoc(raw_->myDoc);
                        raw_->myDoc = 0;
                    }
                    xmlFreeParserCtxt(raw_);
                    raw_ = 0;
                }
            }

            xmlParserCtxt* get()
            {
                return raw_;
            }

        private:

            //! Non-implemented copy constructor.
            dom_parser_context_wrapper(const dom_parser_context_wrapper&);

            //! Non-implemented copy assignment.
            dom_parser_context_wrapper& operator=(const dom_parser_context_wrapper&);

        private:

            xmlParserCtxt* raw_;

        };


        //! Deactivate any non-standards-compliant libxml1 features. These are deactivated by
        //! default, but if we don't deactivate them for each context then some other code which
        //! uses a global function, such as xmlKeepBlanksDefault(), could cause this to use the
        //! wrong settings.
        //! \param context  the libxml2 parser context to initialize.
        void init_dom_parser_context(xmlParserCtxt* context)
        {
            assert(context != 0 && "init_dom_parser_context() called with null parser context");
            if (context->sax != 0)
            {
                context->sax->warning = 0;  // Shut up for all warnings.
                context->sax->error   = 0;  // Shut up for all errors.
            }
            context->_private         = 0;  // Private data: not so necessary at this moment.
            context->linenumbers      = 1;  // Set line number (this is the default anyway).
            context->validate         = 0;  // Turn off validation.
            context->vctxt.error      = 0;  // Clear the validator's error callback.
            context->vctxt.warning    = 0;  // Clear the validator's warning callback.
            context->replaceEntities  = 1;  // Replace entities automatically.
        }


        //! Parse an xml document under the libxml2 parser context, and returns the _xmlDoc object.
        //! \param context  the libxml2 parser context under which the xml document is parsed.
        //! \return pointer to _xmlDoc object, never null.
        //! \throws dom_error if the XML is not well-formed, or if an error occurs.
        xmlDoc* parse_in_context(xmlParserCtxt* context)
        {
            assert(context != 0 && "parse_in_context() called with null parser context");
            // Overwrite global configuration value.
            set_global_variables global_variables;
            // Initialize libxml2 parser context.
            init_dom_parser_context(context);
            // Parse xml document under the parser context and check return code.
            int ret_code = xmlParseDocument(context);
            if (ret_code != 0 || context->errNo != 0)
            {
                std::string what = "fail to parse xml using libxml2: "
                                 + detail::build_error_message(context->lastError);
                throw dom_error(what);
            }
            if (context->myDoc == 0)
            {
                // We should not be here: if (errNo == 0), myDoc should be available.
                throw dom_error("fail to parse xml using libxml2: null document returned");
            }
            // Return the xml document parsed.
            xmlDoc* xdoc = context->myDoc;
            context->myDoc = 0;
            return xdoc;
        }


        //! This class manages the HTTP proxy of libxml2's NanoHTTP library. It initializes
        //! NanoHTTP at construction, and cleans up NanoHTTP at destruction.
        class set_nano_http_proxy
        {

        public:

            //! Constructs an object to set HTTP proxy and manage NanoHTTP's resource.
            //! \param proxy  the HTTP proxy. If empty, use environment variable "http_proxy".
            explicit set_nano_http_proxy(const std::string& proxy)
            {
                xmlNanoHTTPInit();
                xmlNanoHTTPScanProxy(proxy.empty() ? std::getenv("http_proxy") : proxy.c_str());
            }

            //! Destructs the object and cleans up the resource of NanoHTTP.
            ~set_nano_http_proxy()
            {
                xmlNanoHTTPCleanup();
            }

        private:

            //! Non-implemented copy constructor.
            set_nano_http_proxy(const set_nano_http_proxy&);

            //! Non-implemented copy assignment.
            set_nano_http_proxy& operator=(const set_nano_http_proxy&);

        };


    }  // anonymous namespace


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // dom_parser
    //


    dom_parser::dom_parser()
    {
        // Do nothing.
    }


    dom_parser::~dom_parser()
    {
        // Do nothing.
    }


    std::auto_ptr<document> dom_parser::parse_file(const std::string& file_name)
    {
        set_global_variables global_variables;
        // Create a libxml2 parser context for parsing the xml file.
        dom_parser_context_wrapper context( xmlCreateFileParserCtxt(file_name.c_str()) );
        if (context.get() == 0)
        {
            std::string what = "fail to parse xml file " + file_name + ": "
                             + "unable to create libxml2 parser context";
            throw dom_error(what);
        }
        if (context.get()->directory == 0)
        {
            const xmlChar* dir = detail::to_xml_chars(xmlParserGetDirectory(file_name.c_str()));
            context.get()->directory = const_cast<char*>(detail::to_chars(xmlStrdup(dir)));
        }
        // Parse xml file under the constructed parser context.
        xmlDoc* px = parse_in_context(context.get());
        assert(px != 0);
        return std::auto_ptr<document>(new document(px));
    }


    std::auto_ptr<document> dom_parser::parse_string(const char* str)
    {
        set_global_variables global_variables;
        // Create a libxml2 parser context for parsing the xml string.
        if (str == 0)
        {
            throw dom_error("fail to parse xml string: string is null");
        }
        int size = static_cast<int>(std::strlen(str));
        if (size == 0)
        {
            throw dom_error("fail to parse xml string: string is empty");
        }
        dom_parser_context_wrapper context( xmlCreateMemoryParserCtxt(str, size) );
        if (context.get() == 0)
        {
            throw dom_error("Fail to parse xml string: unable to create libxml2 parser context");
        }
        // Parse xml string under the constructed parser context.
        xmlDoc* px = parse_in_context(context.get());
        assert(px != 0);
        return std::auto_ptr<document>(new document(px));
    }


    std::auto_ptr<document> dom_parser::parse_url(const std::string& url, const std::string& proxy)
    {
        set_global_variables global_variables;
        // Create a libxml2 parser context for parsing the xml string.
        if (url.empty())
        {
            throw dom_error("fail to parse remote xml: URL is null");
        }
        // Set the HTTP proxy URL.
        set_nano_http_proxy set_proxy(proxy);
        // Create a libxml2 parser context for parsing the URL.
        dom_parser_context_wrapper context( xmlCreateURLParserCtxt(url.c_str(), 0) );
        if (context.get() == 0)
        {
            throw dom_error("Fail to parse remote xml: unable to create libxml2 parser context");
        }
        // Parse remote xml under the constructed parser context.
        xmlDoc* px = parse_in_context(context.get());
        assert(px != 0);
        return std::auto_ptr<document>(new document(px));
    }


} // namespace xtree


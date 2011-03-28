//
// Created by ZHENG Zhong on 2008-01-17.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/parser.hpp"
#include "xtree/document.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/globals.h>         // used by set_global_variables
#include <libxml/parserInternals.h> // for xmlCreateFileParserCtxt()
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <cassert>
#include <cstring>
#include <string>


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


        class parser_context_wrapper
        {

        public:

            explicit parser_context_wrapper(xmlParserCtxt* px): raw_(px)
            {
                // Do nothing.
            }

            ~parser_context_wrapper()
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
            parser_context_wrapper(const parser_context_wrapper&);

            //! Non-implemented copy assignment.
            parser_context_wrapper& operator=(const parser_context_wrapper&);

        private:

            xmlParserCtxt* raw_;

        };


        //! Deactivate any non-standards-compliant libxml1 features. These are deactivated by
        //! default, but if we don't deactivate them for each context then some other code which
        //! uses a global function, such as xmlKeepBlanksDefault(), could cause this to use the
        //! wrong settings.
        //! \param context  the libxml2 parser context to initialize.
        void init_parser_context(xmlParserCtxt* context)
        {
            assert(context != 0 && "init_parser_context() called with null parser context");
            if (context->sax != 0)
            {
                context->sax->warning = 0; // Shut up for all warnings.
                context->sax->error   = 0; // Shut up for all errors.
            }
            context->_private         = 0; // Private data: not so necessary at this moment.
            context->linenumbers      = 1; // Set line number (this is the default anyway).
            context->validate         = 0; // Turn off validation.
            context->vctxt.error      = 0; // Clear the validator's error callback.
            context->vctxt.warning    = 0; // Clear the validator's warning callback.
            context->replaceEntities  = 1; // Replace entities automatically.
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
            init_parser_context(context);

            // Parse xml document under the parser context.
            xmlParseDocument(context);

            // Check the return value.
            if (!context->wellFormed)
            {
                throw dom_error("fail to parse xml using libxml2: document is not well-formed");
            }
            if (context->errNo != 0)
            {
                std::string what = "fail to parse xml using libxml2: "
                                 + detail::build_error_message(context->errNo);
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


    }  // anonymous namespace


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // parser
    //


    parser::parser()
    {
        // Do nothing.
    }


    parser::~parser()
    {
        // Do nothing.
    }


    document* parser::parse_file(const std::string& file_name)
    {

        set_global_variables global_variables;

        // Create a libxml2 parser context for parsing the xml file.
        parser_context_wrapper context( xmlCreateFileParserCtxt(file_name.c_str()) );
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
        return new document(px);
    }


    document* parser::parse_string(const char* str)
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
        parser_context_wrapper context( xmlCreateMemoryParserCtxt(str, size) );
        if (context.get() == 0)
        {
            throw dom_error("Fail to parse xml string: unable to create libxml2 parser context");
        }

        // Parse xml string under the constructed parser context.
        xmlDoc* px = parse_in_context(context.get());
        assert(px != 0);
        return new document(px);
    }


} // namespace xtree


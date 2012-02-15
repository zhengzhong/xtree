//
// Created by ZHENG Zhong on 2009-07-15.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/schema.hpp"
#include "xtree/validity.hpp"
#include "xtree/document.hpp"
#include "xtree/exceptions.hpp"

#include <libxml/tree.h>
#include <libxml/xmlschemas.h>

#include <cassert>
#include <cstring>
#include <functional>
#include <string>


namespace xtree {


    namespace {


        class schema_valid_context
        {

        public:

            explicit schema_valid_context(xmlSchema* px): raw_(xmlSchemaNewValidCtxt(px))
            {
                // Do nothing.
            }

            ~schema_valid_context()
            {
                if (raw_ != 0)
                {
                    xmlSchemaFreeValidCtxt(raw_);
                    raw_ = 0;
                }
            }

            xmlSchemaValidCtxt* get()
            {
                return raw_;
            }

        private:

            schema_valid_context(const schema_valid_context&);

            schema_valid_context& operator=(const schema_valid_context&);

        private:

            xmlSchemaValidCtxt* raw_;  //!< The underlying libxml2 valid context.

        };


    } // anonymous namespace


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // public
    //


    schema::schema(): raw_(0)
    {
        // Do nothing.
    }


    schema::~schema()
    {
        free_();
    }


    bool schema::loaded() const
    {
        return (raw_ != 0);
    }


    void schema::load_from_file(const std::string& file_name)
    {
        free_();
        xmlSchemaParserCtxt* ctxt = xmlSchemaNewParserCtxt(file_name.c_str());
        if (ctxt != 0)
        {
            raw_ = xmlSchemaParse(ctxt);
            xmlSchemaFreeParserCtxt(ctxt);
        }
        if (raw_ == 0)
        {
            std::string what = "failed to load schema from file " + file_name;
            throw dom_error(what);
        }
    }


    void schema::load_from_string(const char* str)
    {
        free_();
        int size = static_cast<int>(std::strlen(str));
        xmlSchemaParserCtxt* ctxt = xmlSchemaNewMemParserCtxt(str, size);
        if (ctxt != 0)
        {
            raw_ = xmlSchemaParse(ctxt);
            xmlSchemaFreeParserCtxt(ctxt);
        }
        if (raw_ == 0)
        {
            throw dom_error("failed to load schema from string");
        }
    }


    validity schema::validate(document& doc) const
    {
        // Check if the schema has been loaded.
        if (raw_ == 0)
        {
            throw bad_dom_operation("failed to validate XML document: schema not loaded");
        }
        // Create a schema validation context from the schema.
        schema_valid_context valid_ctxt(raw_);
        if (valid_ctxt.get() == 0)
        {
            throw internal_dom_error("failed to create xmlSchemaValidCtxt from xmlSchema");
        }
        // Set validation callback functions, and validate the XML document.
        validity result;
        xmlSchemaSetValidErrors( valid_ctxt.get(),
                                 &detail::on_validity_error,
                                 &detail::on_validity_warning,
                                 static_cast<void*>(&result) );
        int ret_code = xmlSchemaValidateDoc(valid_ctxt.get(), doc.raw_doc());
        // Check the return code: 0 if the document is schemas valid, a positive error code number
        // otherwise, -1 in case of internal or API error.
        if (ret_code >= 0)
        {
            return result;
        }
        else
        {
            throw internal_dom_error("fail to validate: xmlSchemaValidateDoc returned -1");
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // private
    //


    void schema::free_()
    {
        if (raw_ != 0)
        {
            xmlSchemaFree(raw_);
            raw_ = 0;
        }
    }


} // namespace xtree


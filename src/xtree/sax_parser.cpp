#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/sax_parser.hpp"
#include "xtree/sax_attribute_list.hpp"
#include "xtree/sax_error_info.hpp"
#include "xtree/sax_features.hpp"
#include "xtree/sax_xmlns_context.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/libxml2_utility.hpp"

#include <cassert>
#include <cstring>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <libxml/parser.h>
#include <libxml/xmlerror.h>


namespace xtree {


    namespace {


        //! Creates a Libxml2 SAX handler with all the callback functions registered.
        //! See: http://www.xmlsoft.org/html/libxml-tree.html#xmlSAXHandler
        xmlSAXHandler create_libxml2_sax_handler_()
        {
            xmlSAXHandler handler;

            // Initialize the callback function pointers for content handler.
            handler.startDocument = &libxml2_begin_document;
            handler.endDocument = &libxml2_end_document;
            handler.startElementNs = 0;
            handler.endElementNs = 0;
            handler.startElement = &libxml2_begin_element;
            handler.endElement = &libxml2_end_element;
            handler.characters = &libxml2_characters;
            handler.cdataBlock = &libxml2_cdata_block;
            handler.ignorableWhitespace = &libxml2_ignorable_whitespace;
            handler.comment = &libxml2_comment;

            // Initialize the callback function pointers for error handler.
            handler.warning = 0;
            handler.error = 0;
            handler.fatalError = 0;
            handler.serror = &libxml2_structured_error;

            handler.internalSubset = 0;
            handler.isStandalone = 0;
            handler.hasInternalSubset = 0;
            handler.hasExternalSubset = 0;
            handler.resolveEntity = 0;
            handler.getEntity = 0;
            handler.entityDecl = 0;
            handler.notationDecl = 0;
            handler.attributeDecl = 0;
            handler.elementDecl = 0;
            handler.unparsedEntityDecl = 0;
            handler.setDocumentLocator = 0;
            handler.reference = 0;

            handler.processingInstruction = 0;
            handler.getParameterEntity = 0;

            handler.externalSubset = 0;
            handler._private = 0;
            handler.initialized = XML_SAX2_MAGIC; // Use Libxml2 SAX2!

            return handler;
        }


        //! Casts the user data pointer to a pointer to the SAX parser.
        //! \param user_data  pointer to user-defined data (should be a pointer to a SAX parser).
        //! \return pointer to the SAX parser.
        sax_parser* get_sax_parser_(void* user_data)
        {
            assert(user_data != 0 && "User-defined data should not be null");
            return static_cast<sax_parser*>(user_data);
        }


    }  // anonymous namespace


    ////////////////////////////////////////////////////////////////////////////////////////////////


    void libxml2_begin_document(void* user_data)
    {
        get_sax_parser_(user_data)->begin_document();
    }


    void libxml2_end_document(void* user_data)
    {
        get_sax_parser_(user_data)->end_document();
    }


    void libxml2_begin_element(void* user_data, const xmlChar* qname, const xmlChar** attrs)
    {
        get_sax_parser_(user_data)->begin_element(qname, attrs);
    }


    void libxml2_end_element(void* user_data, const xmlChar* qname)
    {
        get_sax_parser_(user_data)->end_element(qname);
    }


    void libxml2_characters(void* user_data, const xmlChar* chars, int length)
    {
        get_sax_parser_(user_data)->characters(chars, length);
    }


    void libxml2_cdata_block(void* user_data, const xmlChar* chars, int length)
    {
        get_sax_parser_(user_data)->characters(chars, length);
    }


    void libxml2_ignorable_whitespace(void* user_data, const xmlChar* chars, int length)
    {
        get_sax_parser_(user_data)->ignorable_whitespace(chars, length);
    }


    void libxml2_comment(void* user_data, const xmlChar* chars)
    {
        get_sax_parser_(user_data)->comment(chars);
    }


    void libxml2_structured_error(void* user_data, xmlError* err)
    {
        get_sax_parser_(user_data)->structured_error(err);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // sax_parser
    //


    sax_parser::sax_parser()
    : features_(), level_(0), context_(), content_handler_(0), error_handler_(0)
    {
        features_.insert(sax_namespaces);
    }


    sax_parser::~sax_parser()
    {
        // Do nothing.
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // parser :: public member functions
    //


    void sax_parser::set_content_handler(sax_content_handler* handler)
    {
        content_handler_ = handler;
    }


    void sax_parser::set_error_handler(sax_error_handler* handler)
    {
        error_handler_ = handler;
    }


    void sax_parser::set_feature(const std::string& name, bool enable)
    {
        if (enable)
        {
            features_.insert(name);
        }
        else
        {
            features_.erase(name);
        }
    }


    bool sax_parser::get_feature(const std::string& name) const
    {
        return (features_.find(name) != features_.end());
    }


    void sax_parser::parse_file(const std::string& file_name)
    {
        xmlSAXHandler handler = create_libxml2_sax_handler_();
        int ret = xmlSAXUserParseFile(&handler, this, file_name.c_str());
        if (ret != 0)
        {
            std::ostringstream oss;
            oss << "Fail to parse " << file_name << ": xmlSAXUserParseFile returned " << ret;
            throw sax_error(oss.str());
        }
    }

    void sax_parser::parse_string(const char* str)
    {
        xmlSAXHandler handler = create_libxml2_sax_handler_();
        int ret = xmlSAXUserParseMemory(&handler, this, str, std::strlen(str));
        if (ret != 0)
        {
            std::ostringstream oss;
            oss << "Fail to parse string: xmlSAXUserParseMemory returned " << ret;
            throw sax_error(oss.str());

        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // private callback member functions
    //


    void sax_parser::begin_document()
    {
        level_ = 0;
        context_.reset();
        if (content_handler_ != 0)
        {
            content_handler_->begin_document();
        }
    }


    void sax_parser::end_document()
    {
        if (content_handler_ != 0)
        {
            content_handler_->end_document();
        }
    }


    void sax_parser::begin_element(const xmlChar* qname, const xmlChar** attrs)
    {
        ++level_;
        // Convert attrs array into a SAX attribute list.
        sax_attribute_list sax_attrs;
        if (attrs != 0)
        {
            // attrs[2 * n]     : attribute QNames.
            // attrs[2 * n + 1] : attribute values.
            for (unsigned int i = 0; attrs[i] != 0; i = i + 2)
            {
                std::pair<std::string, std::string> prefix_name = detail::split_qname(attrs[i]);
                std::string value = ( attrs[i + 1] != 0
                                    ? detail::to_chars(attrs[i + 1])
                                    : std::string() );
                // Check if the attribute is a namespace declaration.
                if (prefix_name.first.empty() && prefix_name.second == "xmlns")
                {
                    // default namespace declaration: xmlns="..."
                    context_.push(level_, std::string(), value);
                }
                else if (prefix_name.first == "xmlns")
                {
                    // prefixed namespace declaration: xmlns:x="..."
                    context_.push(level_, prefix_name.second, value);
                }
                else
                {
                    // not a namespace declaration.
                    std::string uri = context_.find_uri(prefix_name.first);
                    sax_attribute sax_attr(prefix_name.second, uri, prefix_name.first, value);
                    sax_attrs.push_back(sax_attr);
                }
            }
        }
        // Call content handler's callback.
        std::pair<std::string, std::string> prefix_name = detail::split_qname(qname);
        std::string uri = context_.find_uri(prefix_name.first);
        if (content_handler_ != 0)
        {
            content_handler_->begin_element(
                prefix_name.second,
                uri,
                detail::to_chars(qname),
                sax_attrs
            );
        }
    }


    void sax_parser::end_element(const xmlChar* qname)
    {
        std::pair<std::string, std::string> prefix_name = detail::split_qname(qname);
        std::string uri = context_.find_uri(prefix_name.first);
        if (content_handler_ != 0)
        {
            content_handler_->end_element(prefix_name.second, uri, detail::to_chars(qname));
        }
        context_.pop(level_);
        --level_;
    }


    void sax_parser::characters(const xmlChar* chars, int length)
    {
        if (content_handler_ != 0)
        {
            content_handler_->characters(detail::to_chars(chars), length);
        }
    }


    void sax_parser::cdata_block(const xmlChar* chars, int length)
    {
        if (content_handler_ != 0)
        {
            content_handler_->cdata_block(detail::to_chars(chars), length);
        }
    }


    void sax_parser::ignorable_whitespace(const xmlChar* chars, int length)
    {
        if (content_handler_ != 0)
        {
            content_handler_->ignorable_whitespace(detail::to_chars(chars), length);
        }
    }


    void sax_parser::comment(const xmlChar* chars)
    {
        if (content_handler_ != 0)
        {
            content_handler_->comment(detail::to_chars(chars));
        }
    }


    void sax_parser::structured_error(xmlError* err)
    {
        sax_error_info info(err->message, err->file, err->line, 0);
        if (error_handler_ != 0)
        {
            switch (err->level)
            {
            case XML_ERR_WARNING:
                error_handler_->warning(info);
                break;
            case XML_ERR_ERROR:
                error_handler_->error(info);
                break;
            case XML_ERR_FATAL:
                error_handler_->fatal(info);
                break;
            default:
                // We should not arrive here. Consider this as a fatal error!
                error_handler_->fatal(info);
                break;
            }
        }
    }


} // namespace xtree


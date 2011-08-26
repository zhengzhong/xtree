#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/sax_parser.hpp"
#include "xtree/sax_attribute_list.hpp"
#include "xtree/sax_error_info.hpp"
#include "xtree/sax_features.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/parser.h>
#include <libxml/xmlerror.h>

#include <cassert>
#include <cstring>
#include <memory>
#include <sstream>
#include <string>


namespace xtree {


    namespace detail {

        void initialize_libxml2_sax2_handler(xmlSAXHandler& handler)
        {
            std::memset(&handler, 0, sizeof(xmlSAXHandler));
            handler.startDocument = &sax_parser::start_document;
            handler.endDocument = &sax_parser::end_document;
            handler.startElementNs = &sax_parser::start_element_ns;
            handler.endElementNs = &sax_parser::end_element_ns;
            handler.characters = &sax_parser::characters;
            handler.cdataBlock = &sax_parser::cdata_block;
            handler.ignorableWhitespace = &sax_parser::ignorable_whitespace;
            handler.comment = &sax_parser::comment;
            handler.serror = &sax_parser::structured_error;
            handler.initialized = XML_SAX2_MAGIC;  // Use Libxml2 SAX2!
        }

    }  // namespace xtree::detail


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // sax_parser :: constructor / destructor
    //


    sax_parser::sax_parser(): features_(), content_handler_(0), error_handler_(0)
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
        xmlSAXHandler handler;
        detail::initialize_libxml2_sax2_handler(handler);
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
        xmlSAXHandler handler;
        detail::initialize_libxml2_sax2_handler(handler);
        int ret = xmlSAXUserParseMemory(&handler, this, str, std::strlen(str));
        if (ret != 0)
        {
            std::ostringstream oss;
            oss << "Fail to parse string: xmlSAXUserParseMemory returned " << ret;
            throw sax_error(oss.str());

        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // private static libxml2 callback functions
    //


    void sax_parser::start_document(void* context)
    {
        sax_parser& p = get_sax_parser(context);
        if (p.content_handler_ != 0)
        {
            p.content_handler_->start_document();
        }
    }


    void sax_parser::end_document(void* context)
    {
        sax_parser& p = get_sax_parser(context);
        if (p.content_handler_ != 0)
        {
            p.content_handler_->end_document();
        }
    }


    void sax_parser::start_element_ns(void* context,
                                      const xmlChar* name,
                                      const xmlChar* prefix,
                                      const xmlChar* uri,
                                      int ,
                                      const xmlChar** ,
                                      int nb_attrs,
                                      int ,
                                      const xmlChar** attrs)
    {
        sax_parser& p = get_sax_parser(context);
        // Convert attrs array into a SAX attribute list.
        sax_attribute_list sax_attrs;
        for (int i = 0; i < nb_attrs; ++i)
        {
            sax_attrs.push_back(sax_attribute(i, attrs));
        }
        // Call content handler's callback.
        if (p.content_handler_ != 0)
        {
            p.content_handler_->start_element(
                (name != 0 ? detail::to_chars(name) : std::string()),
                (prefix != 0 ? detail::to_chars(prefix) : std::string()),
                (uri != 0 ? detail::to_chars(uri) : std::string()),
                sax_attrs
            );
        }
    }


    void sax_parser::end_element_ns(void* context,
                                    const xmlChar* name,
                                    const xmlChar* prefix,
                                    const xmlChar* uri)
    {
        sax_parser& p = get_sax_parser(context);
        if (p.content_handler_ != 0)
        {
            p.content_handler_->end_element(
                (name != 0 ? detail::to_chars(name) : std::string()),
                (prefix != 0 ? detail::to_chars(prefix) : std::string()),
                (uri != 0 ? detail::to_chars(uri) : std::string())
            );
        }
    }


    void sax_parser::characters(void* context, const xmlChar* chars, int length)
    {
        sax_parser& p = get_sax_parser(context);
        if (p.content_handler_ != 0)
        {
            p.content_handler_->characters(detail::to_chars(chars), length);
        }
    }


    void sax_parser::cdata_block(void* context, const xmlChar* chars, int length)
    {
        sax_parser& p = get_sax_parser(context);
        if (p.content_handler_ != 0)
        {
            p.content_handler_->cdata_block(detail::to_chars(chars), length);
        }
    }


    void sax_parser::ignorable_whitespace(void* context, const xmlChar* chars, int length)
    {
        sax_parser& p = get_sax_parser(context);
        if (p.content_handler_ != 0)
        {
            p.content_handler_->ignorable_whitespace(detail::to_chars(chars), length);
        }
    }


    void sax_parser::comment(void* context, const xmlChar* chars)
    {
        sax_parser& p = get_sax_parser(context);
        if (p.content_handler_ != 0)
        {
            p.content_handler_->comment(detail::to_chars(chars));
        }
    }


    void sax_parser::structured_error(void* context, xmlError* err)
    {
        sax_parser& p = get_sax_parser(context);
        sax_error_info info(err->message, err->file, err->line, 0);
        if (p.error_handler_ != 0)
        {
            switch (err->level)
            {
            case XML_ERR_WARNING:
                p.error_handler_->warning(info);
                break;
            case XML_ERR_ERROR:
                p.error_handler_->error(info);
                break;
            case XML_ERR_FATAL:
                p.error_handler_->fatal(info);
                break;
            default:
                // We should not arrive here. Consider this as a fatal error!
                p.error_handler_->fatal(info);
                break;
            }
        }
    }


    sax_parser& sax_parser::get_sax_parser(void* context)
    {
        assert(context != 0 && "SAX2 context pointer should not be null");
        return *(static_cast<sax_parser*>(context));
    }



} // namespace xtree


//
// Created by ZHENG Zhong on 2011-08-24.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/xmlns.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>
#include <cassert>
#include <string>


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // static functions
    //


    xmlns* xmlns::get_or_create(xmlNs* px)
    {
        if (px == 0)
        {
            return 0;
        }
        if (px->_private == 0)
        {
            px->_private = new xmlns(px);
        }
        return static_cast<xmlns*>(px->_private);
    }


    void xmlns::delete_private(xmlNs* px)
    {
        if (px != 0 && px->_private != 0)
        {
            xmlns* wrapper = static_cast<xmlns*>(px->_private);
            delete wrapper;
            px->_private = 0;
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // public functions
    //


    xmlns::xmlns(xmlNs* px): raw_(px)
    {
        assert(raw_ != 0 && "Underlying libxml2 namespace should not be null");
    }


    xmlns::~xmlns()
    {
        raw_ = 0;
    }


    std::string xmlns::uri() const
    {
        return (raw()->href != 0 ? detail::to_chars(raw()->href) : std::string() );
    }


    //void xmlns::set_uri(const std::string& uri);


    std::string xmlns::prefix() const
    {
        return (raw()->prefix != 0 ? detail::to_chars(raw()->prefix) : std::string() );
    }


    //void xmlns::set_prefix(const std::string& prefix);


    std::string xmlns::str() const
    {
        std::string tmp;
        str(tmp);
        return tmp;
    }


    void xmlns::str(std::string& str) const
    {
        // TODO: will this work?
        xmlBuffer* buffer = xmlBufferCreate();
        xmlNodeDump(buffer, raw()->context, const_cast<xmlNode*>(raw_as_node()), 0, 0);
        str = detail::to_chars(buffer->content);
        xmlBufferFree(buffer);
    }        


    document& xmlns::doc()
    {
        return const_cast<document&>( (static_cast<const xmlns&>(*this)).doc() );
    }


    const document& xmlns::doc() const
    {
        // TODO: will this work?
        assert(raw()->context != 0 && raw()->context->_private != 0);
        const document* doc = static_cast<const document*>(raw()->context->_private);
        return (*doc);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // private functions
    //


    const xmlns* xmlns::next_sibling_() const
    {
        return get_or_create(raw()->next);
    }


}  // namespace xtree






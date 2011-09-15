//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/text.hpp"

#include <libxml/tree.h>
#include <cassert>
#include <string>


namespace xtree {


    text::text(xmlNode* px): child_node(px)
    {
        assert(raw()->type == XML_TEXT_NODE || raw()->type == XML_CDATA_SECTION_NODE);
    }


    text::~text()
    {
        // Do nothing.
    }


    bool text::is_cdata() const
    {
        return (raw()->type == XML_CDATA_SECTION_NODE);
    }


    const text* text::cast_(const xmlNode* px)
    {
        if (px != 0 && (px->type == XML_TEXT_NODE || px->type == XML_CDATA_SECTION_NODE))
        {
            if (px->_private == 0)
            {
                throw internal_dom_error("fail to cast xmlNode to text: _private is null");
            }
            return static_cast<const text*>(px->_private);
        }
        else
        {
            return 0;
        }
    }


} // namespace xtree




//
// Created by ZHENG Zhong on 2009-07-27.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/xml_pi.hpp"

#include <libxml/tree.h>
#include <string>


namespace xtree {


    xml_pi::xml_pi(xmlNode* px): child_node(px)
    {
        assert(raw()->type == XML_PI_NODE);
    }


    xml_pi::~xml_pi()
    {
        // Do nothing.
    }


    const xml_pi* xml_pi::cast_(const xmlNode* px)
    {
        if (px != 0 && px->type == XML_PI_NODE)
        {
            if (px->_private == 0)
            {
                throw internal_dom_error("fail to cast xmlNode to xml_pi: _private is null");
            }
            return static_cast<const xml_pi*>(px->_private);
        }
        else
        {
            return 0;
        }
    }


    xml_pi* xml_pi::clone(bool recursive) const
    {
        xmlNode* px = clone_raw(recursive);
        return static_cast<xml_pi*>(px->_private);
    }


} // namespace xtree


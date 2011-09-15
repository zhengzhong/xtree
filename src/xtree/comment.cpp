//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/comment.hpp"

#include <libxml/tree.h>
#include <cassert>
#include <string>


namespace xtree {


    comment::comment(xmlNode* px): child_node(px)
    {
        assert(raw()->type == XML_COMMENT_NODE);
    }


    comment::~comment()
    {
        // Do nothing.
    }


    const comment* comment::cast_(const xmlNode* px)
    {
        if (px != 0 && px->type == XML_COMMENT_NODE)
        {
            if (px->_private == 0)
            {
                throw internal_dom_error("fail to cast xmlNode to comment: _private is null");
            }
            return static_cast<const comment*>(px->_private);
        }
        else
        {
            return 0;
        }
    }


} // namespace xtree




//
// Created by ZHENG Zhong on 2009-07-27.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/instruction.hpp"

#include <libxml/tree.h>
#include <string>


namespace xtree {


    instruction::instruction(xmlNode* px): child_node(px)
    {
        assert(raw()->type == XML_PI_NODE);
    }


    instruction::~instruction()
    {
        // Do nothing.
    }


    const instruction* instruction::cast_(const xmlNode* px)
    {
        if (px != 0 && px->type == XML_PI_NODE)
        {
            if (px->_private == 0)
            {
                throw internal_dom_error("fail to cast xmlNode to instruction: _private is null");
            }
            return static_cast<const instruction*>(px->_private);
        }
        else
        {
            return 0;
        }
    }


} // namespace xtree


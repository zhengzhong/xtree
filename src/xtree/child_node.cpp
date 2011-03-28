//
// Created by ZHENG Zhong on 2008-07-26.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/child_node.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>
#include <string>


namespace xtree {


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // constructor/destructor
    //


    child_node::child_node(xmlNode* px): node(px)
    {
        // Do nothing.
    }


    child_node::~child_node()
    {
        // Do nothing.
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // public
    //


    basic_node_ptr<child_node> child_node::prev_sibling()
    {
        return basic_node_ptr<child_node>( const_cast<child_node*>(prev_sibling_()) );
    }


    basic_node_ptr<child_node> child_node::next_sibling()
    {
        return basic_node_ptr<child_node>( const_cast<child_node*>(next_sibling_()) );
    }


    basic_node_ptr<const child_node> child_node::prev_sibling() const
    {
        return basic_node_ptr<const child_node>( prev_sibling_() );
    }


    basic_node_ptr<const child_node> child_node::next_sibling() const
    {
        return basic_node_ptr<const child_node>( next_sibling_() );
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // private static
    //


    const child_node* child_node::cast_(const xmlNode* px)
    {
        if ( px != 0 && (  px->type == XML_ELEMENT_NODE
                        || px->type == XML_TEXT_NODE
                        || px->type == XML_CDATA_SECTION_NODE
                        || px->type == XML_PI_NODE
                        || px->type == XML_COMMENT_NODE) )
        {
            if (px->_private == 0)
            {
                throw internal_dom_error("fail to cast xmlNode to child_node: _private is null");
            }
            return static_cast<const child_node*>(px->_private);
        }
        else
        {
            return 0;
        }
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // private
    //


    const child_node* child_node::prev_sibling_() const
    {
        if (raw()->prev != 0)
        {
            return static_cast<const child_node*>(raw()->prev->_private);
        }
        else
        {
            return 0;
        }
    }


    const child_node* child_node::next_sibling_() const
    {
        if (raw()->next != 0)
        {
            return static_cast<const child_node*>(raw()->next->_private);
        }
        else
        {
            return 0;
        }
    }


} // namespace xtree




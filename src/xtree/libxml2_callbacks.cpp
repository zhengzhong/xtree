//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/libxml2_callbacks.hpp"
#include "xtree/attribute.hpp"
#include "xtree/element.hpp"
#include "xtree/text.hpp"
#include "xtree/comment.hpp"
#include "xtree/instruction.hpp"
#include "xtree/xmlns.hpp"

#include <libxml/tree.h>
#include <cassert>


namespace xtree {
namespace detail {


    namespace {


        template<class T>
        void delete_private(xmlNode* px)
        {
            // Delete all the namespace declarations on this node (if it's an element).
            if (px->type == XML_ELEMENT_NODE)
            {
                for (xmlNs* ns = px->nsDef; ns != 0; ns = ns->next)
                {
                    xmlns::delete_private(ns);
                }
            }
            // Delete the wrapper object for this node.
            T* wrapper = static_cast<T*>(px->_private);
            // If libxml2 has been loaded before xtree is loaded, we may find xmlNode containing
            // a null private pointer when it is being destructed. So we do not assert the private
            // pointer to be non-null.
            if (wrapper != 0)
            {
                delete wrapper;
                px->_private = 0;
            }
        }


    }  // anonymous namespace


    ////////////////////////////////////////////////////////////////////////////////////////////////


    void on_node_construct(xmlNode* px)
    {
        switch (px->type)
        {
        case XML_DOCUMENT_NODE:
            // Do nothing: the document wrapper will be created by the parser.
            break;
        case XML_ATTRIBUTE_NODE:
            px->_private = new attribute(px);
            break;
        case XML_ELEMENT_NODE:
            px->_private = new element(px);
            break;
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
            px->_private = new text(px);
            break;
        case XML_COMMENT_NODE:
            px->_private = new comment(px);
            break;
        case XML_PI_NODE:
            px->_private = new instruction(px);
            break;
            //case XML_DTD_NODE:
            //case XML_ENTITY_REF_NODE:
        default:
            // TODO: Unsupported node types.
            px->_private = 0;
            break;
        }
    }


    void on_node_destruct(xmlNode* px)
    {
        switch (px->type)
        {
        case XML_DOCUMENT_NODE:
            // Private pointer should have been set to null.
            assert(px->_private == 0 && "Document node should have null _private.");
            break;
        case XML_ATTRIBUTE_NODE:
            delete_private<attribute>(px);
            break;
        case XML_ELEMENT_NODE:
            delete_private<element>(px);
            break;
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
            delete_private<text>(px);
            break;
        case XML_COMMENT_NODE:
            delete_private<comment>(px);
            break;
        case XML_PI_NODE:
            delete_private<instruction>(px);
            break;
        default:
            // TODO: Unsupported node types.
            break;
        }
    }


} // namespace xtree::detail
} // namespace xtree



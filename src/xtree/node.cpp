//
// Created by ZHENG Zhong on 2008-07-16.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/node.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/document.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>
#include <cassert>
#include <string>


namespace xtree {


    ///////////////////////////////////////////////////////////////////////////////////////////
    //! \name Static Functions
    //! \{


    void node::delete_(node* ptr)
    {
        if (ptr != 0)
        {
            xmlNode* px = ptr->raw();
            assert(px != 0);
            if (px->type == XML_DOCUMENT_NODE)
            {
                // Deletion of a document node is not allowed.
                throw bad_dom_operation("document should not be deleted by node::delete_()");
            }
            else
            {
                // Unlink and free the underlying libxml2 node. The wrapper object will be deleted
                // by the libxml2 callback function.
                xmlUnlinkNode(px);
                xmlFreeNode(px);
            }
        }
    }


    //! \}


    ///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Constructor and Destructor
    //! \{


    node::node(xmlNode* px): raw_(px)
    {
        assert(raw_ != 0 && "Underlying libxml2 node should not be null.");
    }


    node::~node()
    {
        raw_ = 0;
    }


    //! \}


    ///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Property Access
    //! \{


    node_t node::type() const
    {
        switch (raw()->type)
        {
        case XML_DOCUMENT_NODE:
            return document_node;
        case XML_ATTRIBUTE_NODE:
            return attribute_node;
        case XML_ELEMENT_NODE:
            return element_node;
        case XML_TEXT_NODE:
            return text_node;
        case XML_CDATA_SECTION_NODE:
            return cdata_node;
        case XML_COMMENT_NODE:
            return comment_node;
        case XML_PI_NODE:
            return instruction_node;
        default:
            assert(! "Unknown libxml2 node type");
            return unknown_node;
        }
    }


    std::string node::content() const
    {
        // Check whether this is a document node.
        if (raw()->type == XML_DOCUMENT_NODE)
        {
            return std::string();
        }
        // Quoted from libxml2's documentation:
        //   Read the value of a node, this can be either the text carried directly by this node if
        //   it's a TEXT node or the aggregate string of the values carried by this node child's
        //   (TEXT and ENTITY_REF). Entity references are substituted.
        // Note: the returned xmlChar* (maybe null) should be freed (using xmlFree) by the caller.
        std::string value;
        xmlChar* raw_value = xmlNodeGetContent( const_cast<xmlNode*>(raw()) );
        if (raw_value != 0)
        {
            value = detail::to_chars(raw_value);
            xmlFree(raw_value);
            raw_value = 0;
        }
        return value;
    }


    void node::set_content(const std::string& content)
    {
        // Check whether this is a document node.
        if (raw()->type == XML_DOCUMENT_NODE)
        {
            throw bad_dom_operation("fail to set content: document does not have content");
        }
        // Quoted from libxml2 documentation:
        //   (xmlNodeSetContent) Replace the content of a node. NOTE: @content is supposed to be a
        //   piece of XML CDATA, so it allows entity references, but XML special chars need to be
        //   escaped first by using xmlEncodeEntitiesReentrant() resp. xmlEncodeSpecialChars().
        // Note: if this node is an element, after this function is called, all its original child
        // nodes will be destroyed.
        xmlNodeSetContent(raw(), detail::to_xml_chars(content.c_str()));
    }


    std::string node::name() const
    {
        switch (raw()->type)
        {
        case XML_DOCUMENT_NODE:
            return "#document";
        case XML_ELEMENT_NODE:
        case XML_ATTRIBUTE_NODE:
        case XML_PI_NODE:
            return detail::to_chars(raw()->name);
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
            return "#text";
        case XML_COMMENT_NODE:
            return "#comment";
        default:
            assert(! "Unknown libxml2 node type");
            return "#unknown";
        }
    }


    std::string node::path() const
    {
        std::string path;
        xmlChar* raw_path = xmlGetNodePath( const_cast<xmlNode*>(raw()) );
        if (raw_path != 0)
        {
            path = detail::to_chars(raw_path);
            xmlFree(raw_path);
            raw_path = 0;
        }
        return path;
    }


    void node::str(std::string& str) const
    {
        xmlBuffer* buffer = xmlBufferCreate();
        xmlNodeDump(buffer, raw()->doc, const_cast<xmlNode*>(raw()), 0, 0);
        str = detail::to_chars(buffer->content);
        xmlBufferFree(buffer);
    }


    //! \}


    ///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Owner Access
    //! \{


    document& node::doc()
    {
        return const_cast<document&>( (static_cast<const node&>(*this)).doc() );
    }


    const document& node::doc() const
    {
        assert(raw()->doc != 0 && raw()->doc->_private != 0);
        const document* doc = static_cast<const document*>(raw()->doc->_private);
        return (*doc);
    }


    basic_node_ptr<element> node::parent()
    {
        return basic_node_ptr<element>( const_cast<element*>(parent_()) );
    }


    basic_node_ptr<const element> node::parent() const
    {
        return basic_node_ptr<const element>( parent_() );
    }


    //! \}


    ///////////////////////////////////////////////////////////////////////////////////////////
    //! \name Underlying Libxml2 Object Access
    //! \{


    xmlNode* node::clone_raw(bool recursive) const
    {
        // This function should not be called on a document node.
        if (raw()->type == XML_DOCUMENT_NODE)
        {
            throw internal_dom_error("fail to clone a document node");
        }
        // The third parameter (extended) of xmlDocCopyNode takes the value 1 or 2:
        // - 1 to do a recursive copy (attributes, namespaces and children when applicable);
        // - 2 to copy attributes and namespaces (when applicable).
        xmlNode* px = xmlDocCopyNode( const_cast<xmlNode*>(raw()),
                                      raw()->doc,
                                      (recursive ? 1 : 2) );
        // Check the cloned node.
        if (px == 0)
        {
            throw internal_dom_error("Fail to clone node: xmlDocCopyNode() returned null");
        }
        else if (px->_private == 0)
        {
            throw internal_dom_error("Fail to clone node: the cloned node has null _private");
        }
        // Return a pointer to the cloned node.
        return px;
    }


    //! \}


    ///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Private Functions
    //! \{


    const node* node::cast_(const xmlNode* px)
    {
        if (px != 0)
        {
            if (px->_private == 0)
            {
                throw internal_dom_error("fail to cast xmlNode to node: _private is null");
            }
            return static_cast<const node*>(px->_private);
        }
        else
        {
            return 0;
        }
    }


    const element* node::parent_() const
    {
        if (raw()->parent != 0 && raw()->parent->type == XML_ELEMENT_NODE)
        {
            return static_cast<const element*>(raw()->parent->_private);
        }
        else
        {
            return 0;
        }
    }


    const node* node::prev_sibling_() const
    {
        if (raw()->prev != 0)
        {
            return static_cast<const node*>(raw()->prev->_private);
        }
        else
        {
            return 0;
        }
    }


    const node* node::next_sibling_() const
    {
        if (raw()->next != 0)
        {
            return static_cast<const node*>(raw()->next->_private);
        }
        else
        {
            return 0;
        }
    }


    //! \}


}  // namespace xtree






//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/document.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/node.hpp"
#include "xtree/element.hpp"
#include "xtree/text.hpp"
#include "xtree/comment.hpp"
#include "xtree/child_node_list.hpp"

#include "xtree/xpath.hpp"
#include "xtree/xpath_context.hpp"
#include "xtree/xpath_result.hpp"
#include "xtree/node_set.hpp"

#include "xtree/check_rules.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>

#include <algorithm>
#include <cassert>
#include <map>
#include <sstream>
#include <string>
#include <vector>


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // constructor/destructor
    //


    document::document()
    : raw_( xmlNewDoc(detail::to_xml_chars("1.0")) ), children_(raw_), orphans_()
    {
        if (raw_ == 0)
        {
            throw internal_dom_error("fail to create libxml2 document: xmlNewDoc() returns null");
        }
        raw_->_private = this;
    }


    document::document(xmlDoc* px): raw_(px), children_(px), orphans_() {
        assert(raw_ != 0 && raw_->type == XML_DOCUMENT_NODE);
        raw_->_private = this;
    }


    document::~document()
    {
        // TODO: Free all orphan nodes.
        typedef std::vector<node*>::iterator iterator;
        for (iterator i = orphans_.begin(); i != orphans_.end(); ++i)
        {
            if (is_my_orphan_(*i))
            {
                node* orphan = *i;
                xmlUnlinkNode(orphan->raw());
                xmlFreeNode(orphan->raw());
            }
        }
        // Free the underlying libxml2 document.
        raw_->_private = 0;
        xmlFreeDoc(raw_);
        raw_ = 0;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // property access
    //


    std::string document::encoding() const
    {
        if (raw()->encoding != 0)
        {
            return detail::to_chars(raw()->encoding);
        }
        else
        {
            return std::string();
        }
    }


    std::string document::version() const
    {
        return "1.0";  // TODO: always 1.0?
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // root element access
    //


    basic_node_ptr<element> document::root()
    {
        return basic_node_ptr<element>( const_cast<element*>(root_()) );
    }


    basic_node_ptr<const element> document::root() const
    {
        return basic_node_ptr<const element>( root_() );
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // serialization
    //


    std::string document::str() const
    {
        std::string tmp;
        str(tmp);
        return tmp;
    }


    void document::str(std::string& str) const
    {
        // Dump the document to a memory buffer: it is up to the caller to free the buffer.
        xmlChar* buffer = 0;
        int size = 0;
        xmlDocDumpMemory( const_cast<xmlDoc*>(raw()), &buffer, &size );
        // Save the buffer's contents to string.
        if (size > 0 && buffer != 0)
        {
            str = detail::to_chars(buffer);
        }
        else
        {
            str = std::string();
        }
        // Free the buffer.
        xmlFree(buffer);
        buffer = 0;
    }


    void document::save_to_file(const std::string& file_name, const std::string& encoding) const
    {
        int size = xmlSaveFormatFileEnc(
            file_name.c_str(),
            const_cast<xmlDoc*>(raw()),
            (encoding.empty() ? 0 : encoding.c_str()),
            0
        );
        (size > 0);
    }


    document* document::clone() const
    {
        xmlDoc* px = xmlCopyDoc(const_cast<xmlDoc*>(raw()), 1);
        if (px == 0)
        {
            throw internal_dom_error("fail to clone libxml2 document: xmlCopyDoc() returns null");
        }
        return new document(px);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // root element
    //


    basic_node_ptr<element> document::reset_root(const std::string& name)
    {
        detail::check_name(name);
        // Create a new element without namespace.
        xmlNode* px = xmlNewDocNode( raw(), 0, detail::to_xml_chars(name.c_str()), 0 );
        if (px == 0)
        {
            std::string what = "fail to create libxml2 element: xmlNewDocNode() returns null";
            throw internal_dom_error(what);
        }
        // Reset the root element.
        basic_node_ptr<element> root = reset_root_(px);
        return root;
    }


    basic_node_ptr<element> document::reset_root(const std::string& qname, const std::string& uri)
    {
        detail::check_qname(qname);
        // Split QName into prefix and local name.
        std::pair<std::string, std::string> name_pair = detail::split_qname(qname);
        const std::string& prefix = name_pair.first;
        const std::string& name = name_pair.second;
        // Create a new element without namespace.
        xmlNode* px = xmlNewDocNode( raw(), 0, detail::to_xml_chars(name.c_str()), 0 );
        if (px == 0)
        {
            std::string what = "fail to create libxml2 element: xmlNewDocNode() returns null";
            throw internal_dom_error(what);
        }
        // Declare XML namespace on the element, and put the element under it.
        xmlNs* ns = xmlNewNs( px,
                              uri.empty() ? 0 : detail::to_xml_chars(uri.c_str()),
                              prefix.empty() ? 0 : detail::to_xml_chars(prefix.c_str()) );
        if (ns == 0)
        {
            // TODO: delete the node.
            std::string what = "fail to create libxml2 namespace for " + prefix + "=" + uri;
            throw internal_dom_error(what);
        }
        xmlSetNs(px, ns);
        // Reset the root element.
        basic_node_ptr<element> root = reset_root_(px);
        return root;
    }


    basic_node_ptr<element> document::reset_root_clone(const element& elem)
    {
        return reset_root_(elem.clone_raw(true));
    }


    basic_node_ptr<element> document::reset_root_adopt(element& elem)
    {
        return reset_root_(elem.raw());
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // children
    //


    void document::clear()
    {
        // TODO: Free all orphan nodes.
        for (std::vector<node*>::iterator i = orphans_.begin(); i != orphans_.end(); ++i) {
            if (is_my_orphan_(*i))
            {
                node* orphan = *i;
                xmlUnlinkNode(orphan->raw());
                xmlFreeNode(orphan->raw());
            }
        }
        // Free the DOM tree.
        children_.clear();
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // XPath
    //


    void document::select_nodes(const xpath& expr, node_set& nodes)
    {
        xpath_context context(raw(), 0);
        context.eval(expr, nodes);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // node creation
    //


    basic_node_ptr<element> document::make_element(const std::string& name) const
    {
        // Create a new libxml2 element node.
        xmlNode* px = xmlNewDocNode( const_cast<xmlDoc*>(raw()),
                                     0,
                                     detail::to_xml_chars(name.c_str()),
                                     0 );
        if (px == 0)
        {
            std::string what = "fail to create new element: xmlNewDocNode() returns null";
            throw internal_dom_error(what);
        }
        // Push the new libxml2 element to the orphan list and return.
        element* orphan = static_cast<element*>(px->_private);
        add_orphan_(orphan);
        return basic_node_ptr<element>(orphan);
    }


    basic_node_ptr<text> document::make_text(const std::string& value) const
    {
        // Create a new libxml2 text node.
        xmlNode* px = xmlNewDocText( const_cast<xmlDoc*>(raw()),
                                     detail::to_xml_chars(value.c_str()) );
        if (px == 0)
        {
            std::string what = "fail to create text node: xmlNewDocText() returns null";
            throw internal_dom_error(what);
        }
        // Push the new libxml2 text node to the orphan list and return.
        text* orphan = static_cast<text*>(px->_private);
        add_orphan_(orphan);
        return basic_node_ptr<text>(orphan);
    }


    basic_node_ptr<text> document::make_cdata(const std::string& value) const
    {
        xmlNode* px = xmlNewCDataBlock( const_cast<xmlDoc*>(raw()),
                                        detail::to_xml_chars(value.c_str()),
                                        static_cast<int>(value.size()) );
        if (px == 0)
        {
            std::string what = "fail to create CDATA node: xmlNewCDataBlock() returns null";
            throw internal_dom_error(what);
        }
        text* orphan = static_cast<text*>(px->_private);
        add_orphan_(orphan);
        return basic_node_ptr<text>(orphan);
    }


    basic_node_ptr<comment> document::make_comment(const std::string& value) const {
        xmlNode* px = xmlNewDocComment( const_cast<xmlDoc*>(raw()),
                                        detail::to_xml_chars(value.c_str()) );
        if (px == 0)
        {
            std::string what = "fail to create comment node: xmlNewDocComment() returns null";
            throw internal_dom_error(what);
        }
        comment* orphan = static_cast<comment*>(px->_private);
        add_orphan_(orphan);
        return basic_node_ptr<comment>(orphan);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // private
    //


    const element* document::root_() const
    {
        const xmlNode* px = xmlDocGetRootElement( const_cast<xmlDoc*>(raw()) );
        if (px != 0)
        {
            return static_cast<const element*>(px->_private);
        }
        else
        {
            return 0;
        }
    }


    basic_node_ptr<element> document::reset_root_(xmlNode* px)
    {
        assert(px != 0);
        xmlNode* px_old = xmlDocSetRootElement(raw(), px);
        if (px_old != 0)
        {
            xmlUnlinkNode(px_old);
            xmlFreeNode(px_old);
        }
        return basic_node_ptr<element>( static_cast<element*>(px->_private) );
    }


    bool document::is_my_orphan_(const node* orphan) const
    {
        return (
            orphan != 0                  // the node is not null, and...
            && orphan->is_orphan()       // is an orphan node, and...
            && &(orphan->doc()) == this  // belongs to this document.
        );
    }


    void document::add_orphan_(node* orphan) const
    {
        assert(is_my_orphan_(orphan));
        if (std::find(orphans_.begin(), orphans_.end(), orphan) == orphans_.end())
        {
            orphans_.push_back(orphan);
        }
    }


    void document::remove_orphan_(node*) const
    {
        // TODO: implement me!
    }


} // namespace xtree


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
#include "xtree/xpath_typed_results.hpp"
#include "xtree/node_set.hpp"

#include "xtree/check_rules.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Constructor and Destructor
    //! \{


    document::document(xmlDoc* px): node(reinterpret_cast<xmlNode*>(px)), children_(px)
    {
        px->_private = this;
    }


    document::~document()
    {
        xmlDoc* px = raw_doc();
        px->_private = 0;
        xmlFreeDoc(px);
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Property Access
    //! \{


    std::string document::encoding() const
    {
        if (raw_doc()->encoding != 0)
        {
            return detail::to_chars(raw_doc()->encoding);
        }
        else
        {
            return std::string();
        }
    }


    std::string document::version() const
    {
        return "1.0";  // always returns "1.0".
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Root Element Access
    //! \{


    basic_node_ptr<element> document::root()
    {
        return basic_node_ptr<element>( const_cast<element*>(root_()) );
    }


    basic_node_ptr<const element> document::root() const
    {
        return basic_node_ptr<const element>( root_() );
    }


    basic_node_ptr<element> document::reset_root(const std::string& name)
    {
        detail::check_local_part(name);
        // Create a new element without namespace.
        xmlNode* px = xmlNewDocNode( raw_doc(), 0, detail::to_xml_chars(name.c_str()), 0 );
        if (px == 0)
        {
            std::string what = "fail to create libxml2 element: xmlNewDocNode() returned null";
            throw internal_dom_error(what);
        }
        // Reset the root element.
        basic_node_ptr<element> root = reset_root_(px);
        return root;
    }


    basic_node_ptr<element> document::reset_root(const std::string& qname, const std::string& uri)
    {
        detail::check_qname(qname);
        detail::check_uri(uri);
        // Split QName into prefix and local name.
        std::pair<std::string, std::string> name_pair = detail::split_qname(qname);
        const std::string& prefix = name_pair.first;
        const std::string& name = name_pair.second;
        // Create a new element without namespace.
        xmlNode* px = xmlNewDocNode( raw_doc(), 0, detail::to_xml_chars(name.c_str()), 0 );
        if (px == 0)
        {
            std::string what = "fail to create libxml2 element: xmlNewDocNode() returned null";
            throw internal_dom_error(what);
        }
        // Declare XML namespace on the element, and put the element under it.
        xmlNs* ns = xmlNewNs( px,
                              detail::to_xml_chars(uri.c_str()),
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


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Serialization
    //! \{


    void document::save_to_file(const std::string& file_name, const std::string& encoding) const
    {
        int size = xmlSaveFormatFileEnc( file_name.c_str(),
                                         const_cast<xmlDoc*>(raw_doc()),
                                         ( encoding.empty() ? 0 : encoding.c_str() ),
                                         0 );
        (size > 0);
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name XPath
    //! \{


    void document::eval(const xpath& expr, xpath_result& result)
    {
        detail::xpath_context context(raw_doc(), 0);
        context.eval(expr, result);
    }


    bool document::eval_boolean(const xpath& expr) const
    {
        xpath_boolean result;
        detail::xpath_context context(const_cast<xmlDoc*>(raw_doc()), 0);
        context.eval(expr, result);
        return result.value();
    }


    double document::eval_number(const xpath& expr) const
    {
        xpath_number result;
        detail::xpath_context context(const_cast<xmlDoc*>(raw_doc()), 0);
        context.eval(expr, result);
        return result.value();
    }


    std::string document::eval_string(const xpath& expr) const
    {
        xpath_string result;
        detail::xpath_context context(const_cast<xmlDoc*>(raw_doc()), 0);
        context.eval(expr, result);
        return result.value();
    }


    void document::select_nodes(const xpath& expr, node_set& nodes)
    {
        eval(expr, nodes);
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Private Functions
    //! \{


    const element* document::root_() const
    {
        const xmlNode* px = xmlDocGetRootElement( const_cast<xmlDoc*>(raw_doc()) );
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
        assert(px != 0 && px->type == XML_ELEMENT_NODE);
        // Reset root element, and free the old one.
        xmlNode* px_old = xmlDocSetRootElement(raw_doc(), px);
        if (px_old != 0)
        {
            xmlUnlinkNode(px_old);
            xmlFreeNode(px_old);
            px_old = 0;
        }
        // Reconciliate XML namespaces on the new root element.
        int count = xmlReconciliateNs(px->doc, px);
        if (count < 0)
        {
            throw internal_dom_error("fail to reconciliate xmlns on the root element");
        }
        // Return the new root element.
        return basic_node_ptr<element>( static_cast<element*>(px->_private) );
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Document Creation Functions
    //! \{


    std::auto_ptr<document> create_document()
    {
        xmlDoc* px = xmlNewDoc(detail::to_xml_chars("1.0"));
        if (px == 0)
        {
            throw internal_dom_error("fail to create libxml2 document: xmlNewDoc returned null");
        }
        return std::auto_ptr<document>(new document(px));
    }


    std::auto_ptr<document> create_document(const std::string& name)
    {
        std::auto_ptr<document> doc = create_document();
        doc->reset_root(name);
        return doc;
    }


    std::auto_ptr<document> create_document(const std::string& qname,
                                            const std::string& uri)
    {
        std::auto_ptr<document> doc = create_document();
        doc->reset_root(qname, uri);
        return doc;
    }


    std::auto_ptr<document> clone_document(const document& doc)
    {
        xmlDoc* px = xmlCopyDoc(const_cast<xmlDoc*>(doc.raw_doc()), 1);
        if (px == 0)
        {
            throw internal_dom_error("fail to clone libxml2 document: xmlCopyDoc() returned null");
        }
        return std::auto_ptr<document>(new document(px));
    }


    //! \}


} // namespace xtree


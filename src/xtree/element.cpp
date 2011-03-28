//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/element.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/document.hpp"

#include "xtree/xpath_context.hpp"
#include "xtree/xpath_result.hpp"
#include "xtree/node_set.hpp"

#include "xtree/libxml2_utility.hpp"

#include <cassert>
#include <cstring>
#include <string>


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // constructor/destructor
    //


    element::element(xmlNode* px): child_node(px), children_(px), attrs_(px)
    {
        assert(raw()->type == XML_ELEMENT_NODE);
    }


    element::~element()
    {
        // Do nothing.
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // element information
    //


    std::string element::uri() const
    {
        if (raw()->ns != 0 && raw()->ns->href != 0)
        {
            return detail::to_chars(raw()->ns->href);
        }
        else
        {
            return std::string();
        }
    }


    std::string element::prefix() const
    {
        if (raw()->ns != 0 && raw()->ns->prefix != 0)
        {
            return detail::to_chars(raw()->ns->prefix);
        }
        else
        {
            return std::string();
        }
    }


    std::string element::qname() const
    {
        std::string prefix_str = prefix();
        if (prefix_str.empty())
        {
            return name();
        }
        else
        {
            return ( prefix_str + ":" + name() );
        }
    }


    std::string element::tag() const
    {
        std::string uri_str = uri();
        if (uri_str.empty())
        {
            return name();
        }
        else
        {
            return ( "{" + uri_str + "}" + name() );
        }
    }


    std::string element::head() const
    {
        std::string head_text;
        for (basic_node_ptr<const child_node> i = prev_sibling(); i != 0; i = i->prev_sibling())
        {
            if (i->type() == text_node || i->type() == cdata_node)
            {
                head_text.insert(0, i->content());
            }
            else
            {
                break;
            }
        }
        return head_text;
    }


    std::string element::tail() const
    {
        std::string tail_text;
        for (basic_node_ptr<const child_node> i = next_sibling(); i != 0; i = i->next_sibling())
        {
            if (i->type() == text_node || i->type() == cdata_node)
            {
                tail_text.append(i->content());
            }
            else
            {
                break;
            }
        }
        return tail_text;
    }


    void element::set_name(const std::string& name)
    {
        // TODO: check if the name is a valid name.
        xmlNodeSetName(raw(), detail::to_xml_chars(name.c_str()));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // namespaces
    //


    void element::set_uri(const std::string& uri)
    {
        xmlNs* ns = 0;
        if (!uri.empty())
        {
            ns = declare_namespace_(uri);
        }
        xmlSetNs(raw(), ns);
    }


    std::string element::declare_namespace(const std::string& uri)
    {
        xmlNs* px = declare_namespace_(uri);
        assert(px != 0);
        return ( px->prefix != 0 ? detail::to_chars(px->prefix) : std::string() );
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // children
    //


    child_node_list& element::children()
    {
        return children_;
    }


    const child_node_list& element::children() const
    {
        return children_;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // subelement access
    //


    basic_node_ptr<element> element::find_first_elem()
    {
        return basic_node_ptr<element>( const_cast<element*>(find_first_elem_()) );
    }


    basic_node_ptr<const element> element::find_first_elem() const
    {
        return basic_node_ptr<const element>( find_first_elem_() );
    }


    basic_node_ptr<element> element::find_last_elem()
    {
        return basic_node_ptr<element>( const_cast<element*>(find_last_elem_()) );
    }


    basic_node_ptr<const element> element::find_last_elem() const
    {
        return basic_node_ptr<const element>( find_last_elem_() );
    }


    basic_node_ptr<element> element::find_elem_by_name(const std::string& name)
    {
        return basic_node_ptr<element>( const_cast<element*>(find_elem_by_name_(name)) );
    }


    basic_node_ptr<const element> element::find_elem_by_name(const std::string& name) const
    {
        return basic_node_ptr<const element>( find_elem_by_name_(name) );
    }


    basic_node_ptr<element> element::find_elem(const std::string& name,
                                               const std::string& uri)
    {
        return basic_node_ptr<element>( const_cast<element*>(find_elem_(name, uri)) );
    }


    basic_node_ptr<const element> element::find_elem(const std::string& name,
                                                     const std::string& uri) const
    {
        return basic_node_ptr<const element>( find_elem_(name, uri) );
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // sibling element access
    //


    basic_node_ptr<element> element::prev_sibling_elem()
    {
        return basic_node_ptr<element>( const_cast<element*>(prev_sibling_elem_()) );
    }


    basic_node_ptr<element> element::next_sibling_elem()
    {
        return basic_node_ptr<element>( const_cast<element*>(next_sibling_elem_()) );
    }


    basic_node_ptr<const element> element::prev_sibling_elem() const
    {
        return basic_node_ptr<const element>( prev_sibling_elem_() );
    }


    basic_node_ptr<const element> element::next_sibling_elem() const
    {
        return basic_node_ptr<const element>( next_sibling_elem_() );
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // XPath
    //


    void element::select_nodes(const std::string& xpath, node_set& nodes)
    {
        select_nodes(xpath, std::map<std::string, std::string>(), nodes);
    }


    void element::select_nodes(const std::string& xpath,
                               const std::pair<std::string, std::string>& ns_mapping,
                               node_set& nodes)
    {
        std::map<std::string, std::string> ns_mappings;
        ns_mappings.insert(ns_mapping);
        select_nodes(xpath, ns_mappings, nodes);
    }


    void element::select_nodes(const std::string& xpath,
                               const std::map<std::string, std::string>& ns_mappings,
                               node_set& nodes)
    {
        xpath_context context(raw()->doc, raw());
        typedef std::map<std::string, std::string>::const_iterator const_iterator;
        for (const_iterator i = ns_mappings.begin(); i != ns_mappings.end(); ++i)
        {
            context.register_ns(i->first, i->second);
        }
        context.eval(xpath, nodes);
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // private static
    //


    const element* element::cast_(const xmlNode* px)
    {
        if (px != 0 && px->type == XML_ELEMENT_NODE)
        {
            if (px->_private == 0)
            {
                throw internal_dom_error("fail to cast xmlNode to element: _private is null");
            }
            return static_cast<const element*>(px->_private);
        }
        else
        {
            return 0;
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // private
    //


    element* element::clone(bool recursive) const
    {
        xmlNode* px = clone_raw(recursive);
        return static_cast<element*>(px->_private);
    }


    xmlNs* element::declare_namespace_(const std::string& uri)
    {
        // TODO: check the namespace URI is valid.
        if (uri.empty())
        {
            std::string what = "invalid namespace URI: " + uri;
            throw bad_dom_operation(what);
        }

        // Check if a namespace with the same URI has already been declared.
        // Quoted from libxml2 documentation:
        //   (xmlSearchNsByHref) Search a namespace aliasing a given URI. Recurse on the parents
        //   until it finds the defined namespace or return NULL otherwise.
        xmlNs* px = xmlSearchNsByHref( raw()->doc, raw(), detail::to_xml_chars(uri.c_str()) );

        // If the namespace is not found, get a unique namespace prefix for the specified URI from
        // the owner document, and create a new namespace declaration on this element.
        // Quoted from libxml2 documentation:
        //   (xmlNewNs) Creation of a new namespace. This function will refuse to create a
        //   namespace with a similar prefix than an existing one present on this node. We use
        //   href==NULL in the case of an element creation where the namespace was not defined.
        // Note: xmlNewNs changes xmlNode::nsDef but not xmlNode::ns, which means, the created
        // namespace is only declared on the element, but the element is not under the namespace.
        if (px == 0)
        {
            std::string prefix = doc().next_namespace_prefix();
            px = xmlNewNs( raw(),
                           detail::to_xml_chars(uri.c_str()),
                           detail::to_xml_chars(prefix.c_str()) );
            if (px == 0)
            {
                std::string what = "fail to declare namespace with URI " + uri
                                 + " and prefix " + prefix
                                 + ": xmlNewNs() returns null";
                throw internal_dom_error(what);
            }
        }

        // Return the libxml2 namespace.
        return px;
    }


    const element* element::find_first_elem_() const
    {
        const element* found = 0;
        for (const xmlNode* i = raw()->children; found == 0 && i != 0; i = i->next)
        {
            if (i->type == XML_ELEMENT_NODE)
            {
                found = static_cast<const element*>(i->_private);
            }
        }
        return found;
    }


    const element* element::find_last_elem_() const
    {
        const element* current = find_first_elem_();
        for (const element* i = current; i != 0; i = i->next_sibling_elem_())
        {
            current = i;
        }
        return current;
    }


    const element* element::find_elem_by_name_(const std::string& name) const
    {
        const element* found = 0;
        for (const xmlNode* i = raw()->children; found == 0 && i != 0; i = i->next)
        {
            if (i->type == XML_ELEMENT_NODE)
            {
                const element* child = static_cast<const element*>(i->_private);
                if (child->name() == name)
                {
                    found = child;
                }
            }
        }
        return found;
    }


    const element* element::find_elem_(const std::string& name, const std::string& uri) const
    {
        const element* found = 0;
        for (const xmlNode* i = raw()->children; found == 0 && i != 0; i = i->next)
        {
            if (i->type == XML_ELEMENT_NODE)
            {
                const element* child = static_cast<const element*>(i->_private);
                if (child->name() == name && child->uri() == uri)
                {
                    found = child;
                }
            }
        }
        return found;
    }


    const element* element::prev_sibling_elem_() const
    {
        xmlNode* prev = raw()->prev;
        while (prev != 0 && prev->type != XML_ELEMENT_NODE)
        {
            prev = prev->prev;
        }
        if (prev != 0)
        {
            return static_cast<const element*>(prev->_private);
        }
        else
        {
            return 0;
        }
    }


    const element* element::next_sibling_elem_() const
    {
        xmlNode* next = raw()->next;
        while (next != 0 && next->type != XML_ELEMENT_NODE)
        {
            next = next->next;
        }
        if (next != 0)
        {
            return static_cast<const element*>(next->_private);
        }
        else
        {
            return 0;
        }
    }


} // namespace xtree


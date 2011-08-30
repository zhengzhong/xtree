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

#include "xtree/check_rules.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>

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
        detail::check_name(name);
        xmlNodeSetName(raw(), detail::to_xml_chars(name.c_str()));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // namespaces
    //


    void element::set_xmlns(const std::string& prefix, const std::string& uri)
    {
        xmlns* px = declare_xmlns_(prefix, uri);
        xmlSetNs(raw(), px->raw());
    }


    size_type element::reconciliate_xmlns()
    {
        int count = xmlReconciliateNs(raw()->doc, raw());
        if (count < 0)
        {
            throw internal_dom_error("fail to reconciliate xmlns: xmlReconciliateNs returned -1");
        }
        return static_cast<size_type>(count);
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


    xmlns* element::declare_xmlns_(const std::string& prefix, const std::string& uri)
    {
        // TODO: check the namespace URI is valid.
        if (uri.empty())
        {
            std::string what = "invalid namespace URI: " + uri;
            throw bad_dom_operation(what);
        }
        // Create (declare) a new libxml2 xmlNs on the element.
        xmlNs* px = xmlNewNs( raw(),
                              detail::to_xml_chars(uri.c_str()),
                              detail::to_xml_chars(prefix.c_str()) );
        if (px == 0)
        {
            std::string what = "fail to declare namespace " + prefix + "=" + uri
                             + ": xmlNewNs() returns null";
            throw internal_dom_error(what);
        }
        // Return the libxml2 xmlNs.
        return xmlns::get_or_create(px);
    }


    const xmlns* element::get_first_xmlns_() const
    {
        return xmlns::get_or_create(raw()->nsDef);
    }


    const xmlns* element::find_xmlns_by_prefix_(const std::string& prefix) const
    {
        xmlNs* px = xmlSearchNs( raw()->doc,
                                 const_cast<xmlNode*>(raw()),
                                 prefix.empty() ? 0 : detail::to_xml_chars(prefix.c_str()) );
        return xmlns::get_or_create(px);
    }


    const xmlns* element::find_xmlns_by_uri_(const std::string& uri) const
    {
        xmlNs* px = xmlSearchNsByHref( raw()->doc,
                                       const_cast<xmlNode*>(raw()),
                                       uri.empty() ? 0 : detail::to_xml_chars(uri.c_str()) );
        return xmlns::get_or_create(px);
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
        const element* found = 0;
        for (const xmlNode* i = raw()->children; i != 0; i = i->next)
        {
            if (i->type == XML_ELEMENT_NODE)
            {
                found = static_cast<const element*>(i->_private);
            }
        }
        return found;
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


    const element* element::find_elem_by_path_(const std::string& path) const
    {
        // If the path does not contain slash, call find_elem_by_name_().
        std::string::size_type pos = path.find_first_of('/');
        if (pos == std::string::npos)
        {
            return find_elem_by_name_(path);
        }
        // Otherwise, make recursive call to elem_by_path_().
        std::string name = path.substr(0, pos);
        std::string rest = path.substr(pos + 1);
        const element* found = 0;
        for (const xmlNode* i = raw()->children; found == 0 && i != 0; i = i->next)
        {
            if (i->type == XML_ELEMENT_NODE)
            {
                const element* child = static_cast<const element*>(i->_private);
                if (child->name() == name)
                {
                    found = child->find_elem_by_path_(rest);
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


    const element* element::get_prev_sibling_elem_() const
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


    const element* element::get_next_sibling_elem_() const
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


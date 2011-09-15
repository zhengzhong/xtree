//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/attribute.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/check_rules.hpp"
#include "xtree/basic_xmlns_ptr.hpp"
#include "xtree/xmlns.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>
#include <cassert>
#include <string>


namespace xtree {


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // constructor/destructor
    //


    attribute::attribute(xmlNode* px): node(px)
    {
        assert(raw()->type == XML_ATTRIBUTE_NODE);
    }


    attribute::~attribute()
    {
        // Do nothing.
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // property access
    //


    std::string attribute::uri() const
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


    std::string attribute::prefix() const
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


    std::string attribute::qname() const
    {
        std::string pre = prefix();
        if (pre.empty())
        {
            return name();
        }
        else
        {
            return (pre + ":" + name());
        }
    }


    void attribute::set_name(const std::string& name)
    {
        detail::check_local_part(name);
        xmlNodeSetName(raw(), detail::to_xml_chars(name.c_str()));
    }


    std::string attribute::value() const
    {
        return content();
    }


    void attribute::set_value(const std::string& value)
    {
        set_content(value);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // namespace access
    //


    basic_xmlns_ptr<xmlns> attribute::get_xmlns()
    {
        return basic_xmlns_ptr<xmlns>(xmlns::get_or_create(raw()->ns));
    }


    basic_xmlns_ptr<const xmlns> attribute::get_xmlns() const
    {
        return basic_xmlns_ptr<const xmlns>(xmlns::get_or_create(raw()->ns));
    }


    void attribute::set_xmlns(basic_xmlns_ptr<const xmlns> ns)
    {
        xmlSetNs(raw(), (ns != 0 ? const_cast<xmlNs*>(ns->raw()) : 0));
        // Reconciliate xmlns on the element holding this attribute.
        int count = xmlReconciliateNs(raw()->parent->doc, raw()->parent);
        if (count < 0)
        {
            throw internal_dom_error("fail to reconciliate xmlns on parent element");
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // sibling access
    //


    basic_node_ptr<attribute> attribute::prev_sibling()
    {
        return basic_node_ptr<attribute>( const_cast<attribute*>(prev_sibling_()) );
    }


    basic_node_ptr<attribute> attribute::next_sibling()
    {
        return basic_node_ptr<attribute>( const_cast<attribute*>(next_sibling_()) );
    }


    basic_node_ptr<const attribute> attribute::prev_sibling() const
    {
        return basic_node_ptr<const attribute>( prev_sibling_() );
    }


    basic_node_ptr<const attribute> attribute::next_sibling() const
    {
        return basic_node_ptr<const attribute>( next_sibling_() );
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // private static
    //


    const attribute* attribute::cast_(const xmlNode* px)
    {
        if (px != 0 && px->type == XML_ATTRIBUTE_NODE)
        {
            if (px->_private == 0)
            {
                throw internal_dom_error("fail to cast xmlNode to attribute: _private is null");
            }
            return static_cast<const attribute*>(px->_private);
        }
        else
        {
            return 0;
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Private Functions
    //! \{


    const attribute* attribute::prev_sibling_() const
    {
        if (raw()->prev != 0)
        {
            return static_cast<const attribute*>(raw()->prev->_private);
        }
        else
        {
            return 0;
        }
    }


    const attribute* attribute::next_sibling_() const
    {
        if (raw()->next != 0)
        {
            return static_cast<const attribute*>(raw()->next->_private);
        }
        else
        {
            return 0;
        }
    }


    //! \}


}  // namespace xtree





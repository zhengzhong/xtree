//
// Created by ZHENG Zhong on 2008-07-26.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/attribute_map.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/element.hpp"
#include "xtree/basic_xmlns_ptr.hpp"
#include "xtree/xmlns.hpp"
#include "xtree/check_rules.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>
#include <utility>
#include <string>


namespace xtree {


    attribute_map::attribute_map(xmlNode* px): raw_(px)
    {
        assert(raw()->type == XML_ELEMENT_NODE);
    }


    attribute_map::~attribute_map()
    {
        raw_ = 0;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // capacity
    //


    bool attribute_map::empty() const
    {
        return (first_attr_() == 0);
    }


    size_type attribute_map::size() const
    {
        size_type count = 0;
        for (const_iterator i = begin(); i != end(); ++i)
        {
            ++count;
        }
        return count;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // iterators
    //


    attribute_map::iterator attribute_map::begin()
    {
        return iterator( const_cast<attribute*>(first_attr_()) );
    }


    attribute_map::iterator attribute_map::end()
    {
        return iterator();
    }


    attribute_map::const_iterator attribute_map::begin() const
    {
        return const_iterator( first_attr_() );
    }


    attribute_map::const_iterator attribute_map::end() const
    {
        return const_iterator();
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // attribute access
    //


    void attribute_map::set(const std::string& qname, const std::string& value)
    {
        detail::check_qname(qname);
        attribute* existing = const_cast<attribute*>(find_attr_(qname));
        if (existing == 0)
        {
            attribute* new_attribute = create_attr_(qname, value);
            assert(new_attribute != 0);
        }
        else
        {
            existing->set_value(value);
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // modifiers/insert
    //


    std::pair<basic_node_ptr<attribute>, bool> attribute_map::insert(const std::string& qname,
                                                                     const std::string& value)
    {
        detail::check_qname(qname);
        attribute* existing = const_cast<attribute*>(find_attr_(qname));
        if (existing == 0)
        {
            attribute* new_attribute = create_attr_(qname, value);
            assert(new_attribute != 0);
            return std::make_pair(basic_node_ptr<attribute>(new_attribute), true);
        }
        else
        {
            return std::make_pair(basic_node_ptr<attribute>(existing), false);
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // modifiers/update
    //


    basic_node_ptr<attribute> attribute_map::update(const std::string& qname,
                                                    const std::string& value)
    {
        detail::check_qname(qname);
        std::pair<basic_node_ptr<attribute>, bool> inserted = insert(qname, value);
        if (!inserted.second)
        {
            inserted.first->set_value(value);
        }
        return inserted.first;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // modifiers/erase
    //


    attribute_map::iterator attribute_map::erase(iterator pos)
    {
        // Check the iterator to erase is not the end and belongs to this map.
        check_ownership_(pos);
        if (pos == end())
        {
            throw bad_dom_operation("the iterator to erase should not point to the end.");
        }
        // Delete the attribute pointed by the iterator.
        basic_node_ptr<attribute> ptr = pos.ptr();
        ++pos;
        ptr.delete_();
        return pos;
    }


    attribute_map::iterator attribute_map::erase(iterator first, iterator last)
    {
        // Check iterator parameters.
        check_ownership_(first);
        check_ownership_(last);
        // Erase all the attributes between the two iterators.
        for (iterator i = first; i != last; )
        {
            i = erase(i);
        }
        return last;
    }


    size_type attribute_map::erase(const std::string& qname)
    {
        iterator pos = find(qname);
        if (pos != end())
        {
            erase(pos);
            return 1;
        }
        else
        {
            return 0;
        }
    }


    size_type attribute_map::erase(const std::string& name, const std::string& uri)
    {
        iterator pos = find(name, uri);
        if (pos != end())
        {
            erase(pos);
            return 1;
        }
        else
        {
            return 0;
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // modifiers/clear
    //


    void attribute_map::clear()
    {
        erase(begin(), end());
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // private
    //


    element& attribute_map::owner()
    {
        return const_cast<element&>( (static_cast<const attribute_map&>(*this)).owner() );
    }


    const element& attribute_map::owner() const
    {
        const element* owner = static_cast<const element*>(raw()->_private);
        assert(owner != 0 && "Owner element should not be null.");
        return *owner;
    }


    const attribute* attribute_map::first_attr_() const
    {
        if (raw()->properties != 0)
        {
            return static_cast<const attribute*>(raw()->properties->_private);
        }
        else
        {
            return 0;
        }
    }


    void attribute_map::check_ownership_(iterator pos)
    {
        if ( pos != end() && (pos->parent() == 0 || pos->parent()->raw() != owner().raw()) )
        {
            throw bad_dom_operation("The iterator does not belong to this attribute map.");
        }
    }


    const attribute* attribute_map::find_attr_(const std::string& qname) const
    {
        std::pair<std::string, std::string> name_pair = detail::split_qname(qname);
        const attribute* found = 0;
        for (const xmlAttr* i = raw()->properties; found == 0 && i != 0; i = i->next)
        {
            const attribute* attr = static_cast<const attribute*>(i->_private);
            if (attr->prefix() == name_pair.first && attr->name() == name_pair.second)
            {
                found = attr;
            }
        }
        return found;
    }


    const attribute* attribute_map::find_attr_(const std::string& name,
                                               const std::string& uri) const
    {
        const attribute* found = 0;
        for (const xmlAttr* i = raw()->properties; found == 0 && i != 0; i = i->next)
        {
            const attribute* attr = static_cast<const attribute*>(i->_private);
            if (attr->name() == name && attr->uri() == uri)
            {
                found = attr;
            }
        }
        return found;
    }


    attribute* attribute_map::create_attr_(const std::string& qname, const std::string& value)
    {
        // Find xmlns for the attribute prefix (if exists).
        std::pair<std::string, std::string> name_pair = detail::split_qname(qname);
        basic_xmlns_ptr<xmlns> ns = owner().find_xmlns_by_prefix(name_pair.first);
        // TODO: if the prefix is empty, we use null xmlns or the default one (if exists)?
        if (!name_pair.first.empty() && ns == 0)
        {
            std::string what = "fail to create attribute " + qname
                             + ": xmlns for prefix " + name_pair.first + " not found";
            throw bad_dom_operation(what);
        }
        // Create the libxml2 attribute for the owner element.
        xmlAttr* px = xmlNewNsProp( raw(),
                                    (ns != 0 ? ns->raw() : 0),
                                    detail::to_xml_chars(name_pair.second.c_str()),
                                    detail::to_xml_chars(value.c_str()) );
        if (px == 0)
        {
            std::string what = "fail to create attribute " + qname
                             + ": xmlNewNsProp returned null";
            throw internal_dom_error(what);
        }
        // Return the new attribute.
        return static_cast<attribute*>(px->_private);
    }


} // namespace xtree


//
// Created by ZHENG Zhong on 2008-07-26.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/child_node_list.hpp"
#include "xtree/exceptions.hpp"

#include "xtree/document.hpp"
#include "xtree/element.hpp"
#include "xtree/text.hpp"
#include "xtree/comment.hpp"
#include "xtree/instruction.hpp"
#include "xtree/xmlns.hpp"

#include "xtree/check_rules.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>
#include <string>


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Constructor and Destructor
    //! \{


    child_node_list::child_node_list(xmlDoc* px): raw_(reinterpret_cast<xmlNode*>(px))
    {
        assert(raw_ != 0 && raw_->type == XML_DOCUMENT_NODE);
    }


    child_node_list::child_node_list(xmlNode* px): raw_(px)
    {
        assert(raw_ != 0 && raw_->type == XML_ELEMENT_NODE);
    }


    child_node_list::~child_node_list()
    {
        raw_ = 0;
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Capacity
    //! \{


    bool child_node_list::empty() const
    {
        return (first_() == 0);
    }


    size_type child_node_list::size() const
    {
        size_type count = 0;
        for (const_iterator i = begin(); i != end(); ++i)
        {
            ++count;
        }
        return count;
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Iterators
    //! \{


    child_node_list::iterator child_node_list::begin()
    {
        return iterator( const_cast<child_node*>(first_()) );
    }


    child_node_list::iterator child_node_list::end()
    {
        return iterator();
    }


    child_node_list::const_iterator child_node_list::begin() const
    {
        return const_iterator( first_() );
    }


    child_node_list::const_iterator child_node_list::end() const
    {
        return const_iterator();
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Front and Back Access
    //! \{


    child_node& child_node_list::front()
    {
        return const_cast<child_node&>( (static_cast<const child_node_list&>(*this)).front() );
    }


    const child_node& child_node_list::front() const
    {
        if (empty())
        {
            throw bad_dom_operation("front() called on an empty child_node_list");
        }
        else
        {
            return *first_();
        }
    }


    child_node& child_node_list::back()
    {
        return const_cast<child_node&>( (static_cast<const child_node_list&>(*this)).back() );
    }


    const child_node& child_node_list::back() const
    {
        if (empty())
        {
            throw bad_dom_operation("back() called on an empty child_node_list");
        }
        else
        {
            basic_node_ptr<const child_node> last(first_());
            while (last->next_sibling() != 0)
            {
                last = last->next_sibling();
            }
            return *last;
        }
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Modifiers: push_back
    //! \{


    basic_node_ptr<element> child_node_list::push_back_element(const std::string& qname)
    {
        detail::check_qname(qname);
        xmlNode* px = insert_(end(), create_element_(qname));
        return basic_node_ptr<element>( static_cast<element*>(px->_private) );
    }


    basic_node_ptr<element> child_node_list::push_back_element(const std::string& qname,
                                                               const std::string& uri)
    {
        detail::check_qname(qname);
        detail::check_uri(uri);
        xmlNode* px = insert_(end(), create_element_(qname, uri));
        return basic_node_ptr<element>( static_cast<element*>(px->_private) );
    }


    basic_node_ptr<element> child_node_list::push_back_element(const std::string& name,
                                                               basic_xmlns_ptr<const xmlns> ns)
    {
        detail::check_local_part(name);
        xmlNode* px = insert_(end(), create_element_(name, ns));
        return basic_node_ptr<element>( static_cast<element*>(px->_private) );
    }


    basic_node_ptr<text> child_node_list::push_back_text(const std::string& value)
    {
        xmlNode* px = insert_(end(), create_text_(value));
        return basic_node_ptr<text>( static_cast<text*>(px->_private) );
    }


    basic_node_ptr<text> child_node_list::push_back_cdata(const std::string& value)
    {
        xmlNode* px = insert_(end(), create_cdata_(value));
        return basic_node_ptr<text>( static_cast<text*>(px->_private) );
    }


    basic_node_ptr<comment> child_node_list::push_back_comment(const std::string& value)
    {
        xmlNode* px = insert_(end(), create_comment_(value));
        return basic_node_ptr<comment>( static_cast<comment*>(px->_private) );
    }


    basic_node_ptr<instruction> child_node_list::push_back_instruction(const std::string& target,
                                                                       const std::string& value)
    {
        xmlNode* px = insert_(end(), create_instruction_(target, value));
        return basic_node_ptr<instruction>( static_cast<instruction*>(px->_private) );
    }


    basic_node_ptr<child_node> child_node_list::push_back_clone(const child_node& child)
    {
        xmlNode* px = child.clone_raw(true);  // recursive clone: never returns null.
        px = insert_(end(), px);
        return basic_node_ptr<child_node>( static_cast<child_node*>(px->_private) );
    }


    basic_node_ptr<child_node> child_node_list::push_back_adopt(child_node& child)
    {
        xmlNode* px = insert_(end(), child.raw());
        return basic_node_ptr<child_node>( static_cast<child_node*>(px->_private) );
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Modifiers: push_front
    //! \{


    basic_node_ptr<element> child_node_list::push_front_element(const std::string& qname)
    {
        detail::check_qname(qname);
        xmlNode* px = insert_(begin(), create_element_(qname));
        return basic_node_ptr<element>( static_cast<element*>(px->_private) );
    }


    basic_node_ptr<element> child_node_list::push_front_element(const std::string& qname,
                                                                const std::string& uri)
    {
        detail::check_qname(qname);
        detail::check_uri(uri);
        xmlNode* px = insert_(begin(), create_element_(qname, uri));
        return basic_node_ptr<element>( static_cast<element*>(px->_private) );
    }


    basic_node_ptr<element> child_node_list::push_front_element(const std::string& name,
                                                                basic_xmlns_ptr<const xmlns> ns)
    {
        detail::check_local_part(name);
        xmlNode* px = insert_(begin(), create_element_(name, ns));
        return basic_node_ptr<element>( static_cast<element*>(px->_private) );
    }


    basic_node_ptr<text> child_node_list::push_front_text(const std::string& value)
    {
        xmlNode* px = insert_(begin(), create_text_(value));
        return basic_node_ptr<text>( static_cast<text*>(px->_private) );
    }


    basic_node_ptr<text> child_node_list::push_front_cdata(const std::string& value)
    {
        xmlNode* px = insert_(begin(), create_cdata_(value));
        return basic_node_ptr<text>( static_cast<text*>(px->_private) );
    }


    basic_node_ptr<comment> child_node_list::push_front_comment(const std::string& value)
    {
        xmlNode* px = insert_(begin(), create_comment_(value));
        return basic_node_ptr<comment>( static_cast<comment*>(px->_private) );
    }


    basic_node_ptr<instruction> child_node_list::push_front_instruction(const std::string& target,
                                                                        const std::string& value)
    {
        xmlNode* px = insert_(begin(), create_instruction_(target, value));
        return basic_node_ptr<instruction>( static_cast<instruction*>(px->_private) );
    }


    basic_node_ptr<child_node> child_node_list::push_front_clone(const child_node& child)
    {
        xmlNode* px = child.clone_raw(true);  // recursive clone: never returns null.
        px = insert_(begin(), px);
        return basic_node_ptr<child_node>( static_cast<child_node*>(px->_private) );
    }


    basic_node_ptr<child_node> child_node_list::push_front_adopt(child_node& child)
    {
        xmlNode* px = insert_(begin(), child.raw());
        return basic_node_ptr<child_node>( static_cast<child_node*>(px->_private) );
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Modifiers: pop
    //! \{


    void child_node_list::pop_back()
    {
        // Check the list is not empty.
        if (empty())
        {
            throw bad_dom_operation("pop_back() called on an empty child_node_list");
        }
        // Find the last child node in the list (before-the-end iterator).
        iterator last;
        for (iterator i = begin(); i != end(); ++i)
        {
            last = i;
        }
        // Erase the last child node from the list.
        erase(last);
    }


    void child_node_list::pop_front()
    {
        // Check the list is not empty.
        if (empty())
        {
            throw bad_dom_operation("pop_front() called on an empty child_node_list");
        }
        // Erase the first child node from the list.
        erase(begin());
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Modifiers: insert
    //! \{


    basic_node_ptr<element> child_node_list::insert_element(iterator pos,
                                                            const std::string& qname)
    {
        detail::check_qname(qname);
        xmlNode* px = insert_(pos, create_element_(qname));
        return basic_node_ptr<element>( static_cast<element*>(px->_private) );
    }


    basic_node_ptr<element> child_node_list::insert_element(iterator pos,
                                                            const std::string& qname,
                                                            const std::string& uri)
    {
        detail::check_qname(qname);
        detail::check_uri(uri);
        xmlNode* px = insert_(pos, create_element_(qname, uri));
        return basic_node_ptr<element>( static_cast<element*>(px->_private) );
    }


    basic_node_ptr<element> child_node_list::insert_element(iterator pos,
                                                            const std::string& name,
                                                            basic_xmlns_ptr<const xmlns> ns)
    {
        detail::check_local_part(name);
        xmlNode* px = insert_(pos, create_element_(name, ns));
        return basic_node_ptr<element>( static_cast<element*>(px->_private) );
    }


    basic_node_ptr<text> child_node_list::insert_text(iterator pos, const std::string& value)
    {
        xmlNode* px = insert_(pos, create_text_(value));
        return basic_node_ptr<text>( static_cast<text*>(px->_private) );
    }


    basic_node_ptr<text> child_node_list::insert_cdata(iterator pos, const std::string& value)
    {
        xmlNode* px = insert_(pos, create_cdata_(value));
        return basic_node_ptr<text>( static_cast<text*>(px->_private) );
    }


    basic_node_ptr<comment> child_node_list::insert_comment(iterator pos,
                                                            const std::string& value)
    {
        xmlNode* px = insert_(pos, create_comment_(value));
        return basic_node_ptr<comment>( static_cast<comment*>(px->_private) );
    }


    basic_node_ptr<instruction> child_node_list::insert_instruction(iterator pos,
                                                                    const std::string& target,
                                                                    const std::string& value)
    {
        xmlNode* px = insert_(pos, create_instruction_(target, value));
        return basic_node_ptr<instruction>( static_cast<instruction*>(px->_private) );
    }


    child_node_list::iterator child_node_list::insert_clone(iterator pos, const child_node& child)
    {
        xmlNode* px = child.clone_raw(true);  // recursive clone: never returns null.
        px = insert_(pos, px);
        return iterator( static_cast<child_node*>(px->_private) );
    }


    void child_node_list::insert_clone(iterator pos, const_iterator first, const_iterator last)
    {
        for (const_iterator i = first; i != last; ++i)
        {
            xmlNode* px = i->clone_raw(true);  // recursive clone: never returns null.
            px = insert_(pos, px);
            pos = iterator( static_cast<child_node*>(px->_private) );
            ++pos;
        }
    }


    child_node_list::iterator child_node_list::insert_adopt(iterator pos, child_node& child)
    {
        xmlNode* px = insert_(pos, child.raw());
        return iterator( static_cast<child_node*>(px->_private) );
    }


    void child_node_list::insert_adopt(iterator pos, iterator first, iterator last)
    {
        for (iterator i = first; i != last; )
        {
            xmlNode* px = insert_(pos, (i++)->raw());
            pos = iterator( static_cast<child_node*>(px->_private) );
            ++pos;
        }
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Modifiers: erase and clear
    //! \{


    child_node_list::iterator child_node_list::erase(iterator pos)
    {
        // Check the iterator to erase is not the end and belongs to this list.
        check_ownership_(pos);
        if (pos == end())
        {
            throw bad_dom_operation("the iterator to erase should not point to the end");
        }
        // Delete the node pointed by the iterator.
        basic_node_ptr<child_node> ptr = pos.ptr();
        ++pos;
        ptr.delete_();
        return pos;
    }


    child_node_list::iterator child_node_list::erase(iterator first, iterator last)
    {
        // Check iterator parameters.
        check_ownership_(first);
        check_ownership_(last);
        // Erase all the child nodes between the two iterators.
        for (iterator i = first; i != last; )
        {
            i = erase(i);
        }
        return last;
    }


    //! \}


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Private Functions
    //! \{


    const document& child_node_list::doc_() const
    {
        assert(raw_->doc != 0 && "owner document of child node list should not be null");
        return *(static_cast<const document*>(raw_->doc->_private));
    }


    const element* child_node_list::owner_element_() const
    {
        if (raw_->type == XML_ELEMENT_NODE)
        {
            return static_cast<const element*>(raw_->_private);
        }
        else
        {
            return 0;
        }
    }


    const child_node* child_node_list::first_() const
    {
        if (raw_->children != 0)
        {
            return static_cast<const child_node*>(raw_->children->_private);
        }
        else
        {
            return 0;
        }
    }


    void child_node_list::check_ownership_(iterator pos)
    {
        if (pos != end() && pos->raw()->parent != raw_)
        {
            throw bad_dom_operation("the iterator does not belong to this child_node_list");
        }
    }


    xmlNode* child_node_list::insert_(iterator pos, xmlNode* child)
    {
        // Check the ownership of the iterator parameter.
        check_ownership_(pos);
        // Unlink the child node from its previous owner.
        xmlUnlinkNode(child);
        // Insert the libxml2 node to this child node list.
        xmlNode* px = 0;
        if (pos == end())
        {
            px = xmlAddChild(raw_, child);
        }
        else
        {
            px = xmlAddPrevSibling(pos->raw(), child);
        }
        if (px == 0)
        {
            std::string what = "fail to insert: xmlAddChild()/xmlAddPrevSibling() returned null";
            throw internal_dom_error(what);
        }
        // Reconciliate XML namespaces as necessary.
        if (px->type == XML_ELEMENT_NODE)
        {
            int count = xmlReconciliateNs(px->doc, px);
            if (count < 0)
            {
                throw internal_dom_error("fail to reconciliate xmlns on the inserted element");
            }
        }
        // Return the inserted libxml2 node.
        return px;
    }


    xmlNode* child_node_list::create_element_(const std::string& qname)
    {
        // Split QName into prefix and local name.
        std::pair<std::string, std::string> name_pair = detail::split_qname(qname);
        const std::string& prefix = name_pair.first;
        const std::string& name = name_pair.second;
        // Find xmlns by prefix (if this child node list belongs to an element).
        xmlNs* ns = 0;
        if (raw_->type == XML_ELEMENT_NODE)
        {
            ns = xmlSearchNs( raw_->doc,
                              raw_,
                              prefix.empty() ? 0 : detail::to_xml_chars(prefix.c_str()) );
        }
        if (!prefix.empty() && ns == 0)
        {
            std::string what = "fail to create element " + qname
                             + ": xmlns for prefix " + prefix + " not found";
            throw bad_dom_operation(what);
        }
        // Create element under the xmlns.
        xmlNode* px = xmlNewDocNode( raw_->doc,
                                     ns,
                                     detail::to_xml_chars(name.c_str()),
                                     0 );
        if (px == 0)
        {
            std::string what = "fail to create libxml2 element node for " + name;
            throw internal_dom_error(what);
        }
        // Return the new element.
        return px;
    }


    xmlNode* child_node_list::create_element_(const std::string& qname, const std::string& uri)
    {
        // Split QName into prefix and local name.
        std::pair<std::string, std::string> name_pair = detail::split_qname(qname);
        const std::string& prefix = name_pair.first;
        const std::string& name = name_pair.second;
        // Create element without namespace.
        xmlNode* px = xmlNewDocNode( raw_->doc,
                                     0,
                                     detail::to_xml_chars(name.c_str()),
                                     0 );
        if (px == 0)
        {
            std::string what = "fail to create libxml2 element node for " + name;
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
        // Return the new element.
        return px;
    }


    xmlNode* child_node_list::create_element_(const std::string& name,
                                              basic_xmlns_ptr<const xmlns> ns)
    {
        xmlNode* px = xmlNewDocNode( raw_->doc,
                                     ( ns != 0 ? const_cast<xmlNs*>(ns->raw()) : 0 ),
                                     detail::to_xml_chars(name.c_str()),
                                     0 );
        if (px == 0)
        {
            std::string what = "fail to create libxml2 element node for " + name;
            throw internal_dom_error(what);
        }
        return px;
    }


    xmlNode* child_node_list::create_text_(const std::string& value)
    {
        xmlNode* px = xmlNewDocText(raw_->doc, detail::to_xml_chars(value.c_str()));
        if (px == 0)
        {
            std::string what = "fail to create libxml2 text node for " + value;
            throw internal_dom_error(what);
        }
        else
        {
            return px;
        }
    }


    xmlNode* child_node_list::create_cdata_(const std::string& value)
    {
        xmlNode* px = xmlNewCDataBlock( raw_->doc,
                                        detail::to_xml_chars(value.c_str()),
                                        static_cast<int>(value.size()) );
        if (px == 0)
        {
            std::string what = "fail to create libxml2 CData node for " + value;
            throw internal_dom_error(what);
        }
        else
        {
            return px;
        }
    }


    xmlNode* child_node_list::create_comment_(const std::string& value)
    {
        xmlNode* px = xmlNewDocComment(raw_->doc, detail::to_xml_chars(value.c_str()));
        if (px == 0)
        {
            std::string what = "fail to create libxml2 comment node for " + value;
            throw internal_dom_error(what);
        }
        else
        {
            return px;
        }
    }


    xmlNode* child_node_list::create_instruction_(const std::string& target,
                                                  const std::string& value)
    {
        xmlNode* px = xmlNewDocPI( raw_->doc,
                                   detail::to_xml_chars(target.c_str()),
                                   detail::to_xml_chars(value.c_str()) );
        if (px == 0)
        {
            std::string what = "fail to create libxml2 PI node for " + target;
            throw internal_dom_error(what);
        }
        else
        {
            return px;
        }
    }


    //! \}


}  // namespace xtree


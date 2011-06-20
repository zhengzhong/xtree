//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_ELEMENT_HPP_20080717__
#define XTREE_ELEMENT_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/types.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/basic_node_iterator.hpp"

#include "xtree/child_node.hpp"
#include "xtree/child_node_list.hpp"
#include "xtree/attribute.hpp"
#include "xtree/attribute_map.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <map>
#include <string>

#ifdef XTREE_MSVC
#  pragma warning(push)
#  pragma warning(disable: 4511 4512)  // noncopyable warnings
#endif


namespace xtree {


    class XTREE_DECL node_set;


    //! This class represents an XML element.
    class XTREE_DECL element: public child_node
    {

        //! \todo TODO: declare_namespace_().
        friend class attribute_map;

    public:

        typedef attribute_map::iterator          attribute_iterator;
        typedef attribute_map::const_iterator    const_attribute_iterator;

        typedef child_node_list::iterator        child_iterator;
        typedef child_node_list::const_iterator  const_child_iterator;

    public:

        ///////////////////////////////////////////////////////////////////////////////////////////
        // static
        //

        static element* cast(xmlNode* px)
        {
            return const_cast<element*>(cast_(px));
        }

        static const element* cast(const xmlNode* px)
        {
            return cast_(px);
        }

    public:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // constructor/destructor
        //

        //! Constructor.
        explicit element(xmlNode* px);

        //! Destructor.
        ~element();

        ////////////////////////////////////////////////////////////////////////////////////////////
        // element information
        //

        //! Returns the namespace URI of this element.
        std::string uri() const;

        //! Returns the namespace prefix of this element.
        std::string prefix() const;

        //! Returns the qualified name (prefix:name) of this element.
        std::string qname() const;

        //! Returns the tag of this element.
        std::string tag() const;

        //! Returns any text found before the element's begin tag and after the previous tag.
        std::string head() const;

        //! Returns any text found after the element's end tag and before the next tag.
        std::string tail() const;

        //! Sets the local name of this element.
        void set_name(const std::string& name);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // namespaces
        //

        //! Sets the namespace URI of this element. If the specified URI is empty, this function
        //! removes the original namespace URI (if it exists) from the element.
        //! \param uri  the new namespace URI to set.
        void set_uri(const std::string& uri);

        //! Declares a namespace on this element. The namespace URI is provided as an argument,
        //! while the namespace prefix is generated automatically.
        //!
        //! Note: in an XML document, namespace prefix is only an alias to a certain namespace URI,
        //! and different prefixes mapped to the same URI are semantically identical. Thus, to
        //! avoid prefix conflicts and ambiguity, each time a namespace URI is declared on the
        //! element, a new prefix will be generated automatically, and it is guaranteed to be
        //! unique in the document scope. Users are not allowed to choose a customized namespace
        //! prefix.
        //!
        //! \param uri  the namespace URI to declare.
        //! \return the auto-generated prefix mapped to the namespace URI.
        //! \throw bad_dom_operation  if the namespace URI is invalid.
        std::string declare_namespace(const std::string& uri);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // attributes
        //

        //! Returns the value of an attribute by name.
        //! \param name  the attribute name.
        //! \return the attribute value, or an empty string if the attribute does not exist.
        std::string attr(const std::string& name) const
        {
            return attrs_.get(name);
        }

        //! Returns the value of an attribute by name and namespace URI.
        //! \param name  the attribute name.
        //! \param uri   the attribute namespace URI.
        //! \return the attribute value, or an empty string if the attribute does not exist.
        std::string attr(const std::string& name, const std::string& uri) const
        {
            return attrs_.get(name, uri);
        }

        //! Sets an attribute to this element. If the attribute with the same name already exists,
        //! its value will be updated.
        //! \param name   the attribute name.
        //! \param value  the attribute value.
        void set_attr(const std::string& name, const std::string& value)
        {
            attrs_.set(name, value);
        }

        //! Sets an attribute to this element. If the attribute with the same name and URI already
        //! exists, its value will be updated.
        //! \param name   the attribute name.
        //! \param uri    the attribute URI.
        //! \param value  the attribute value.
        void set_attr(const std::string& name, const std::string& uri, const std::string& value)
        {
            attrs_.set(name, uri, value);
        }

        //! Returns a reference to the attribute map of this element.
        //! \return a reference to the attribute map of this element.
        attribute_map& attrs()
        {
            return attrs_;
        }

        //! Const version of attributes().
        const attribute_map& attrs() const
        {
            return attrs_;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: capacity
        //

        bool empty() const
        {
            return children_.empty();
        }

        size_type size() const
        {
            return children_.size();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: iterators
        //

        child_iterator begin()
        {
            return children_.begin();
        }

        child_iterator end()
        {
            return children_.end();
        }

        const_child_iterator begin() const
        {
            return children_.begin();
        }

        const_child_iterator end() const
        {
            return children_.end();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers
        //

        child_node& front()
        {
            return children_.front();
        }

        const child_node& front() const
        {
            return children_.front();
        }

        child_node& back()
        {
            return children_.back();
        }

        const child_node& back() const
        {
            return children_.back();
        }

        void clear()
        {
            children_.clear();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers/push_back
        //

        basic_node_ptr<element> push_back_element(const std::string& name)
        {
            return children_.push_back_element(name);
        }

        basic_node_ptr<element> push_back_element(const std::string& name, const std::string& uri)
        {
            return children_.push_back_element(name, uri);
        }

        basic_node_ptr<text> push_back_text(const std::string& value)
        {
            return children_.push_back_text(value);
        }

        basic_node_ptr<text> push_back_cdata(const std::string& value)
        {
            return children_.push_back_cdata(value);
        }

        basic_node_ptr<comment> push_back_comment(const std::string& value)
        {
            return children_.push_back_comment(value);
        }

        basic_node_ptr<xml_pi> push_back_xml_pi(const std::string& target, const std::string& value)
        {
            return children_.push_back_xml_pi(target, value);
        }

        basic_node_ptr<child_node> push_back_clone(const child_node& child)
        {
            return children_.push_back_clone(child);
        }

        basic_node_ptr<child_node> push_back_adopt(child_node& child)
        {
            return children_.push_back_adopt(child);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers/push_front
        //

        basic_node_ptr<element> push_front_element(const std::string& name)
        {
            return children_.push_front_element(name);
        }

        basic_node_ptr<element> push_front_element(const std::string& name, const std::string& uri)
        {
            return children_.push_front_element(name, uri);
        }

        basic_node_ptr<text> push_front_text(const std::string& value)
        {
            return children_.push_front_text(value);
        }

        basic_node_ptr<text> push_front_cdata(const std::string& value)
        {
            return children_.push_front_cdata(value);
        }

        basic_node_ptr<comment> push_front_comment(const std::string& value)
        {
            return children_.push_front_comment(value);
        }

        basic_node_ptr<xml_pi> push_front_xml_pi(const std::string& target, const std::string& value)
        {
            return children_.push_front_xml_pi(target, value);
        }

        basic_node_ptr<child_node> push_front_clone(const child_node& child)
        {
            return children_.push_front_clone(child);
        }

        basic_node_ptr<child_node> push_front_adopt(child_node& child)
        {
            return children_.push_front_adopt(child);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers/pop
        //

        void pop_back()
        {
            children_.pop_back();
        }

        void pop_front()
        {
            children_.pop_front();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers/insert
        //

        basic_node_ptr<element> insert_element(child_iterator pos, const std::string& name)
        {
            return children_.insert_element(pos, name);
        }

        basic_node_ptr<element> insert_element(child_iterator pos,
                                               const std::string& name,
                                               const std::string& uri)
        {
            return children_.insert_element(pos, name, uri);
        }

        basic_node_ptr<text> insert_text(child_iterator pos, const std::string& value)
        {
            return children_.insert_text(pos, value);
        }

        basic_node_ptr<text> insert_cdata(child_iterator pos, const std::string& value)
        {
            return children_.insert_cdata(pos, value);
        }

        basic_node_ptr<comment> insert_comment(child_iterator pos, const std::string& value)
        {
            return children_.insert_comment(pos, value);
        }

        basic_node_ptr<xml_pi> insert_xml_pi(child_iterator pos,
                                             const std::string& target,
                                             const std::string& value)
        {
            return children_.insert_xml_pi(pos, target, value);
        }

        child_iterator insert_clone(child_iterator pos, const child_node& child)
        {
            return children_.insert_clone(pos, child);
        }

        void insert_clone(child_iterator pos, const_child_iterator first, const_child_iterator last)
        {
            return children_.insert_clone(pos, first, last);
        }

        child_iterator insert_adopt(child_iterator pos, child_node& child)
        {
            return children_.insert_adopt(pos, child);
        }

        void insert_adopt(child_iterator pos, child_iterator first, child_iterator last)
        {
            return children_.insert_adopt(pos, first, last);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers/erase
        //

        child_iterator erase(child_iterator pos)
        {
            return children_.erase(pos);
        }

        child_iterator erase(child_iterator first, child_iterator last)
        {
            return children_.erase(first, last);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // subelements
        //

        //! Returns the first subelement, or null if no subelement exists.
        //! \return the first subelement, or null if no subelement exists.
        basic_node_ptr<element> find_first_elem()
        {
            return basic_node_ptr<element>( const_cast<element*>(find_first_elem_()) );
        }

        //! Const version of find_first_elem(const std::string& name).
        basic_node_ptr<const element> find_first_elem() const
        {
            return basic_node_ptr<const element>( find_first_elem_() );
        }

        //! Returns the last subelement, or null if no subelement exists.
        //! \return the last subelement, or null if no subelement exists.
        basic_node_ptr<element> find_last_elem()
        {
            return basic_node_ptr<element>( const_cast<element*>(find_last_elem_()) );
        }

        //! Const version of find_last_elem(const std::string& name).
        basic_node_ptr<const element> find_last_elem() const
        {
            return basic_node_ptr<const element>( find_last_elem_() );
        }

        //! Returns the first subelement with the specified local name regardless of the
        //! namespace, or null if no matching subelement is found.
        //! \param name  the local name of the subelement.
        //! \return the first subelement found, or null if no matching subelement is found.
        basic_node_ptr<element> find_elem_by_name(const std::string& name)
        {
            return basic_node_ptr<element>( const_cast<element*>(find_elem_by_name_(name)) );
        }

        //! Const version of find_elem_by_name(const std::string& name).
        basic_node_ptr<const element> find_elem_by_name(const std::string& name) const
        {
            return basic_node_ptr<const element>( find_elem_by_name_(name) );
        }

        //! Returns the first descendant element with the specified path regardless of the
        //! namespace, or null if no matching descendant element is found.
        //! \param path  the path to the descendant element.
        //! \return the first descendant element found, or null if not found.
        basic_node_ptr<element> find_elem_by_path(const std::string& path)
        {
            return basic_node_ptr<element>( const_cast<element*>(find_elem_by_path_(path)) );
        }

        //! Const version of find_elem_by_path(const std::string& path).
        basic_node_ptr<const element> find_elem_by_path(const std::string& path) const
        {
            return basic_node_ptr<const element>( find_elem_by_path_(path) );
        }

        //! Returns the first subelement with the specified local name and namespace URI, or null
        //! if no matching subelement is found.
        //! \param name  the local name of the subelement.
        //! \param uri   the namespace URI of the subelement.
        //! \return the first subelement found, or null if no matching subelement is found.
        basic_node_ptr<element> find_elem(const std::string& name, const std::string& uri)
        {
            return basic_node_ptr<element>( const_cast<element*>(find_elem_(name, uri)) );
        }

        //! Const version of find_elem(const std::string& name, const std::string& uri).
        basic_node_ptr<const element> find_elem(const std::string& name, const std::string& uri) const
        {
            return basic_node_ptr<const element>( find_elem_(name, uri) );
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // sibling elements
        //

        //! Returns the previous sibling element.
        //! \return the previous sibling element.
        basic_node_ptr<element> get_prev_sibling_elem()
        {
            return basic_node_ptr<element>( const_cast<element*>(get_prev_sibling_elem_()) );
        }

        //! Returns the next sibling element.
        //! \return the next sibling element.
        basic_node_ptr<element> get_next_sibling_elem()
        {
            return basic_node_ptr<element>( const_cast<element*>(get_next_sibling_elem_()) );
        }

        //! Const version of prev_elem().
        basic_node_ptr<const element> get_prev_sibling_elem() const
        {
            return basic_node_ptr<const element>( get_prev_sibling_elem_() );
        }

        //! Const version of next_elem().
        basic_node_ptr<const element> get_next_sibling_elem() const
        {
            return basic_node_ptr<const element>( get_next_sibling_elem_() );
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // XPath
        //

        //! Evaluates the XPath expression and selects nodes.
        //! \param xpath  the XPath expression, should be evaluated to a node set.
        //! \param nodes  the result node set.
        void select_nodes(const std::string& xpath, node_set& nodes);

        //! Evaluates the XPath expression and selects nodes.
        //! \param xpath  the XPath expression, should be evaluated to a node set.
        //! \param ns_mapping  the XML namespace mapping for the XPath.
        //! \param nodes  the result node set.
        void select_nodes(const std::string& xpath,
                          const std::pair<std::string, std::string>& ns_mapping,
                          node_set& nodes);

        //! Evaluates the XPath expression and selects nodes.
        //! \param xpath  the XPath expression, should be evaluated to a node set.
        //! \param ns_mappings  the XML namespace mappings for the XPath.
        //! \param nodes  the result node set.
        void select_nodes(const std::string& xpath,
                          const std::map<std::string, std::string>& ns_mappings,
                          node_set& nodes);

    private:

        ///////////////////////////////////////////////////////////////////////////////////////////
        // private static
        //

        static const element* cast_(const xmlNode* px);

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // private
        //

        element* clone(bool recursive) const;

        //! Returns a namespace declaration for the specified URI. This function will firstly
        //! search a namespace declaration for the URI under the element's context. If the URI
        //! has not been declared, it will declare a namespace for the URI on the element.
        //! \param uri  the namespace URI.
        //! \return a libxml2 namespace for the URI.
        xmlNs* declare_namespace_(const std::string& uri);

        //! Returns the first subelement.
        //! \return pointer to the first subelement, or null if no subelement exists.
        const element* find_first_elem_() const;

        //! Returns the last subelement.
        //! \return pointer to the last subelement, or null if no subelement exists.
        const element* find_last_elem_() const;

        //! Returns the first subelement with the specified local name regardless of the
        //! namespace, or null if no matching subelement is found.
        //! \param name  the local name of the subelement.
        //! \return pointer to the first subelement found, or null if not found.
        const element* find_elem_by_name_(const std::string& name) const;

        //! Returns the first descendant element with the specified path regardless of the
        //! namespace, or null if no descendant element is found.
        //! \param path  the path to the descendant element.
        //! \return pointer to the first descendant element found, or null if not found.
        const element* find_elem_by_path_(const std::string& path) const;

        //! Returns the first subelement with the specified local name and namespace URI, or null
        //! if no matching subelement is found.
        //! \param name  the local name of the subelement.
        //! \param uri   the namespace URI of the subelement.
        //! \return pointer to the first subelement found, or null if not found.
        const element* find_elem_(const std::string& name, const std::string& uri) const;

        //! Returns the previous sibling element.
        const element* get_prev_sibling_elem_() const;

        //! Returns the next sibling element.
        const element* get_next_sibling_elem_() const;

    private:

        child_node_list children_;  //!< Child nodes.
        attribute_map   attrs_;     //!< Attributes.

    };


}  // namespace xtree


#ifdef XTREE_MSVC
#  pragma warning(pop)  // noncopyable warnings
#endif


#endif  // XTREE_ELEMENT_HPP_20080717__


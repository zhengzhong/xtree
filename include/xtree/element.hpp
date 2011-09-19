//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_ELEMENT_HPP_20080717__
#define XTREE_ELEMENT_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/types.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/basic_node_iterator.hpp"
#include "xtree/basic_xmlns_ptr.hpp"
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


    class XTREE_DECL xmlns;
    class XTREE_DECL xpath;
    class XTREE_DECL xpath_result;
    class XTREE_DECL node_set;


    //! This class represents an XML element.
    class XTREE_DECL element: public child_node
    {

        friend class attribute_map;

    public:

        typedef attribute_map::iterator          attribute_iterator;
        typedef attribute_map::const_iterator    const_attribute_iterator;

        typedef child_node_list::iterator        child_iterator;
        typedef child_node_list::const_iterator  const_child_iterator;

    public:

        //! \cond DEV

        ///////////////////////////////////////////////////////////////////////////////////////////
        //! \name Static Cast Functions
        //! \{

        static element* cast(xmlNode* px)
        {
            return const_cast<element*>(cast_(px));
        }

        static const element* cast(const xmlNode* px)
        {
            return cast_(px);
        }

        //! \}

        //! \endcond

    public:

        //! \cond DEV

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Constructor and Destructor
        //! \{

        //! Constructor.
        explicit element(xmlNode* px);

        //! Destructor.
        ~element();

        //! \}

        //! \endcond

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Property Access
        //! \{

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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Namespace Access
        //! \{

        //! Declares a namespace on this element. This function refuses to declare the namespace
        //! if the prefix is already declared on the element.
        //!
        //! Notes:
        //!
        //! 1) Declaring a new namespace on the element might affect this element and its
        //!    descendants. This function will reconciliate namespaces after a successful
        //!    declaration, keeping the URIs of this element and its descendants unchanged.
        //!    This operation might change their prefixes.
        //! 2) Declaring an unprefixed namespace might cause inconsistence: if this element or any
        //!    of its descendants is not associated with a namespace, after such declaration,
        //!    it will be impossible to distinguish whether it is under the unprefixed namespace
        //!    or not.
        //!
        //! \param prefix  the namespace prefix.
        //! \param uri     the namespace URI.
        //! \return a pair where its first is either the new xmlns or the existing xmlns with the
        //!         same prefix, and its second is a bool indicating if a new xmlns is declared.
        std::pair<basic_xmlns_ptr<xmlns>, bool> declare_xmlns(const std::string& prefix,
                                                              const std::string& uri);

        //! Returns the namespace associate to this element.
        //! \return the namespace associate to this element.
        basic_xmlns_ptr<xmlns> get_xmlns();

        //! Const version of get_xmlns().
        basic_xmlns_ptr<const xmlns> get_xmlns() const;

        //! Associates a namespace to this element.
        //! \param ns  the namespace to associate.
        void set_xmlns(basic_xmlns_ptr<const xmlns> ns);

        //! Returns the first XML namespace declaration on this element.
        //! \return the first XML namespace declaration on this element, or null if not found.
        basic_xmlns_ptr<xmlns> get_first_xmlns()
        {
            return basic_xmlns_ptr<xmlns>(const_cast<xmlns*>(get_first_xmlns_()));
        }

        //! Const version of get_first_xmlns().
        basic_xmlns_ptr<const xmlns> get_first_xmlns() const
        {
            return basic_xmlns_ptr<const xmlns>(get_first_xmlns_());
        }

        //! Finds XML namespace by prefix. This function searches namespace declarations on this
        //! element and all its parents.
        //! \param prefix  the namespace prefix to search for.
        //! \return the XML namespace, or null if not found.
        basic_xmlns_ptr<xmlns> find_xmlns_by_prefix(const std::string& prefix)
        {
            return basic_xmlns_ptr<xmlns>(const_cast<xmlns*>(find_xmlns_by_prefix_(prefix)));
        }

        //! Const version of find_xlmns_by_prefix().
        basic_xmlns_ptr<const xmlns> find_xmlns_by_prefix(const std::string& prefix) const
        {
            return basic_xmlns_ptr<const xmlns>(find_xmlns_by_prefix_(prefix));
        }

        //! Finds XML namespace URI by prefix.
        //! \param prefix  the namespace prefix to search for.
        //! \return the namespace URI or an empty string if the prefix is not found.
        std::string find_uri_by_prefix(const std::string& prefix) const;

        //! Finds the XML namespace by URI. This function searches namespace declarations on this
        //! element and all its parents.
        //! \param uri  the namespace URI to search for.
        //! \return the XML namespace, or null if not found.
        basic_xmlns_ptr<xmlns> find_xmlns_by_uri(const std::string& uri)
        {
            return basic_xmlns_ptr<xmlns>(const_cast<xmlns*>(find_xmlns_by_uri_(uri)));
        }

        //! Const version of find_xmlns_by_uri().
        basic_xmlns_ptr<const xmlns> find_xmlns_by_uri(const std::string& uri) const
        {
            return basic_xmlns_ptr<const xmlns>(find_xmlns_by_uri_(uri));
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Attribute Access
        //! \{

        //! Returns the value of an attribute by QName.
        //! \param qname  the attribute QName.
        //! \return the attribute value, or an empty string if the attribute does not exist.
        std::string attr(const std::string& qname) const
        {
            return attrs_.get(qname);
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
        //! \param qname  the attribute QName.
        //! \param value  the attribute value.
        void set_attr(const std::string& qname, const std::string& value)
        {
            attrs_.update(qname, value);
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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Capacity
        //! \{

        bool empty() const
        {
            return children_.empty();
        }

        size_type size() const
        {
            return children_.size();
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Iterators
        //! \{

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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Front and Back
        //! \{

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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Modifiers: push_back
        //! \{

        basic_node_ptr<element> push_back_element(const std::string& qname)
        {
            return children_.push_back_element(qname);
        }

        basic_node_ptr<element> push_back_element(const std::string& qname, const std::string& uri)
        {
            return children_.push_back_element(qname, uri);
        }

        basic_node_ptr<element> push_back_element(const std::string& name,
                                                  basic_xmlns_ptr<const xmlns> ns)
        {
            return children_.push_back_element(name, ns);
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

        basic_node_ptr<instruction> push_back_instruction(const std::string& target,
                                                          const std::string& value)
        {
            return children_.push_back_instruction(target, value);
        }

        basic_node_ptr<child_node> push_back_clone(const child_node& child)
        {
            return children_.push_back_clone(child);
        }

        basic_node_ptr<child_node> push_back_adopt(child_node& child)
        {
            return children_.push_back_adopt(child);
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Modifiers: push_front
        //! \{

        basic_node_ptr<element> push_front_element(const std::string& qname)
        {
            return children_.push_front_element(qname);
        }

        basic_node_ptr<element> push_front_element(const std::string& qname, const std::string& uri)
        {
            return children_.push_front_element(qname, uri);
        }

        basic_node_ptr<element> push_front_element(const std::string& name,
                                                   basic_xmlns_ptr<const xmlns> ns)
        {
            return children_.push_front_element(name, ns);
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

        basic_node_ptr<instruction> push_front_instruction(const std::string& target,
                                                           const std::string& value)
        {
            return children_.push_front_instruction(target, value);
        }

        basic_node_ptr<child_node> push_front_clone(const child_node& child)
        {
            return children_.push_front_clone(child);
        }

        basic_node_ptr<child_node> push_front_adopt(child_node& child)
        {
            return children_.push_front_adopt(child);
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Modifiers: pop
        //! \{

        void pop_back()
        {
            children_.pop_back();
        }

        void pop_front()
        {
            children_.pop_front();
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Modifiers: insert
        //! \{

        basic_node_ptr<element> insert_element(child_iterator pos, const std::string& qname)
        {
            return children_.insert_element(pos, qname);
        }

        basic_node_ptr<element> insert_element(child_iterator pos,
                                               const std::string& qname,
                                               const std::string& uri)
        {
            return children_.insert_element(pos, qname, uri);
        }

        basic_node_ptr<element> insert_element(child_iterator pos,
                                               const std::string& name,
                                               basic_xmlns_ptr<const xmlns> ns)
        {
            return children_.insert_element(pos, name, ns);
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

        basic_node_ptr<instruction> insert_instruction(child_iterator pos,
                                                       const std::string& target,
                                                       const std::string& value)
        {
            return children_.insert_instruction(pos, target, value);
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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Modifiers: erase and clear
        //! \{

        child_iterator erase(child_iterator pos)
        {
            return children_.erase(pos);
        }

        child_iterator erase(child_iterator first, child_iterator last)
        {
            return children_.erase(first, last);
        }

        void clear()
        {
            children_.clear();
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Subelement Access
        //! \{

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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Sibling Element Access
        //! \{

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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name XPath
        //! \{

        //! Evaluates an XPath expression.
        //! \param expr  the XPath expression.
        //! \return the generic XPath result.
        //! \throws xpath_error  if the XPath expression is invalid.
        void eval(const xpath& expr, xpath_result& result);

        //! Evaluates an XPath expression to a boolean.
        //! \param expr  the XPath expression.
        //! \return the boolean result.
        //! \throws xpath_error  if the XPath expression is invalid.
        bool eval_boolean(const xpath& expr) const;

        //! Evaluates an XPath expression to a number.
        //! \param expr  the XPath expression.
        //! \return the number result.
        //! \throws xpath_error  if the XPath expression is invalid.
        double eval_number(const xpath& expr) const;

        //! Evaluates an XPath expression to a string.
        //! \param expr  the XPath expression.
        //! \return the string result.
        //! \throws xpath_error  if the XPath expression is invalid.
        std::string eval_string(const xpath& expr) const;

        //! Evaluates the XPath expression to a node set. This is an alias to eval().
        //! \param expr   the XPath expression.
        //! \param nodes  the result node set.
        //! \throws xpath_error  if the XPath expression is invalid.
        void select_nodes(const xpath& expr, node_set& nodes);

        //! \}

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Private Functions
        //! \{

        static const element* cast_(const xmlNode* px);

        //! Declares a namespace on the element. This function refuses to declare the namespace
        //! if the prefix is already declared on the element.
        //! \param prefix  the namespace prefix.
        //! \param uri     the namespace URI.
        //! \return a pair where its first is either a new xmlns or an existing xmlns with the same
        //!         prefix, and its second is a bool indicating if a new xmlns is declared.
        std::pair<xmlns*, bool> declare_xmlns_(const std::string& prefix, const std::string& uri);

        //! Returns the first XML namespace declaration on this element.
        //! \return the first XML namespace declaration on this element, or null if not found.
        const xmlns* get_first_xmlns_() const;

        //! Find XML namespace by prefix. This function searches namespace declarations on this
        //! element and all its parents.
        //! \param prefix  the namespace prefix to search for.
        //! \return the XML namespace, or null if not found.
        const xmlns* find_xmlns_by_prefix_(const std::string& prefix) const;

        //! Find XML namespace by URI. This function searches namespace declarations on this
        //! element and all its parents.
        //! \param uri  the namespace URI to search for.
        //! \return the XML namespace, or null if not found.
        const xmlns* find_xmlns_by_uri_(const std::string& uri) const;

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

        //! \}

    private:

        child_node_list children_;  //!< Child nodes.
        attribute_map   attrs_;     //!< Attributes.

    };


}  // namespace xtree


#ifdef XTREE_MSVC
#  pragma warning(pop)  // noncopyable warnings
#endif


#endif  // XTREE_ELEMENT_HPP_20080717__


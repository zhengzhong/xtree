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


    class XTREE_DECL child_node_list;
    class XTREE_DECL attribute_map;

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
        //! \param uri     the namespace URI to declare.
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
        //! \param uri   the attribute's namespace URI.
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
        // children
        //

        //! Returns a reference to the child node list of this element.
        //! \return a reference to the child node list of this element.
        child_node_list& children();

        //! Const version of children().
        const child_node_list& children() const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // subelements
        //

        //! Returns the first subelement, or null if no subelement exists.
        //! \return the first subelement, or null if no subelement exists.
        basic_node_ptr<element> find_first_elem();

        //! Const version of find_first_elem(const std::string& name).
        basic_node_ptr<const element> find_first_elem() const;

        //! Returns the last subelement, or null if no subelement exists.
        //! \return the last subelement, or null if no subelement exists.
        basic_node_ptr<element> find_last_elem();

        //! Const version of find_last_elem(const std::string& name).
        basic_node_ptr<const element> find_last_elem() const;

        //! Returns the first subelement with the specified local name regardless of the
        //! namespace, or null if no matching subelement is found.
        //! \param name  the local name of the subelement.
        //! \return the first subelement found, or null if no matching subelement is found.
        basic_node_ptr<element> find_elem_by_name(const std::string& name);

        //! Const version of find_elem_by_name(const std::string& name).
        basic_node_ptr<const element> find_elem_by_name(const std::string& name) const;

        //! Returns the first subelement with the specified local name and namespace URI, or null
        //! if no matching subelement is found.
        //! \param name  the local name of the subelement.
        //! \param uri   the namespace URI of the subelement.
        //! \return the first subelement found, or null if no matching subelement is found.
        basic_node_ptr<element> find_elem(const std::string& name,
                                          const std::string& uri);

        //! Const version of find_elem(const std::string& name, const std::string& uri).
        basic_node_ptr<const element> find_elem(const std::string& name,
                                                const std::string& uri) const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // sibling elements
        //

        //! Returns the previous sibling element.
        //! \return the previous sibling element.
        basic_node_ptr<element> prev_sibling_elem();

        //! Const version of prev_elem().
        basic_node_ptr<const element> prev_sibling_elem() const;

        //! Returns the next sibling element.
        //! \return the next sibling element.
        basic_node_ptr<element> next_sibling_elem();

        //! Const version of next_elem().
        basic_node_ptr<const element> next_sibling_elem() const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // XPath
        //

        void select_nodes(const std::string& xpath, node_set& nodes);

        void select_nodes(const std::string& xpath,
                          const std::pair<std::string, std::string>& ns_mapping,
                          node_set& nodes);

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

        //! Returns the first subelement with the specified local name and namespace URI, or null
        //! if no matching subelement is found.
        //! \param name  the local name of the subelement.
        //! \param uri   the namespace URI of the subelement.
        //! \return pointer to the first subelement found, or null if not found.
        const element* find_elem_(const std::string& name, const std::string& uri) const;

        //! Returns the previous sibling element.
        const element* prev_sibling_elem_() const;

        //! Returns the next sibling element.
        const element* next_sibling_elem_() const;

    private:

        child_node_list children_;  //!< Child nodes.
        attribute_map   attrs_;     //!< Attributes.

    };


}  // namespace xtree


#ifdef XTREE_MSVC
#  pragma warning(pop)  // noncopyable warnings
#endif


#endif  // XTREE_ELEMENT_HPP_20080717__




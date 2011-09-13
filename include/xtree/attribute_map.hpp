//
// Created by ZHENG Zhong on 2008-07-26.
//

#ifndef XTREE_ATTRIBUTE_MAP_HPP_20080726__
#define XTREE_ATTRIBUTE_MAP_HPP_20080726__

#include "xtree/config.hpp"
#include "xtree/types.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/basic_node_iterator.hpp"
#include "xtree/attribute.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <utility>
#include <string>


namespace xtree {


    class XTREE_DECL element;


    //! This class represents an attribute map attached to an element. It may be viewed as a
    //! container of pointers to attribute. This class provides a similar API as std::map, the key
    //! of each attribute is its local name and namespace URI.
    class XTREE_DECL attribute_map
    {

    public:

        typedef basic_node_ptr<attribute>            auto_type;

        typedef attribute&                           reference;
        typedef const attribute&                     const_reference;

        typedef basic_node_iterator<attribute>       iterator;
        typedef basic_node_iterator<const attribute> const_iterator;

    public:

        //! \cond DEV

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Constructor and Destructor
        //! \{

        //! Constructs an attribute_map from a libxml2 element node. This function should NOT be
        //! called by client code. It is only used by element.
        explicit attribute_map(xmlNode* px);

        //! Destructor.
        ~attribute_map();

        //! \}

        //! \endcond

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Capacity
        //! \{

        //! Checks if this container is empty.
        //! \return true if this container is empty, false otherwise.
        bool empty() const;

        //! Returns the number of items in this map.
        //! \return the number of items in this map.
        size_type size() const;

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Iterators
        //! \{

        iterator begin()
        {
            return iterator( const_cast<attribute*>(first_attr_()) );
        }

        iterator end()
        {
            return iterator();
        }

        const_iterator begin() const
        {
            return const_iterator( first_attr_() );
        }

        const_iterator end() const
        {
            return const_iterator();
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Attribute Access
        //! \{

        //! Returns the value of an attribute by QName.
        //! \param qname  the attribute QName.
        //! \return the attribute value, or an empty string if the attribute does not exist.
        std::string get(const std::string& qname) const
        {
            const attribute* attr = find_attr_(qname);
            return (attr != 0 ? attr->value() : std::string());
        }

        //! Returns the value of an attribute by name and namespace URI.
        //! \param name  the attribute local name.
        //! \param uri   the attribute namespace URI.
        //! \return the attribute value, or an empty string if the attribute does not exist.
        std::string get(const std::string& name, const std::string& uri) const
        {
            const attribute* attr = find_attr_(name, uri);
            return (attr != 0 ? attr->value() : std::string());
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Find
        //! \{

        iterator find(const std::string& qname)
        {
            return iterator( const_cast<attribute*>(find_attr_(qname)) );
        }

        const_iterator find(const std::string& qname) const
        {
            return const_iterator( find_attr_(qname) );
        }

        iterator find(const std::string& name, const std::string& uri)
        {
            return iterator( const_cast<attribute*>(find_attr_(name, uri)) );
        }

        const_iterator find(const std::string& name, const std::string& uri) const
        {
            return const_iterator( find_attr_(name, uri) );
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Modifiers
        //! \{

        //! Inserts an attribute to the map. Like std::map::insert, this function will insert the
        //! attribute to the map only if no attribute with the same local name and namespace URI
        //! already exists.
        //! \param qname  QName of the attribute to insert.
        //! \param value  value of the attribute to insert.
        //! \return a std::pair containing an iterator to the attribute inserted (or an existing
        //!         one), and a boolean which is true if an insertion took place.
        std::pair<iterator, bool> insert(const std::string& qname, const std::string& value);

        //! Updates attribute value in the map.
        //! \param qname  QName of the attribute to update.
        //! \param value  value of the attribute to update.
        //! \return an iterator to the updated attribute.
        iterator update(const std::string& qname, const std::string& value);

        //! Removes and destroys the attribute at the given position from the attribute map.
        //! \post size() is one less.
        //! \param pos  the position of the attribute to erase.
        //! \return an iterator to the attribute after the erased one.
        iterator erase(iterator pos);

        //! Removes and destroys all the attributes between the first position (included) and the
        //! last position (excluded). This function has undefined behavior if the first iterator
        //! cannot reach the last iterator by increasing itself.
        //! \post size() is N less (N is the number of attributes erased).
        //! \param first  the first position (included) of the attribute to erase.
        //! \param last   the last position (excluded) of the attribute to erase.
        //! \return an iterator to the attribute after the last erased one (thus, ==last).
        iterator erase(iterator first, iterator last);

        //! Removes and destroys the attribute of the given name from the attribute map. If the
        //! attribute cannot be found, this function does nothing.
        //! \param qname  the QName of the attribute to erase.
        //! \return 1 if the attribute is erased, 0 if the attribute is not found.
        size_type erase(const std::string& qname);

        //! Removes and destroys the attribute of the given name and namespace URI from the
        //! attribute map. If the attribute cannot be found, this function does nothing.
        //! \param name  the name of the attribute to erase.
        //! \param uri   the namespace URI of the attribute to erase.
        //! \return 1 if the attribute is erased, 0 if the attribute is not found.
        size_type erase(const std::string& name, const std::string& uri);

        //! Removes all the attribute objects in this map.
        void clear()
        {
            erase(begin(), end());
        }

        //! \}

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Non-Copyable
        //! \{

        //! Non-implemented copy constructor.
        attribute_map(const attribute_map&);

        //! Non-implemented copy assignment.
        attribute_map& operator=(const attribute_map&);

        //! \}

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Private Functions
        //! \{

        xmlNode* raw()
        {
            assert(raw_ != 0 && "Underlying libxml2 node should not be null.");
            return raw_;
        }

        const xmlNode* raw() const
        {
            assert(raw_ != 0 && "Underlying libxml2 node should not be null.");
            return raw_;
        }

        element& owner();

        const element& owner() const;

        const attribute* first_attr_() const;

        //! Checks if the given iterator belongs to this attribute map.
        //! \param pos  the iterator to check.
        //! \throws bad_dom_operation  if the iterator does not belong to this attribute map.
        void check_ownership_(iterator pos);

        const attribute* find_attr_(const std::string& qname) const;

        const attribute* find_attr_(const std::string& name, const std::string& uri) const;

        attribute* create_attr_(const std::string& qname, const std::string& value);

        //! \}

    private:

        xmlNode* raw_;  //!< The libxml2 element holding the attribute map.

    };


} // namespace xtree


#endif // XTREE_ATTRIBUTE_MAP_HPP_20080726__



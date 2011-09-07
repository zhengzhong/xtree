//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_ATTRIBUTE_HPP_20080717__
#define XTREE_ATTRIBUTE_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/basic_xmlns_ptr.hpp"
#include "xtree/node.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <string>


namespace xtree {


    class XTREE_DECL attribute_map;
    class XTREE_DECL xmlns;


    //! This class represents an XML attribute.
    class XTREE_DECL attribute: public node
    {

    public:

        ///////////////////////////////////////////////////////////////////////////////////////////
        // static
        //

        static attribute* cast(xmlNode* px)
        {
            return const_cast<attribute*>(cast_(px));
        }

        static const attribute* cast(const xmlNode* px)
        {
            return cast_(px);
        }

    public:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // constructor/destructor
        //

        //! Constructs an attribute from a libxml2 node. This function should NOT be called by
        //! client code.
        explicit attribute(xmlNode* px);

        //! Destructor. This function should NOT be called by client code.
        ~attribute();

        ////////////////////////////////////////////////////////////////////////////////////////////
        // property access
        //

        //! Returns the namespace URI of this attribute, or an empty string if it hasn't one.
        std::string uri() const;

        //! Returns the namespace prefix of this attribute, or an empty string if it hasn't one.
        std::string prefix() const;

        //! Returns the full qualified name of this attribute.
        std::string qname() const;

        //! Sets the local name of this attribute.
        //! \param name  the new local name to set.
        void set_name(const std::string& name);

        //! Returns the value of this attribute.
        std::string value() const;

        //! Sets the value of this attribute.
        //! \param value  the value to set.
        void set_value(const std::string& value);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // namespace access
        //

        //! Returns the namespace associate to this attribute.
        //! \return the namespace associate to this attribute.
        basic_xmlns_ptr<xmlns> get_xmlns();

        //! Const version of get_xmlns().
        basic_xmlns_ptr<const xmlns> get_xmlns() const;

        //! Associates a namespace to this attribute.
        //! \param ns  the namespace to associate.
        void set_xmlns(basic_xmlns_ptr<const xmlns> ns);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // sibling access, required by basic_node_iterator
        //

        //! Returns the previous sibling.
        //! \return the previous sibling.
        basic_node_ptr<attribute> prev_sibling();

        //! Returns the next sibling.
        //! \return the next sibling.
        basic_node_ptr<attribute> next_sibling();

        //! Const version of prev_sibling().
        basic_node_ptr<const attribute> prev_sibling() const;

        //! Const version of next_sibling().
        basic_node_ptr<const attribute> next_sibling() const;

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // private static
        //

        static const attribute* cast_(const xmlNode* px);

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // private
        //

        attribute* clone(bool recursive) const;

        const attribute* prev_sibling_() const;

        const attribute* next_sibling_() const;

    };


}  // namespace xtree


#endif  // XTREE_ATTRIBUTE_HPP_20080717__


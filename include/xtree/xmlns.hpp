//
// Created by ZHENG Zhong on 2011-08-24.
//

#ifndef XTREE_XMLNS_HPP_20110824__
#define XTREE_XMLNS_HPP_20110824__

#include "xtree/config.hpp"
#include "xtree/xml_base.hpp"
#include "xtree/basic_xmlns_ptr.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <cassert>
#include <string>


namespace xtree {


    class XTREE_DECL document;
    class XTREE_DECL element;


    //! This class represents an XML namespace declaration attached to an element.
    class XTREE_DECL xmlns: private xml_base
    {

    public:

        static xmlns* get_or_create(xmlNs* px);

        static void delete_private(xmlNs* px);

    public:

        //! Constructs an object to wrap the underlying libxml2 namespace object. This function
        //! should NOT be called by client code.
        //! \param px  the underlying libxml2 namespace object to wrap.
        explicit xmlns(xmlNs* px);

        //! Destructor. Note: this function does NOT free the underlying libxml2 namespace on
        //! destruction. On the contrary, the underlying libxml2 namespace should firstly be freed,
        //! and the wrapper object is then destructed by the callback function.
        ~xmlns();

        //! Returns the namespace URI.
        //! \return the namespace URI.
        std::string uri() const;

        //! Sets the namespace URI. Note: this will change all the nodes which are under this
        //! namespace.
        //! \param uri  the new namespace URI.
        //TODO: void set_uri(const std::string& uri);

        //! Returns the namespace prefix.
        //! \return the namespace prefix.
        std::string prefix() const;

        //! Sets the namespace prefix. Note: this will change all the nodes which are under this
        //! namespace.
        //! \param prefix  the new namespace prefix.
        //TODO: void set_prefix(const std::string& prefix);

        //! Returns a string representation (an XML fragment) of this namespace.
        //! \return a string representation (an XML fragment) of this namespace.
        std::string str() const;

        //! Retrieves a string representation (an XML fragment) of this namespace.
        //! \param str  a string to hold the result.
        void str(std::string& str) const;

        //! Returns the owner document of this namespace.
        //! \return the owner document of this namespace.
        document& doc();

        //! Const version of doc().
        const document& doc() const;

        //! Returns the parent element node on which the namespace is declared.
        //! \return the parent element node on which the namespace is declared.
        //TODO: basic_node_ptr<element> parent();

        //! Const version of parent().
        //TODO: basic_node_ptr<const element> parent() const;

        //! Returns the next XML namespace declaration.
        //! \return the next XML namespace declaration, or null if not found.
        basic_xmlns_ptr<xmlns> next_sibling()
        {
            return basic_xmlns_ptr<xmlns>(const_cast<xmlns*>(next_sibling_()));
        }

        //! Const version of next_sibling().
        basic_xmlns_ptr<const xmlns> next_sibling() const
        {
            return basic_xmlns_ptr<const xmlns>(next_sibling_());
        }

    public:

        //! Returns the underlying libxml2 namespace object. This function should NOT be called by
        //! client code.
        xmlNs* raw()
        {
            assert(raw_ != 0 && "underlying libxml2 namespace should not be null");
            return raw_;
        }

        //! Const version of raw().
        const xmlNs* raw() const
        {
            assert(raw_ != 0 && "underlying libxml2 namespace should not be null");
            return raw_;
        }

        // TODO: very very dangerous!
        xmlNode* raw_as_node()
        {
            return reinterpret_cast<xmlNode*>(raw());
        }

        // TODO: very very dangerous!
        const xmlNode* raw_as_node() const
        {
            return reinterpret_cast<const xmlNode*>(raw());
        }

    private:

        //! Non-implemented copy constructor.
        xmlns(const xmlns&);

        //! Non-implemented copy assignment.
        xmlns& operator=(const xmlns&);

        //! Returns a const pointer to the parent element.
        //TODO: const element* parent_() const;

        //! Returns the next XML namespace declaration.
        //! \return the next XML namespace declaration, or null if not found.
        const xmlns* next_sibling_() const;

    private:

        xmlNs* raw_;  //!< The underlying libxml2 namespace object.

    };


}  // namespace xtree


#endif // XTREE_XMLNS_HPP_20110824__


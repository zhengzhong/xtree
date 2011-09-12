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

        //! \cond DEV

        static xmlns* get_or_create(xmlNs* px);

        static void delete_private(xmlNs* px);

        //! \endcond

    public:

        //! \cond DEV

        //! Constructs an object to wrap the underlying libxml2 namespace object. This function
        //! should NOT be called by client code.
        //! \param px  the underlying libxml2 namespace object to wrap.
        explicit xmlns(xmlNs* px);

        //! Destructor. Note: this function does NOT free the underlying libxml2 namespace on
        //! destruction. On the contrary, the underlying libxml2 namespace should firstly be freed,
        //! and the wrapper object is then destructed by the callback function.
        ~xmlns();

        //! \endcond

        //! Returns the namespace URI.
        //! \return the namespace URI.
        std::string uri() const;

        //! Returns the namespace prefix.
        //! \return the namespace prefix.
        std::string prefix() const;

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

    private:

        //! Non-implemented copy constructor.
        xmlns(const xmlns&);

        //! Non-implemented copy assignment.
        xmlns& operator=(const xmlns&);

    private:

        //! Returns the next XML namespace declaration.
        //! \return the next XML namespace declaration, or null if not found.
        const xmlns* next_sibling_() const;

    private:

        xmlNs* raw_;  //!< The underlying libxml2 namespace object.

    };


}  // namespace xtree


#endif // XTREE_XMLNS_HPP_20110824__


//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_TEXT_HPP_20080717__
#define XTREE_TEXT_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/child_node.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <string>

#ifdef XTREE_MSVC
#  pragma warning(push)
#  pragma warning(disable: 4511 4512)  // noncopyable warnings
#endif


namespace xtree {


    //! This class represents an XML text or CData section.
    class XTREE_DECL text: public child_node
    {

    public:

        //! \cond DEV

        static text* cast(xmlNode* px)
        {
            return const_cast<text*>(cast_(px));
        }

        static const text* cast(const xmlNode* px)
        {
            return cast_(px);
        }

        //! \endcond

    public:

        //! \cond DEV

        //! Constructor.
        explicit text(xmlNode* px);

        //! Destructor.
        ~text();

        //! \endcond

        //! Checks if this node is a CData section.
        //! \return true if this node is a CData section, false otherwise.
        bool is_cdata() const;

        //! Returns the value of this text or CData section.
        //! \return the value of this text or CData section.
        std::string value() const
        {
            return content();
        }

        //! Sets the value of this text or CData section.
        //! \param value  the value to set.
        void set_value(const std::string& value)
        {
            set_content(value);
        }

    private:

        static const text* cast_(const xmlNode* px);

    };


}  // namespace xtree


#ifdef XTREE_MSVC
#  pragma warning(pop)  // noncopyable warnings
#endif


#endif  // XTREE_TEXT_HPP_20080717__




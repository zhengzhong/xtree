//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_COMMENT_HPP_20080717__
#define XTREE_COMMENT_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/child_node.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <string>

#ifdef XTREE_MSVC
#  pragma warning(push)
#  pragma warning(disable: 4511 4512)  // noncopyable warnings
#endif


namespace xtree {


    //! This class represents an XML comment.
    class XTREE_DECL comment: public child_node
    {

    public:

        //! \cond DEV

        static comment* cast(xmlNode* px)
        {
            return const_cast<comment*>(cast_(px));
        }

        static const comment* cast(const xmlNode* px)
        {
            return cast_(px);
        }

        //! \endcond

    public:

        //! \cond DEV

        //! Constructor.
        explicit comment(xmlNode* px);

        //! Destructor.
        ~comment();

        //! \endcond

        //! Returns the value of this comment.
        //! \return the value of this comment.
        std::string value() const
        {
            return content();
        }

        //! Sets the value of this comment.
        //! \param value  the value to set.
        void set_value(const std::string& value)
        {
            set_content(value);
        }

    private:

        static const comment* cast_(const xmlNode* px);

    };


}  // namespace xtree


#ifdef XTREE_MSVC
#  pragma warning(pop)  // noncopyable warnings
#endif


#endif  // XTREE_COMMENT_HPP_20080717__




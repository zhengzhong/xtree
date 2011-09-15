//
// Created by ZHENG Zhong on 2009-07-27.
//

#ifndef XTREE_INSTRUCTION_HPP_20090727__
#define XTREE_INSTRUCTION_HPP_20090727__

#include "xtree/config.hpp"
#include "xtree/child_node.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <string>

#ifdef XTREE_MSVC
#  pragma warning(push)
#  pragma warning(disable: 4511 4512)  // noncopyable warnings
#endif


namespace xtree {


    //! This class represents a processing instruction node.
    class XTREE_DECL instruction: public child_node
    {

    public:

        //! \cond DEV

        static instruction* cast(xmlNode* px)
        {
            return const_cast<instruction*>(cast_(px));
        }

        static const instruction* cast(const xmlNode* px)
        {
            return cast_(px);
        }

        //! \endcond

    public:

        //! \cond DEV

        //! Constructor.
        explicit instruction(xmlNode* px);

        //! Destructor.
        ~instruction();

        //! \endcond

        //! Returns the target of this processing instruction.
        std::string target() const
        {
            return name();
        }

    private:

        static const instruction* cast_(const xmlNode* px);

    };


}  // namespace xtree


#ifdef XTREE_MSVC
#  pragma warning(pop)  // noncopyable warnings
#endif


#endif  // XTREE_INSTRUCTION_HPP_20090727__




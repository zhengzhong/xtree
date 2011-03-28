//
// Created by ZHENG Zhong on 2009-07-27.
//

#ifndef XTREE_XML_PI_HPP_20090727__
#define XTREE_XML_PI_HPP_20090727__

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
    class XTREE_DECL xml_pi: public child_node
    {

    public:

        static xml_pi* cast(xmlNode* px)
        {
            return const_cast<xml_pi*>(cast_(px));
        }

        static const xml_pi* cast(const xmlNode* px)
        {
            return cast_(px);
        }

    public:

        //! Constructor.
        explicit xml_pi(xmlNode* px);

        //! Destructor.
        ~xml_pi();

        //! Returns the target of this processing instruction.
        std::string target() const
        {
            return name();
        }

    private:

        static const xml_pi* cast_(const xmlNode* px);

    private:

        xml_pi* clone(bool recursive) const;

    };


}  // namespace xtree


#ifdef XTREE_MSVC
#  pragma warning(pop)  // noncopyable warnings
#endif


#endif  // XTREE_XML_PI_HPP_20090727__




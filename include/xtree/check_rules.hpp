//
// Created by ZHENG Zhong on 2011-06-16.
//

#ifndef XTREE_CHECK_RULES_HPP_20110616__
#define XTREE_CHECK_RULES_HPP_20110616__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"

#include <string>


//! \cond DEV

namespace xtree {
namespace detail {


    template<typename Char>
    inline bool is_name_start_char(Char c)
    {
        return (
            c == ':' || c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
            || (c >= 0xC0 && c <= 0xD6) || (c >= 0xD8 && c <= 0xF6)
            || (c >= 0xF8 && c <= 0x2FF) || (c >= 0x370 && c <= 0x37D)
            || (c >= 0x37F && c <= 0x1FFF) || (c >= 0x200C && c <= 0x200D)
            || (c >= 0x2070 && c <= 0x218F) || (c >= 0x2C00 && c <= 0x2FEF)
            || (c >= 0x3001 && c <= 0xD7FF) || (c >= 0xF900 && c <= 0xFDCF)
            || (c >= 0xFDF0 && c <= 0xFFFD) || (c >= 0x10000 && c <= 0xEFFFF)
        );
    }


    template<typename Char>
    inline bool is_name_char(Char c)
    {
        return (
            is_name_start_char(c)
            || c == '-' || c == '.' || (c >= '0' && c <= '9') || c == 0xB7
            || (c >= 0x0300 && c <= 0x036F) || (c >= 0x203F && c <= 0x2040)
        );
    }


    //! Checks if the given name conforms to the rules of element/attribute name.
    //! \param name  the name to check.
    //! \throws bad_dom_operation  if the name is not valid.
    template<class String>
    inline void check_name(const String& name)
    {
        if (name.empty())
        {
            throw bad_dom_operation("name should not be empty");
        }
        if (!is_name_start_char(name.at(0)))
        {
            throw bad_dom_operation("invalid start char in name " + name);
        }
        for (std::string::size_type i = 1U; i < name.size(); ++i)
        {
            if (!is_name_char(name.at(i)))
            {
                std::string bad_char(name, i, 1U);
                throw bad_dom_operation( "invalid char '" + bad_char + "' in name " + name);
            }
        }
    }


}  // namespace xtree::detail
}  // namespace xtree

//! \endcond


#endif  // XTREE_CHECK_RULES_HPP_20110616__


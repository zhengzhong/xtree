//
// Created by ZHENG Zhong on 2011-06-16.
//

#ifndef XTREE_CHECK_RULES_HPP_20110616__
#define XTREE_CHECK_RULES_HPP_20110616__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"

#include <string>


//! \cond DEV

//! Refer to: http://www.w3.org/TR/xml-names/
//!
//! QName          ::= PrefixedName | UnprefixedName
//! PrefixedName   ::= Prefix ':' LocalPart
//! UnprefixedName ::= LocalPart
//! Prefix         ::= NCName
//! LocalPart      ::= NCName
//! NCName         ::= Name - (Char* ':' Char*)  /* An XML Name, minus the ":" */
//! Name           ::= NameStartChar (NameChar)*
//! NameStartChar  ::= ":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF]
//!                  | [#x370-#x37D] | [#x37F-#x1FFF] | [#x200C-#x200D] | [#x2070-#x218F]
//!                  | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF] | [#xFDF0-#xFFFD]
//!                  | [#x10000-#xEFFFF]
//! NameChar       ::= NameStartChar | "-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]

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


    ////////////////////////////////////////////////////////////////////////////////////////////////


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
        if (name.find_first_of(':') != String::npos)
        {
            throw bad_dom_operation("invalid char ':' in name " + name);
        }
        if (!is_name_start_char(name.at(0)))
        {
            throw bad_dom_operation("invalid start char in name " + name);
        }
        for (String::size_type i = 1U; i < name.size(); ++i)
        {
            if (!is_name_char(name.at(i)))
            {
                std::string bad_char(name, i, 1U);
                throw bad_dom_operation( "invalid char '" + bad_char + "' in name " + name);
            }
        }
    }


    //! Checks if the given QName conforms to the rules of element/attribute QName.
    //! \param qname  the QName to check.
    //! \throws bad_dom_operation  if the QName is not valid.
    template<class String>
    inline void check_qname(const String& qname)
    {
        String::size_type pos = qname.find_first_of(':');
        if (pos != String::npos)
        {
            check_name(qname.substr(0, pos));
            check_name(qname.substr(pos + 1));
        }
        else
        {
            check_name(qname);
        }
    }


}  // namespace xtree::detail
}  // namespace xtree

//! \endcond


#endif  // XTREE_CHECK_RULES_HPP_20110616__


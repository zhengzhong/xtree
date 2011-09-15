//
// Created by ZHENG Zhong on 2011-08-25.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/check_rules.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>
#include <libxml/uri.h>

#include <string>


namespace xtree {
namespace detail {


    void check_nc_name(const std::string& value)
    {
        if (value.empty())
        {
            throw bad_dom_operation("NCName should not be empty");
        }
        int result = xmlValidateNCName(detail::to_xml_chars(value.c_str()), 0);
        if (result != 0)
        {
            std::string what = value + " does not conform to the lexical scope of NCName";
            throw bad_dom_operation(what);
        }
    }


    void check_name(const std::string& value)
    {
        if (value.empty())
        {
            throw bad_dom_operation("Name should not be empty");
        }
        int result = xmlValidateName(detail::to_xml_chars(value.c_str()), 0);
        if (result != 0)
        {
            std::string what = value + " does not conform to the lexical scope of Name";
            throw bad_dom_operation(what);
        }
    }


    void check_qname(const std::string& value)
    {
        if (value.empty())
        {
            throw bad_dom_operation("QName should not be empty");
        }
        int result = xmlValidateQName(detail::to_xml_chars(value.c_str()), 0);
        if (result != 0)
        {
            std::string what = value + " does not conform to the lexical scope of QName";
            throw bad_dom_operation(what);
        }
    }


    void check_uri(const std::string& value)
    {
        if (value.empty())
        {
            throw bad_dom_operation("URI should not be empty");
        }
        xmlURI* px = xmlParseURI(value.c_str());
        if (px == 0)
        {
            std::string what = value + " is not a valid URI";
            throw bad_dom_operation(what);
        }
        xmlFreeURI(px);
    }


}  // namespace xtree::detail
}  // namespace xtree

//! \endcond


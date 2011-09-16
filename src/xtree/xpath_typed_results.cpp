//
// Created by ZHENG Zhong on 2011-09-16.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/xpath_typed_results.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/xpath.h>

#include <string>


namespace xtree {


    xpath_boolean::xpath_boolean()
    {
        // Do nothing.
    }


    bool xpath_boolean::value() const
    {
        return (raw() != 0 && raw()->boolval != 0);
    }


    bool xpath_boolean::is_acceptable_(xpath_result_t type) const
    {
        return (type == boolean_result);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////


    xpath_number::xpath_number()
    {
        // Do nothing.
    }


    double xpath_number::value() const
    {
        return ( raw() != 0 ? raw()->floatval : 0 );
    }


    bool xpath_number::is_acceptable_(xpath_result_t type) const
    {
        return (type == number_result);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////


    xpath_string::xpath_string()
    {
        // Do nothing.
    }


    std::string xpath_string::value() const
    {
        return ( raw() != 0 && raw()->stringval != 0
               ? detail::to_chars(raw()->stringval)
               : std::string() );
    }


    bool xpath_string::is_acceptable_(xpath_result_t type) const
    {
        return (type == string_result);
    }


}  // namespace xtree


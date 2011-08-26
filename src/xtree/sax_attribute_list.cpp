//
// Created by ZHENG Zhong on 2011-05-12.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/sax_attribute_list.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/xmlstring.h>

#include <cassert>
#include <string>


namespace xtree {


    sax_attribute::sax_attribute(int index, const xmlChar** attrs)
    : name_(), prefix_(), uri_(), value_()
    {
        name_ = ( attrs[index * 5] != 0
                ? detail::to_chars(attrs[index * 5])
                : std::string() );
        prefix_ = ( attrs[index * 5 + 1] != 0
                  ? detail::to_chars(attrs[index * 5 + 1])
                  : std::string() );
        uri_ = ( attrs[index * 5 + 2] != 0
               ? detail::to_chars(attrs[index * 5 + 2])
               : std::string() );
        value_ = std::string( detail::to_chars(attrs[index * 5 + 3]),
                              detail::to_chars(attrs[index * 5 + 4]) );
    }


}  // namespace xtree


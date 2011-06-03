//
// Created by ZHENG Zhong on 2011-05-12.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/sax_attribute_list.hpp"
#include "xtree/sax_xmlns_context.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/xmlstring.h>

#include <cassert>
#include <string>


namespace xtree {


    sax_attribute::sax_attribute(): name_(), uri_(), prefix_(), value_()
    {
        // Do nothing.
    }


    sax_attribute::sax_attribute(const std::string& name,
                                 const std::string& uri,
                                 const std::string& prefix,
                                 const std::string& value)
    : name_(name), uri_(uri), prefix_(prefix), value_(value)
    {
        // Do nothing.
    }


}  // namespace xtree


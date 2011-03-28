//
// Created by ZHENG Zhong on 2008-07-21.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/dom_base.hpp"
#include "xtree/libxml2_globals.hpp"


namespace xtree {


    dom_base::dom_base()
    {
        // Call the instance function to make sure the globals singleton instance is created.
        // We don't need the singleton instance, so we don't care about the return value.
        detail::libxml2_globals::initialize();
    }


}  // namespace xtree


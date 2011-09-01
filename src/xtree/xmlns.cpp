//
// Created by ZHENG Zhong on 2011-08-24.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/xmlns.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>
#include <cassert>
#include <string>


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // static functions
    //


    xmlns* xmlns::get_or_create(xmlNs* px)
    {
        if (px == 0)
        {
            return 0;
        }
        if (px->_private == 0)
        {
            px->_private = new xmlns(px);
        }
        return static_cast<xmlns*>(px->_private);
    }


    void xmlns::delete_private(xmlNs* px)
    {
        if (px != 0 && px->_private != 0)
        {
            xmlns* wrapper = static_cast<xmlns*>(px->_private);
            delete wrapper;
            px->_private = 0;
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // public functions
    //


    xmlns::xmlns(xmlNs* px): raw_(px)
    {
        assert(raw_ != 0 && "Underlying libxml2 namespace should not be null");
    }


    xmlns::~xmlns()
    {
        raw_ = 0;
    }


    std::string xmlns::uri() const
    {
        return (raw()->href != 0 ? detail::to_chars(raw()->href) : std::string() );
    }


    std::string xmlns::prefix() const
    {
        return (raw()->prefix != 0 ? detail::to_chars(raw()->prefix) : std::string() );
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // private functions
    //


    const xmlns* xmlns::next_sibling_() const
    {
        return get_or_create(raw()->next);
    }


}  // namespace xtree






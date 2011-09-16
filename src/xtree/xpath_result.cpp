//
// Created by ZHENG Zhong on 2011-03-22.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/xpath_result.hpp"
#include "xtree/exceptions.hpp"

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <algorithm>  // std::swap
#include <string>


namespace xtree {


    xpath_result::xpath_result(): xpath_(), raw_(0)
    {
        // Do nothing.
    }


    xpath_result::xpath_result(const std::string& xpath, xmlXPathObject* px)
    : xpath_(xpath), raw_(px)
    {
        // Do nothing.
    }


    xpath_result::~xpath_result()
    {
        if (raw_ != 0)
        {
            xmlXPathFreeObject(raw_);
            raw_ = 0;
        }
    }


    void xpath_result::transfer(xpath_result& other)
    {
        if (this != &other)
        {
            if (!other.is_acceptable_(type()))
            {
                std::string what = "inconsistent XPath result type for " + xpath_;
                throw xpath_error(what);
            }
            std::swap(other.xpath_, xpath_);
            std::swap(other.raw_, raw_);
        }
    }


    xpath_result_t xpath_result::type() const
    {
        if (raw() == 0)
        {
            return undefined_result;
        }
        switch (raw()->type)
        {
        case XPATH_NODESET:
            return node_set_result;
        case XPATH_BOOLEAN:
            return boolean_result;
        case XPATH_NUMBER:
            return number_result;
        case XPATH_STRING:
            return string_result;
        default:
            return undefined_result;
        }
    }


    bool xpath_result::is_acceptable_(xpath_result_t) const
    {
        return true;  // always return true: this class represents a catch-all XPath result.
    }


}  // namespace xtree


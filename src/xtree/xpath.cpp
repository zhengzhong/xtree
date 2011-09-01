#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/xpath.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/xpath.h>

#include <cassert>
#include <map>
#include <string>


namespace xtree {


    xpath::xpath(const std::string& str): str_(str), raw_(0), registry_()
    {
        raw_ = xmlXPathCompile(detail::to_xml_chars(str.c_str()));
        if (raw_ == 0)
        {
            std::string what = "fail to compile XPath expression: " + str;
            throw xpath_error(what);
        }
    }


    xpath::xpath(const std::string& str, const std::string& prefix, const std::string& uri)
    : str_(str), raw_(0), registry_()
    {
        raw_ = xmlXPathCompile(detail::to_xml_chars(str.c_str()));
        if (raw_ == 0)
        {
            std::string what = "fail to compile XPath expression: " + str;
            throw xpath_error(what);
        }
        registry_.insert(std::make_pair(prefix, uri));
    }


    xpath::xpath(const xpath& rhs): str_(rhs.str_), raw_(0), registry_(rhs.registry_)
    {
        raw_ = xmlXPathCompile(detail::to_xml_chars(rhs.str_.c_str()));
        if (raw_ == 0)
        {
            std::string what = "fail to compile XPath expression: " + rhs.str_;
            throw xpath_error(what);
        }
    }


    xpath& xpath::operator=(const xpath& rhs)
    {
        if (this != &rhs)
        {
            // Recompile the XPath expression.
            xmlXPathCompExpr* px = xmlXPathCompile(detail::to_xml_chars(rhs.str_.c_str()));
            if (px == 0)
            {
                std::string what = "fail to compile XPath expression: " + rhs.str_;
                throw xpath_error(what);
            }
            // Free the underlying XPath expression object.
            assert(raw_ != 0);
            xmlXPathFreeCompExpr(raw_);
            raw_ = 0;
            // Reinitialize member variables.
            raw_ = px;
            str_ = rhs.str_;
            registry_ = rhs.registry_;
        }
        return *this;
    }


    xpath::~xpath()
    {
        assert(raw_ != 0);
        xmlXPathFreeCompExpr(raw_);
        raw_ = 0;
    }


}  // namespace xtree


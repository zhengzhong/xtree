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


    xpath::xpath(const std::string& str): str_(str), registry_(), raw_(0)
    {
        init_raw_(str);
    }


    xpath::xpath(const char* str): str_(str), registry_(), raw_(0)
    {
        init_raw_(str);
    }


    xpath::xpath(const std::string& str, const std::string& prefix, const std::string& uri)
    : str_(str), registry_(), raw_(0)
    {
        registry_.insert(std::make_pair(prefix, uri));
        init_raw_(str);
    }


    xpath::xpath(const std::string& str, const xmlns_registry& registry)
    : str_(str), registry_(registry), raw_(0)
    {
        init_raw_(str);
    }


    xpath::xpath(const xpath& rhs): str_(rhs.str_), registry_(rhs.registry_), raw_(0)
    {
        init_raw_(rhs.str_);
    }


    xpath& xpath::operator=(const xpath& rhs)
    {
        if (this != &rhs)
        {
            init_raw_(rhs.str_);
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


    void xpath::init_raw_(const std::string& str)
    {
        // Compile the XPath expression.
        xmlXPathCompExpr* px = xmlXPathCompile(detail::to_xml_chars(str.c_str()));
        if (px == 0)
        {
            std::string what = "fail to compile XPath expression: " + str;
            throw xpath_error(what);
        }
        // Free the underlying XPath expression object as necessary.
        if (raw_ != 0)
        {
            xmlXPathFreeCompExpr(raw_);
            raw_ = 0;
        }
        // Reset raw.
        raw_ = px;
    }


}  // namespace xtree


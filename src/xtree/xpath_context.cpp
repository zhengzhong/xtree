//
// Created by ZHENG Zhong on 2011-03-22.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/xpath_context.hpp"
#include "xtree/xpath.hpp"
#include "xtree/xpath_result.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/libxml2_utility.hpp"

#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <cassert>
#include <string>


namespace xtree {
namespace detail {


    namespace {


        //! Libxml2 callback function for XPath errors.
        //! \todo TODO: a better error reporting mechanism?
        void libxml2_on_xpath_error(void*, xmlError*)
        {
            // Do nothing: just to prevent libxml2 from writing error message to console.
        }


    }  // anonymous namespace


    ////////////////////////////////////////////////////////////////////////////////////////////////


    xpath_context::xpath_context(xmlDoc* px_doc, xmlNode* px_node): raw_(0)
    {
        assert(px_doc != 0);
        assert(px_node == 0 || px_node->doc == px_doc);
        raw_ = xmlXPathNewContext(px_doc);
        if (raw_ == 0)
        {
            std::string what = "fail to create xpath_context: xmlXPathNewContext() returned null";
            throw internal_dom_error(what);
        }
        raw_->node = px_node;
        raw_->error = &libxml2_on_xpath_error;
    }


    xpath_context::~xpath_context()
    {
        if (raw_ != 0)
        {
            xmlXPathFreeContext(raw_);
            raw_ = 0;
        }
    }


    void xpath_context::eval(const xpath& expr, xpath_result& result)
    {
        // Register XML namespaces.
        typedef xpath::xmlns_registry::const_iterator const_iterator;
        const xpath::xmlns_registry& registry = expr.get_xmlns_registry();
        for (const_iterator i = registry.begin(); i != registry.end(); ++i)
        {
            int ret_code = xmlXPathRegisterNs( raw_,
                                               detail::to_xml_chars(i->first.c_str()),
                                               detail::to_xml_chars(i->second.c_str()) );
            if (ret_code != 0)
            {
                std::string what = "fail to register namespace " + i->first + "=" + i->second
                                 + ": xmlXPathRegisterNs returned non-zero";
                throw xpath_error(what);
            }
        }
        // Evaluate the XPath expression.
        xmlXPathObject* px = xmlXPathCompiledEval(const_cast<xmlXPathCompExpr*>(expr.raw()), raw_); 
        if (px == 0)
        {
            std::string what = "fail to evaluate XPath '" + expr.str() + "': "
                             + detail::build_error_message(raw_->lastError);
            throw xpath_error(what);
        }
        xpath_result tmp_result(expr.str(), px);
        tmp_result.transfer(result);
    }


}  // namespace detail
}  // namespace xtree


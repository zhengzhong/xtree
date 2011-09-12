//
// Created by ZHENG Zhong on 2009-07-15.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/validity.hpp"

#include <cassert>
#include <string>


namespace xtree {


    namespace detail {


        void on_validity_error(void* ctxt, const char* message, ...)
        {
            assert(ctxt != 0);
            assert(message != 0);
            if (ctxt != 0 && message != 0)
            {
                validity* result = static_cast<validity*>(ctxt);
                result->add_error(message);
            }
        }


        void on_validity_warning(void* ctxt, const char* message, ...)
        {
            assert(ctxt != 0);
            assert(message != 0);
            if (ctxt != 0 && message != 0)
            {
                validity* result = static_cast<validity*>(ctxt);
                result->add_warning(message);
            }
        }


    }  // namespace xtree::detail


    ////////////////////////////////////////////////////////////////////////////////////////////////


    validity::validity(): errors_(), warnings_()
    {
        // Do nothing.
    }


    const validity::messages& validity::errors() const
    {
        return errors_;
    }


    const validity::messages& validity::warnings() const
    {
        return warnings_;
    }


    bool validity::is_valid() const
    {
        return errors_.empty();
    }


    void validity::add_error(const std::string& message)
    {
        errors_.push_back(message);
    }


    void validity::add_warning(const std::string& message)
    {
        warnings_.push_back(message);
    }


} // namespace xtree


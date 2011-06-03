//
// Created by ZHENG Zhong on 2011-05-12.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/sax_xmlns_context.hpp"
#include "xtree/exceptions.hpp"

#include <cassert>
#include <string>


namespace xtree {


    sax_xmlns_context::sax_xmlns_context(): max_level_(0), context_()
    {
        reset();
    }


    void sax_xmlns_context::reset()
    {
        max_level_ = 0;
        context_.clear();
        // Empty prefixes will finally fall into this mapping.
        context_.insert(std::make_pair(0, std::make_pair(std::string(), std::string())));
    }


    void sax_xmlns_context::push(int level, const std::string& prefix, const std::string& uri)
    {
        assert(level >= max_level_ && "level to push should not be less than max level");
        max_level_ = level;
        context_.insert(std::make_pair(level, std::make_pair(prefix, uri)));
    }


    void sax_xmlns_context::pop(int level)
    {
        assert(level > 0 && "level to pop should be strictly positive");
        for (int i = max_level_; i >= level; --i)
        {
            context_.erase(i);
        }
        max_level_ = level - 1;
    }


    const std::string& sax_xmlns_context::find_uri(const std::string& prefix) const
    {
        for (xmlns_map::const_reverse_iterator i = context_.rbegin(); i != context_.rend(); ++i)
        {
            if (i->second.first == prefix)
            {
                return i->second.second;
            }
        }
        throw sax_error("Fail to find namespace URI for prefix " + prefix);
    }


}  // namespace xtree


//
// Created by ZHENG Zhong on 2009-01-22.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/sax_error_info.hpp"
#include "xtree/exceptions.hpp"

#include <sstream>
#include <string>


namespace xtree {


    sax_error_info::sax_error_info(): message_(), file_(), line_(0U), column_(0U)
    {
        // Do nothing.
    }


    sax_error_info::sax_error_info(const std::string& message)
    : message_(message), file_(), line_(0U), column_(0U)
    {
        // Do nothing.
    }


    sax_error_info::sax_error_info(const char* message,
                                   const char* file,
                                   unsigned int line,
                                   unsigned int column)
    : message_(message != 0 ? message : std::string())
    , file_(file != 0 ? file : std::string())
    , line_(line), column_(column)
    {
        // Do nothing.
    }


    std::string sax_error_info::str() const
    {
        std::ostringstream oss;
        oss << (message_.empty() ? "error message not available" : message_);
        if (!file_.empty())
        {
            oss << " [in " << file_ << ", " << line_ << ":" << column_ << "]";
        }
        return oss.str();
    }


    sax_error sax_error_info::exc() const
    {
        return sax_error(str());
    }


} // namespace xtree


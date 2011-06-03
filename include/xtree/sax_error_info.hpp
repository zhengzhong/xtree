//
// Created by ZHENG Zhong on 2009-01-22.
//

#ifndef XTREE_SAX_ERROR_INFO_HPP_20090122__
#define XTREE_SAX_ERROR_INFO_HPP_20090122__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"

#include <sstream>
#include <string>


namespace xtree {


    class sax_error_info
    {

    public:

        //! Default constructor.
        explicit sax_error_info(): message_(), file_(), line_(0U), column_(0U)
        {
            // Do nothing.
        }

        explicit sax_error_info(const std::string& message)
        : message_(message), file_(), line_(0U), column_(0U)
        {
            // Do nothing.
        }

        explicit sax_error_info(const char* message,
                                const char* file,
                                unsigned int line,
                                unsigned int column)
        : message_(message != 0 ? message : std::string())
        , file_(file != 0 ? file : std::string())
        , line_(line), column_(column)
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.
        // Use auto-generated destructor.

        const std::string& message() const
        {
            return message_;
        }

        const std::string& file() const
        {
            return file_;
        }
        unsigned int line() const
        {
            return line_;
        }

        unsigned int column() const
        {
            return column_;
        }

        //! Returns a string representation of this error.
        //! \return a string representation of this error.
        std::string str() const
        {
            std::ostringstream oss;
            oss << (message_.empty() ? "error message not available" : message_);
            if (!file_.empty())
            {
                oss << " [in " << file_ << ", " << line_ << ":" << column_ << "]";
            }
            return oss.str();
        }

        //! Returns a sax_error exception object to wrap the error info.
        //! \return a sax_error exception object to wrap the error info.
        sax_error exc() const
        {
            return sax_error(str());
        }

    private:

        std::string  message_;
        std::string  file_;
        unsigned int line_;
        unsigned int column_;

        // TODO: std::string public_id_;
        // TODO: std::string system_id_;

    };


} // namespace xtree


#endif  // XTREE_SAX_ERROR_INFO_HPP_20090122__


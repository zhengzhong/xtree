//
// Created by ZHENG Zhong on 2009-01-22.
//

#ifndef XTREE_SAX_ERROR_INFO_HPP_20090122__
#define XTREE_SAX_ERROR_INFO_HPP_20090122__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"

#include <string>


namespace xtree {


    //! This class wraps a SAX error.
    class XTREE_DECL sax_error_info
    {

    public:

        //! Default constructor.
        explicit sax_error_info();

        explicit sax_error_info(const std::string& message);

        explicit sax_error_info(const char* message,
                                const char* file,
                                unsigned int line,
                                unsigned int column);

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
        std::string str() const;

        //! Returns a sax_error exception object to wrap the error info.
        //! \return a sax_error exception object to wrap the error info.
        sax_error exc() const;

    private:

        std::string  message_;
        std::string  file_;
        unsigned int line_;
        unsigned int column_;

    };


} // namespace xtree


#endif  // XTREE_SAX_ERROR_INFO_HPP_20090122__


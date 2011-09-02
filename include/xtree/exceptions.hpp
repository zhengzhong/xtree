//
// Created by ZHENG Zhong on 2008-07-16.
//

#ifndef XTREE_EXCEPTIONS_HPP_20080716__
#define XTREE_EXCEPTIONS_HPP_20080716__

#include "xtree/config.hpp"

#include <exception>
#include <string>


namespace xtree {


    //! This class represents an error related to XML DOM.
    class sax_error: public std::exception
    {

    public:

        //! Default constructor.
        explicit sax_error(): what_()
        {
            // Do nothing.
        }

        explicit sax_error(const std::string& what): what_(what)
        {
            // Do nothing.
        }

        virtual ~sax_error() throw()
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.

        virtual const char* what() const throw()
        {
            return what_.c_str();
        }

    private:

        std::string what_;

    };


    //! This class represents an error related to XML DOM.
    class dom_error: public std::exception
    {

    public:

        //! Default constructor.
        explicit dom_error(): what_()
        {
            // Do nothing.
        }

        explicit dom_error(const std::string& what): what_(what)
        {
            // Do nothing.
        }

        virtual ~dom_error() throw()
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.

        virtual const char* what() const throw()
        {
            return what_.c_str();
        }

    private:

        std::string what_;

    };


    //! This exception is thrown when user tries to use a null node pointer.
    class null_pointer: public dom_error
    {

    public:

        explicit null_pointer(): dom_error("null pointer")
        {
            // Do nothing.
        }

        virtual ~null_pointer() throw()
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.

    };


    //! This class represents an error of a bad operation on the XML DOM tree.
    class bad_dom_operation: public dom_error
    {

    public:

        explicit bad_dom_operation(): dom_error("bad DOM operation")
        {
            // Do nothing.
        }

        explicit bad_dom_operation(const std::string& what): dom_error(what)
        {
            // Do nothing.
        }

        virtual ~bad_dom_operation() throw()
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.

    };


    //! This class represents an error related to the underlying XML library.
    class internal_dom_error: public dom_error
    {

    public:

        explicit internal_dom_error(): dom_error("internal DOM error")
        {
            // Do nothing.
        }

        explicit internal_dom_error(const std::string& what): dom_error(what)
        {
            // Do nothing.
        }

        virtual ~internal_dom_error() throw()
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.

    };


    //! This class represents an error related to XPath.
    class xpath_error: public dom_error
    {

    public:

        explicit xpath_error(): dom_error("XPath error")
        {
            // Do nothing.
        }

        explicit xpath_error(const std::string& what): dom_error(what)
        {
            // Do nothing.
        }

        virtual ~xpath_error() throw()
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.

    };


}  // namespace xtree


#endif // XTREE_EXCEPTIONS_HPP_20080716__


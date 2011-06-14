//
// Created by ZHENG Zhong on 2011-03-22.
//

#ifndef XTREE_XPATH_HPP_20110322__
#define XTREE_XPATH_HPP_20110322__

#include "xtree/config.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <string>


namespace xtree {


    //! This class represents a generic XPath result. It may hold a number, a boolean, a string,
    //! or a node set, depending on the XPath expression.
    class XTREE_DECL xpath_result
    {

    public:

        //! Constructs an empty xpath_result.
        explicit xpath_result();

        //! Constructs an xpath_result to wrap an xmlXPathObject pointer.
        //! \param xpath  the XPath expression that generates this result.
        //! \param px     the xmlXPathObject pointer to wrap.
        explicit xpath_result(const std::string& xpath, xmlXPathObject* px);

        //! Destructor.
        virtual ~xpath_result();

        //! Frees the underlying xmlXPathObject.
        void free();

        //! Transfers the ownership of the underlying xmlXPathObject to the other xpath_result.
        //! \param other  the other xpath_result to take over the ownership of the xmlXPathObject.
        //! \throws xpath_error  if the transfer fails.
        void transfer(xpath_result& other);

        //! Returns the XPath expression that generates this result.
        const std::string& xpath() const
        {
            return xpath_;
        }

        //! Returns the underlying xmlXPathObject.
        const xmlXPathObject* raw() const
        {
            return raw_;
        }

    private:

        virtual void check_acceptable(const xpath_result& result);

    private:

        std::string     xpath_;  //!< The XPath expression.
        xmlXPathObject* raw_;    //!< The underlying xmlXPathObject.

    };


}  // namespace xtree


#endif  // XTREE_XPATH_HPP_20110322__

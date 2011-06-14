//
// Created by ZHENG Zhong on 2011-03-22.
//

#ifndef XTREE_XPATH_CONTEXT_HPP_20110322__
#define XTREE_XPATH_CONTEXT_HPP_20110322__

#include "xtree/config.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <string>


namespace xtree {


    class XTREE_DECL xpath_result;


    class XTREE_DECL xpath_context
    {

    public:

        //! Constructor. This function should NOT be called by client code.
        //! \param px_doc   the libxml2 document on which XPath expressions are evaluated.
        //! \param px_node  the libxml2 node from which XPath expressions are evaluated.
        explicit xpath_context(xmlDoc* px_doc, xmlNode* px_node);

        //! Destructor.
        ~xpath_context();

        //! Registers a namespace prefix and URI that will be used to evaluate XPath expressions.
        //! \param prefix  the namespace prefix to register.
        //! \param uri     the namespace URI to register.
        void register_ns(const std::string& prefix, const std::string& uri);

        //! Evaluates an XPath expression.
        //! \param xpath   the XPath expression to evaluate.
        //! \param result  output argument to hold XPath results.
        void eval(const std::string& xpath, xpath_result& result);

    private:

        //! Non-implemented copy constructor.
        xpath_context(const xpath_context&);

        //! Non-implemented copy assignment.
        xpath_context& operator=(const xpath_context&);

    private:

        xmlXPathContext* raw_;  //!< Raw pointer to the underlying xmlXPathContext object.

    };


}  // namespace xtree


#endif  // XTREE_LIBXML2_XPATH_CONTEXT_HPP_20110322__

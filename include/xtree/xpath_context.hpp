//
// Created by ZHENG Zhong on 2011-03-22.
//

#ifndef XTREE_XPATH_CONTEXT_HPP_20110322__
#define XTREE_XPATH_CONTEXT_HPP_20110322__

#include "xtree/config.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <string>


//! \cond DEV


namespace xtree {

    class XTREE_DECL xpath;
    class XTREE_DECL xpath_result;

}  // namespace xtree


////////////////////////////////////////////////////////////////////////////////////////////////////


namespace xtree {
namespace detail {


    //! This class wraps a libxml2 xmlXPathContext object. It should not be used by client code.
    class XTREE_DECL xpath_context
    {

    public:

        //! Constructor. This function should NOT be called by client code.
        //! \param px_doc   the libxml2 document on which XPath expressions are evaluated.
        //! \param px_node  the libxml2 node from which XPath expressions are evaluated.
        explicit xpath_context(xmlDoc* px_doc, xmlNode* px_node);

        //! Destructor.
        ~xpath_context();

        //! Evaluates an XPath expression.
        //! \param expr    the XPath expression to evaluate.
        //! \param result  output argument to hold XPath results.
        void eval(const xpath& expr, xpath_result& result);

    private:

        //! Non-implemented copy constructor.
        xpath_context(const xpath_context&);

        //! Non-implemented copy assignment.
        xpath_context& operator=(const xpath_context&);

    private:

        xmlXPathContext* raw_;  //!< Raw pointer to the underlying xmlXPathContext object.

    };


}  // namespace xtree::detail
}  // namespace xtree

//! \endcond


#endif  // XTREE_LIBXML2_XPATH_CONTEXT_HPP_20110322__


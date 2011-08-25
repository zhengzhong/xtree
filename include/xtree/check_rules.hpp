//
// Created by ZHENG Zhong on 2011-06-16.
//

#ifndef XTREE_CHECK_RULES_HPP_20110616__
#define XTREE_CHECK_RULES_HPP_20110616__

#include "xtree/config.hpp"

#include <string>


//! \cond DEV

namespace xtree {
namespace detail {


    //! Checks if the given value conforms to the lexical scope of NCName.
    //! \param value  the value to check.
    //! \throws bad_dom_operation  if the value is not a valid NCName.
    void check_nc_name(const std::string& value);


    //! Checks if the given value conforms to the lexical scope of Name.
    //! \param value  the value to check.
    //! \throws bad_dom_operation  if the value is not a valid Name.
    void check_name(const std::string& value);


    //! Checks if the given value conforms to the lexical scope of QName.
    //! \param value  the value to check.
    //! \throws bad_dom_operation  if the value is not a valid QName.
    void check_qname(const std::string& value);


}  // namespace xtree::detail
}  // namespace xtree

//! \endcond


#endif  // XTREE_CHECK_RULES_HPP_20110616__


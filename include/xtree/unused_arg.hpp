//
// Created by ZHENG Zhong on 2011-06-14.
//

#ifndef XTREE_UNUSED_ARG_HPP_20110614__
#define XTREE_UNUSED_ARG_HPP_20110614__

#include "xtree/config.hpp"


//! \cond DEV

namespace xtree {
namespace detail {


    //! This function does nothing. It is used to remove compiler warning about unused argument.
    template<typename T>
    inline void unused_arg(const T&)
    {
        // Do nothing.
    }


}  // namespace xtree::detail
}  // namespace xtree

//! \endcond


#endif  // XTREE_UNUSED_ARG_HPP_20110614__


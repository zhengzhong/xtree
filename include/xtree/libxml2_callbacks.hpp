//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_LIBXML2_CALLBACKS_HPP_20080717__
#define XTREE_LIBXML2_CALLBACKS_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/libxml2_fwd.hpp"


//! \cond DEV

namespace xtree {
namespace detail {


    //! Callback function invoked by libxml2 whenever an xmlNode has been constructed.
    //! \param px  the libxml2 node object constructed.
    void on_node_construct(xmlNode* px);


    //! Callback function invoked by libxml2 whenever an xmlNode is to be destructed.
    //! \param px  the libxml2 node to be destructed.
    void on_node_destruct(xmlNode* px);


} // namespace xtree::detail
} // namespace xtree

//! \endcond


#endif // XTREE_LIBXML2_CALLBACKS_HPP_20080717__





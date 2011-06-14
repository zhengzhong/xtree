//
// Created by ZHENG Zhong on 2009-11-26.
//

#ifndef XTREE_LIBXML2_GLOBALS_HPP_20091126__
#define XTREE_LIBXML2_GLOBALS_HPP_20091126__

#include "xtree/config.hpp"
#include "xtree/phoenix_singleton.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <libxml/tree.h>


//! \cond DEV

#ifdef XTREE_MSVC
#  pragma warning(push)
#  pragma warning(disable: 4511 4512)  // noncopyable warnings
#endif

namespace xtree {
namespace detail {


    //! This class manages libxml2's global resources. It is implemented as a phoenix singleton.
    //! This class is NOT exported.
    class libxml2_globals: public phoenix_singleton<libxml2_globals>
    {

        friend class phoenix_singleton<libxml2_globals>;

    public:

        //! Initializes the global resources of libxml2 as necessary.
        static void initialize();

    private:

        //! Constructor: initializes the global resources of libxml2.
        explicit libxml2_globals();

        //! Destructor: releases the global resources of libxml2.
        ~libxml2_globals();

    private:

        xmlRegisterNodeFunc   old_register_node_fn_;
        xmlDeregisterNodeFunc old_deregister_node_fn_;
        xmlRegisterNodeFunc   old_thr_def_register_node_fn_;
        xmlDeregisterNodeFunc old_thr_def_deregister_node_fn_;

    };


}  // namespace xtree::detail
}  // namespace xtree


#ifdef XTREE_MSVC
#  pragma warning(pop)  // noncopyable warnings
#endif

//! \endcond


#endif  // XTREE_LIBXML2_GLOBALS_HPP_20091126__




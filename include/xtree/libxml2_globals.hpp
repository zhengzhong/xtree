//
// Created by ZHENG Zhong on 2009-11-26.
//

#ifndef XTREE_LIBXML2_GLOBALS_HPP_20091126__
#define XTREE_LIBXML2_GLOBALS_HPP_20091126__

#include "xtree/config.hpp"
#include "xtree/phoenix_singleton.hpp"
#include "xtree/libxml2_fwd.hpp"


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

        //! Sets the cleanup parser flag to indicate whether to call xmlCleanupParser() at exit.
        //! WARNING: in a multi-threaded environment, calling xmlCleanupParser() at exit may crash
        //! the application if another thread is still using libxml2... So by default, this flag
        //! is set to false. Use this function with caution!
        //! \param flag  the new cleanup parser flag.
        static void set_cleanup_parser(bool flag);

        //! Returns the cleanup parser flag.
        //! \return the cleanup parser flag.
        static bool get_cleanup_parser();

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

        bool cleanup_parser_;  //!< indicates whether to cleanup parser at exit.

    };


}  // namespace xtree::detail
}  // namespace xtree


#ifdef XTREE_MSVC
#  pragma warning(pop)  // noncopyable warnings
#endif


//! \endcond


namespace xtree {


    //! \name Global Libxml2 Functions
    //! \{


    //! Initializes libxml2 library. In a multi-threaded environment, user should initialize
    //! libxml2 explicitly in the "main" thread by calling this function before using any of the
    //! other API. See http://xmlsoft.org/threads.html for more information.
    XTREE_DECL void initialize_libxml2();


    //! Sets the cleanup parser flag to indicate whether to call xmlCleanupParser() at exit.
    //! \param flag  the new cleanup parser flag.
    XTREE_DECL void set_libxml2_cleanup_parser(bool flag);


    //! Returns the cleanup parser flag.
    //! \return the cleanup parser flag.
    XTREE_DECL bool get_libxml2_cleanup_parser();


    //! \}


}  // namespace xtree


#endif  // XTREE_LIBXML2_GLOBALS_HPP_20091126__




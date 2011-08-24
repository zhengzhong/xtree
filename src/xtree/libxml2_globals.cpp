//
// Created by ZHENG Zhong on 2009-11-26.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/libxml2_globals.hpp"
#include "xtree/libxml2_callbacks.hpp"

#include <libxml/tree.h>


namespace xtree {
namespace detail {


    void libxml2_globals::initialize()
    {
        instance();
    }


    void libxml2_globals::set_cleanup_parser(bool flag)
    {
        instance().cleanup_parser_ = flag;
    }


    bool libxml2_globals::get_cleanup_parser()
    {
        return instance().cleanup_parser_;
    }


    libxml2_globals::libxml2_globals(): old_register_node_fn_(0)
                                      , old_deregister_node_fn_(0)
                                      , old_thr_def_register_node_fn_(0)
                                      , old_thr_def_deregister_node_fn_(0)
                                      , cleanup_parser_(false)
    {
        // Initialize libxml2 resources.
        LIBXML_TEST_VERSION;
        xmlInitParser();
        // Register libxml2 callback functions, and save the old ones.
        old_register_node_fn_ = xmlRegisterNodeDefault(&on_node_construct);
        old_deregister_node_fn_ = xmlDeregisterNodeDefault(&on_node_destruct);
        old_thr_def_register_node_fn_ = xmlThrDefRegisterNodeDefault(&on_node_construct);
        old_thr_def_deregister_node_fn_ = xmlThrDefDeregisterNodeDefault(&on_node_destruct);
        // Libxml2 global initialization.
        xmlSubstituteEntitiesDefault(1);
        xmlLoadExtDtdDefaultValue = 1;
    }


    libxml2_globals::~libxml2_globals()
    {
        // Restore the old libxml2 callback functions.
        xmlRegisterNodeDefault(
            static_cast<xmlRegisterNodeFunc>(old_register_node_fn_)
        );
        xmlDeregisterNodeDefault(
            static_cast<xmlDeregisterNodeFunc>(old_deregister_node_fn_)
        );
        xmlThrDefRegisterNodeDefault(
            static_cast<xmlRegisterNodeFunc>(old_thr_def_register_node_fn_)
        );
        xmlThrDefDeregisterNodeDefault(
            static_cast<xmlDeregisterNodeFunc>(old_thr_def_deregister_node_fn_)
        );
        //
        // According to the libxml2 document on xmlCleanupParser():
        //
        // This function... does not clean up parser state, it cleans up memory allocated by the
        // library itself. It is a cleanup function for the XML library. It tries to reclaim all
        // related global memory allocated for the library processing. It doesn't deallocate any
        // document related memory. One should call xmlCleanupParser() only when the process has
        // finished using the library and all XML/HTML documents built with it.
        // ...
        // WARNING: if your application is multi-threaded or has plugin support, calling this may
        // crash the application if another thread or a plugin is still using libxml2... In case of
        // doubt abstain from calling this function or do it just before calling exit() to avoid
        // leak reports from Valgrind!
        //
        if (cleanup_parser_)
        {
            xmlCleanupParser();
        }
    }


}  // namespace xtree::detail
}  // namespace xtree


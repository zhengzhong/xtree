//
// Created by ZHENG Zhong on 2008-07-21.
//

#ifndef XTREE_DOM_BASE_HPP_20080721__
#define XTREE_DOM_BASE_HPP_20080721__

#include "xtree/config.hpp"

namespace xtree {


    //! The base class for all libxml2-related classes. The constructor of this class ensures that
    //! the globals singleton is created by requesting a reference to the singleton instance. This
    //! class should be derived (private) by classes who require that all libxml2 resources be
    //! initialized.
    class XTREE_DECL dom_base
    {

    protected:

        //! Protected default constructor for subclasses.
        explicit dom_base();

    };  // class dom_base


}  // namespace xtree

#endif  // XTREE_DOM_BASE_HPP_20080721__



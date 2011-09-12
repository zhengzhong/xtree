//
// Created by ZHENG Zhong on 2008-07-21.
//

#ifndef XTREE_XML_BASE_HPP_20080721__
#define XTREE_XML_BASE_HPP_20080721__

#include "xtree/config.hpp"

namespace xtree {

    //! \cond DEV

    //! The base class for all libxml2-related classes. The constructor of this class ensures that
    //! the globals singleton is created by requesting a reference to the singleton instance. This
    //! class should be derived (private) by classes who require that all libxml2 resources be
    //! initialized.
    class XTREE_DECL xml_base
    {

    protected:

        //! Protected default constructor for subclasses.
        explicit xml_base();

    };

    //! \endcond

}  // namespace xtree


#endif  // XTREE_XML_BASE_HPP_20080721__



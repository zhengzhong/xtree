//
// Created by ZHENG Zhong on 2009-01-26.
//

#ifndef XTREE_SAX_FEATURES_HPP_20090126__
#define XTREE_SAX_FEATURES_HPP_20090126__

#include "xtree/config.hpp"

#include <string>


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name SAX2 Standard Features
    //! \{


    // Refer to: http://www.saxproject.org/apidoc/org/xml/sax/package-summary.html

    //! Whether this parser processes external general entities; always true if validating.
    static const std::string sax_external_general_entities = "external-general-entities";

    //! Whether this parser processes external parameter entities; always true if validating.
    static const std::string sax_external_parameter_entities = "external-parameter-entities";

    //! Whether namespace URIs and unprefixed local names for element and attribute names will 
    //! be available. Defaults to "true".
    static const std::string sax_namespaces = "namespaces";

    //! Whether XML qualified names (with prefixes) and attributes (including xmlns* attributes)
    //! will be available.
    static const std::string sax_namespace_prefixes = "namespace-prefixes";


    //! \}


}  // namespace xtree


#endif  // XTREE_SAX_FEATURES_HPP_20090126__


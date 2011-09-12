//
// Created by ZHENG Zhong on 2008-07-03.
//

#ifndef XTREE_TYPES_HPP_20080703__
#define XTREE_TYPES_HPP_20080703__

#include "xtree/config.hpp"

#include <string>


namespace xtree {


    //! The size type used to count nodes.
    typedef unsigned int size_type;


    //! The XML node type enumerations.
    enum node_t
    {
        unknown_node = 0,
        document_node,
        attribute_node,
        element_node,
        text_node,
        cdata_node,
        comment_node,
        instruction_node,
    };


    //! \cond DEV

    //! Converts a node type enumeration to string representation.
    inline std::string to_string(node_t type)
    {
        switch (type)
        {
        case element_node:
            return "element";
        case attribute_node:
            return "attribute";
        case text_node:
            return "text";
        case cdata_node:
            return "cdata";
        case comment_node:
            return "comment";
        case instruction_node:
            return "instruction";
        default:
            return "unknown";
        }
    }

    //! \endcond


    //! The XPath result type enumerations.
    enum xpath_result_t
    {
        undefined_result = 0,
        node_set_result,
        boolean_result,
        number_result,
        string_result,
    };


} // namespace xtree


#endif  // XTREE_TYPES_HPP_20080703__




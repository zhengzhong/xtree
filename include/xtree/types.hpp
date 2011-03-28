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
        attribute_node,
        element_node,
        text_node,
        cdata_node,
        comment_node,
        xml_pi_node,
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////


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
        case xml_pi_node:
            return "xml_pi";
        default:
            return "unknown";
        }
    }


} // namespace xtree


#endif  // XTREE_TYPES_HPP_20080703__




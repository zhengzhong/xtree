//
// Created by ZHENG Zhong on 2008-07-22.
//

#ifndef XTREE_UTILITY_HPP_20080722__
#define XTREE_UTILITY_HPP_20080722__

#include "xtree/config.hpp"
#include "xtree/basic_node_ptr.hpp"

#include <string>


namespace xtree {


    template<class Node>
    inline basic_node_ptr<Node> make_clone(const Node& n, bool recursive)
    {
        basic_node_ptr<node> cloned = n.clone(recursive);
        return dynamic_node_cast<Node>(cloned);
    }


    //! Checks if an XML element is ancestor of another XML node.
    //! \param ancestor  the element node to check.
    //! \param child     the child node to check.
    //! \return true if the element node is ancestor of the child node.
    //! \todo TODO: implement me!
    template<class Element, class Node>
    inline bool is_ancestor(const Element& ancestor, const Node& child);


}  // namespace xtree


#endif  // XTREE_UTILITY_HPP_20080722__


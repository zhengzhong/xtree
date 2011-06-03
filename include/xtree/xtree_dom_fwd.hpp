//
// Created by ZHENG Zhong on 2009-05-14.
//

#ifndef XTREE_XTREE_DOM_FWD_HPP_20090514__
#define XTREE_XTREE_DOM_FWD_HPP_20090514__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/types.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/basic_node_iterator.hpp"


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // class forward declarations
    //


    class XTREE_DECL dom_parser;

    class XTREE_DECL document;
    class XTREE_DECL node;
    class XTREE_DECL attribute;
    class XTREE_DECL child_node;
    class XTREE_DECL element;
    class XTREE_DECL text;
    class XTREE_DECL comment;
    class XTREE_DECL xml_pi;

    class XTREE_DECL child_node_list;
    class XTREE_DECL attribute_map;

    class XTREE_DECL xpath_context;
    class XTREE_DECL xpath_result;
    class XTREE_DECL node_set;

    class XTREE_DECL validity;
    class XTREE_DECL schema;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // pointer typedefs
    //


    typedef basic_node_ptr<node>             node_ptr;
    typedef basic_node_ptr<attribute>        attribute_ptr;  
    typedef basic_node_ptr<element>          element_ptr;
    typedef basic_node_ptr<text>             text_ptr;
    typedef basic_node_ptr<comment>          comment_ptr;
    typedef basic_node_ptr<xml_pi>           xml_pi_ptr;

    typedef basic_node_ptr<const node>       const_node_ptr;
    typedef basic_node_ptr<const attribute>  const_attribute_ptr;
    typedef basic_node_ptr<const element>    const_element_ptr;
    typedef basic_node_ptr<const text>       const_text_ptr;
    typedef basic_node_ptr<const comment>    const_comment_ptr;
    typedef basic_node_ptr<const xml_pi>     const_xml_pi_ptr;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // iterator typedefs
    //


    typedef basic_node_iterator<attribute>         attribute_iterator;
    typedef basic_node_iterator<const attribute>   const_attribute_iterator;

    typedef basic_node_iterator<child_node>        child_iterator;
    typedef basic_node_iterator<const child_node>  const_child_iterator;


}  // namespace xtree


#endif  // XTREE_XTREE_DOM_FWD_HPP_20090514__


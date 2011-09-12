//
// Created by ZHENG Zhong on 2008-07-26.
//

#ifndef XTREE_CHILD_NODE_HPP_20080726__
#define XTREE_CHILD_NODE_HPP_20080726__

#include "xtree/config.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/node.hpp"


namespace xtree {


    //! This abstract base class represents an XML node that can be a child node under an element.
    class XTREE_DECL child_node: public node
    {

    public:

        //! \cond DEV

        ///////////////////////////////////////////////////////////////////////////////////////////
        //! \name Static Cast Functions
        //! \{

        static child_node* cast(xmlNode* px)
        {
            return const_cast<child_node*>(cast_(px));
        }

        static const child_node* cast(const xmlNode* px)
        {
            return cast_(px);
        }

        //! \}

        //! \endcond

    public:

        //! Pure virtual destructor.
        virtual ~child_node() = 0;

        //! Returns the previous sibling.
        //! \return the previous sibling.
        basic_node_ptr<child_node> prev_sibling();

        //! Returns the next sibling.
        //! \return the next sibling.
        basic_node_ptr<child_node> next_sibling();

        //! Const version of prev_sibling().
        basic_node_ptr<const child_node> prev_sibling() const;

        //! Const version of next_sibling().
        basic_node_ptr<const child_node> next_sibling() const;

    protected:

        //! Protected constructor.
        explicit child_node(xmlNode* px);

    private:

        static const child_node* cast_(const xmlNode* px);

    private:

        const child_node* prev_sibling_() const;

        const child_node* next_sibling_() const;

    };


}  // namespace xtree


#endif  // XTREE_CHILD_NODE_HPP_20080726__




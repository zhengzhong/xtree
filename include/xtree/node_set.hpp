//
// Created by ZHENG Zhong on 2008-07-26.
//

#ifndef XTREE_NODE_SET_HPP_20080726__
#define XTREE_NODE_SET_HPP_20080726__

#include "xtree/config.hpp"
#include "xtree/xpath_result.hpp"
#include "xtree/node.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/libxml2_fwd.hpp"


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // const_node_set_iterator
    //


    class XTREE_DECL const_node_set_iterator
    {

    public:

        typedef const node   value_type;
        typedef const node&  reference;
        typedef const node*  pointer;

    public:

        explicit const_node_set_iterator(): nodes_(0), index_(0)
        {
            // Do nothing.
        }

        explicit const_node_set_iterator(xmlNodeSet* nodes, int index)
        : nodes_(nodes), index_(index)
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.
        // Use auto-generated destructor.

        bool operator==(const const_node_set_iterator& rhs) const
        {
            return ( nodes_ == rhs.nodes_ && index_ == rhs.index_ );
        }

        bool operator!=(const const_node_set_iterator& rhs) const
        {
            return !operator==(rhs);
        }

        const node& operator*() const
        {
            return (*current());
        }

        const node* operator->() const
        {
            return current();
        }

        const_node_set_iterator& operator++()
        {
            increment();
            return *this;
        }

        const_node_set_iterator operator++(int)
        {
            const_node_set_iterator tmp(*this);
            increment();
            return tmp;
        }

        const_node_set_iterator& operator--()
        {
            decrement();
            return *this;
        }

        const_node_set_iterator operator--(int)
        {
            const_node_set_iterator tmp(*this);
            decrement();
            return tmp;
        }

        basic_node_ptr<const node> ptr() const
        {
            return basic_node_ptr<const node>(current());
        }

    protected:

        void increment();

        void decrement();

        const node* current() const;

    private:

        xmlNodeSet* nodes_;  //!< Raw pointer to the xmlNodeSet object.
        int         index_;  //!< The current node index of this iterator.

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // node_set_iterator
    //


    class XTREE_DECL node_set_iterator: public const_node_set_iterator
    {

        typedef const_node_set_iterator  base_type;

    public:

        typedef node   value_type;
        typedef node&  reference;
        typedef node*  pointer;

    public:

        explicit node_set_iterator()
        {
            // Do nothing.
        }

        explicit node_set_iterator(xmlNodeSet* nodes, int index): base_type(nodes, index)
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.
        // Use auto-generated destructor.

        node& operator*() const
        {
            return const_cast<node&>(base_type::operator*());
        }

        node* operator->() const
        {
            return const_cast<node*>(base_type::operator->());
        }

        node_set_iterator& operator++()
        {
            increment();
            return *this;
        }

        node_set_iterator operator++(int)
        {
            node_set_iterator tmp(*this);
            increment();
            return tmp;
        }

        node_set_iterator& operator--()
        {
            decrement();
            return *this;
        }

        node_set_iterator operator--(int)
        {
            node_set_iterator tmp(*this);
            decrement();
            return tmp;
        }

        basic_node_ptr<node> ptr() const
        {
            return const_node_cast<node>(base_type::ptr());
        }

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // node_set
    //


    //! This class represents a node set evaluated from an XPath expression.
    class XTREE_DECL node_set: public xpath_result
    {

    public:

        typedef node_set_iterator        iterator;
        typedef const_node_set_iterator  const_iterator;

    public:

        //! Default constructor. Constructs an empty node set.
        explicit node_set();

        //! Checks if this node set is empty.
        //! \return true if this node set is empty, false otherwise.
        bool empty() const;

        //! Returns the size of this node set.
        //! \return the size of this node set.
        size_type size() const;

        //! Returns an iterator to the first node of this node set.
        //! \return an iterator to the first node of this node set.
        iterator begin();

        //! Returns an iterator just past the last node of this node set.
        //! \return an iterator just past the last node of this node set.
        iterator end();

        //! Const version of begin().
        const_iterator begin() const;

        //! Const version of end().
        const_iterator end() const;

    private:

        virtual void check_acceptable(const xpath_result& result);

    private:

        //! Non-implemented copy constructor.
        node_set(const node_set&);

        //! Non-implemented copy assignment.
        node_set& operator=(const node_set&);

    };


}  // namespace xtree


#endif  // XTREE_NODE_SET_HPP_20080726__


//
// Created by ZHENG Zhong on 2008-07-26.
//

#ifndef XTREE_NODE_SET_HPP_20080726__
#define XTREE_NODE_SET_HPP_20080726__

#include "xtree/config.hpp"
#include "xtree/xpath_result.hpp"
#include "xtree/types.hpp"
#include "xtree/node.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <iterator>


namespace xtree {


    class XTREE_DECL element;


    ////////////////////////////////////////////////////////////////////////////////////////////////


    //! This class template represents an iterator for node_set.
    template<class T, class Mover>
    class basic_node_set_iterator: public std::iterator<std::bidirectional_iterator_tag, T>
    {

        template<class T_, class Mover_>
        friend class basic_node_set_iterator;

        typedef basic_node_set_iterator<T, Mover>  this_type;
        typedef std::iterator<std::bidirectional_iterator_tag, T>  base_type;
        typedef Mover  mover_type;

    public:

        typedef typename base_type::value_type  value_type;
        typedef typename base_type::pointer     pointer;
        typedef typename base_type::reference   reference;

    public:

        //! Default constructor.
        explicit basic_node_set_iterator(): mover_(0, 0)
        {
            // Do nothing.
        }

        explicit basic_node_set_iterator(xmlNodeSet* nodes, int index): mover_(nodes, index)
        {
            // Do nothing.
        }

        //! Generic copy constructor.
        template<class T_, class Mover_>
        basic_node_set_iterator(const basic_node_set_iterator<T_, Mover_>& rhs): mover_(rhs.mover_)
        {
            // Do nothing.
        }

        //! Generic copy assignment.
        template<class T_, class Mover_>
        this_type& operator=(const basic_node_set_iterator<T_, Mover_>& rhs)
        {
            mover_ = rhs.mover_;
            return *this;
        }

        //! Generic comparison operator.
        template<class T_, class Mover_>
        bool operator==(const basic_node_set_iterator<T_, Mover_>& rhs) const
        {
            return ( mover_ == rhs.mover_ );
        }

        //! Generic comparison operator.
        template<class T_, class Mover_>
        bool operator!=(const basic_node_set_iterator<T_, Mover_>& rhs) const
        {
            return !operator==(rhs);
        }

        reference operator*() const
        {
            return (*mover_.current());
        }

        pointer operator->() const
        {
            return mover_.current();
        }

        this_type& operator++()
        {
            mover_.increment();
            return *this;
        }

        this_type operator++(int)
        {
            this_type tmp(*this);
            mover_.increment();
            return tmp;
        }

        this_type& operator--()
        {
            mover_.decrement();
            return *this;
        }

        this_type operator--(int)
        {
            this_type tmp(*this);
            mover_.decrement();
            return tmp;
        }

        basic_node_ptr<T> ptr() const
        {
            return basic_node_ptr<T>(mover_.current());
        }

    private:

        mover_type mover_;

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////


    //! \cond DEV


    //! This class is used with basic_node_set_iterator to iterate over all the nodes in a given
    //! node_set.
    class XTREE_DECL node_set_mover
    {

    public:

        explicit node_set_mover(xmlNodeSet* nodes, int index);

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.
        // Use auto-generated destructor.

        bool operator==(const node_set_mover& rhs) const
        {
            return (nodes_ == rhs.nodes_ && index_ == rhs.index_);
        }

        void increment();

        void decrement();

        node* current() const;

    private:

        xmlNodeSet* nodes_;  //!< Raw pointer to the xmlNodeSet object.
        int         index_;  //!< The current node index of this iterator.

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////


    //! This class is used with basic_node_set_iterator to iterate over all the elements (by
    //! ignoring all the non-elements) in a given node_set.
    class XTREE_DECL node_set_element_mover
    {

    public:

        explicit node_set_element_mover(xmlNodeSet* nodes, int index);

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.
        // Use auto-generated destructor.

        bool operator==(const node_set_element_mover& rhs) const
        {
            return (nodes_ == rhs.nodes_ && index_ == rhs.index_);
        }

        void increment();

        void decrement();

        element* current() const;

    private:

        xmlNodeSet* nodes_;  //!< Raw pointer to the xmlNodeSet object.
        int         index_;  //!< The current node index of this iterator.

    };


    //! \endcond


    ////////////////////////////////////////////////////////////////////////////////////////////////


    //! This class represents a node set evaluated from an XPath expression.
    class XTREE_DECL node_set: public xpath_result
    {

    public:

        typedef basic_node_set_iterator<
            node, node_set_mover
        >  iterator;

        typedef basic_node_set_iterator<
            const node, node_set_mover
        >  const_iterator;

        typedef basic_node_set_iterator<
            element, node_set_element_mover
        >  element_iterator;

        typedef basic_node_set_iterator<
            const element, node_set_element_mover
        >  const_element_iterator;

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

        //! Returns an element iterator to the first element of this node set.
        //! \return an element iterator to the first element of this node set.
        element_iterator begin_element();

        //! Returns an element iterator just past the last element of this node set.
        //! \return an element iterator just past the last element of this node set.
        element_iterator end_element();

        //! Const version of begin_element().
        const_element_iterator begin_element() const;

        //! Const version of end_element().
        const_element_iterator end_element() const;

    private:

        virtual bool is_acceptable_(xpath_result_t type) const;

    };


}  // namespace xtree


#endif  // XTREE_NODE_SET_HPP_20080726__


//
// Created by ZHENG Zhong on 2011-03-18.
//

#ifndef XTREE_ELEMENT_ITERATOR_HPP_20110318__
#define XTREE_ELEMENT_ITERATOR_HPP_20110318__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/element.hpp"


namespace xtree {


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // basic_const_element_iterator
    //


    template<class M>
    class basic_const_element_iterator
    {

        typedef basic_const_element_iterator<M> this_type;

    public:

        typedef const element   value_type;
        typedef const element&  reference;
        typedef const element*  pointer;

    public:

        explicit basic_const_element_iterator(): mover_()
        {
            // Do nothing.
        }

        explicit basic_const_element_iterator(basic_node_ptr<const element> parent): mover_(parent)
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.
        // Use auto-generated destructor.

        bool operator==(const this_type& rhs) const
        {
            return (mover_.get() == rhs.mover_.get());
        }

        bool operator!=(const this_type& rhs) const
        {
            return (mover_.get() != rhs.mover_.get());
        }

        const element& operator*() const
        {
            return (*mover_.get());
        }

        const element* operator->() const
        {
            return (mover_.get().operator->());
        }

        //! Prefix-increment operator. If the iterator exceeds the end node, it will be set to
        //! empty (which equals to a default-constructed iterator object).
        this_type& operator++()
        {
            mover_.increment();
            return *this;
        }

        this_type operator++(int)
        {
            this_type tmp = *this;
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
            this_type tmp = *this;
            mover_.decrement();
            return tmp;
        }

        basic_node_ptr<const element> ptr() const
        {
            return mover_.get();
        }

    protected:

        M mover_;

    };


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // basic_element_iterator
    //


    template<class M>
    class basic_element_iterator: public basic_const_element_iterator<M>
    {

        typedef basic_element_iterator<M>        this_type;
        typedef basic_const_element_iterator<M>  base_type;

    public:

        typedef element   value_type;
        typedef element&  reference;
        typedef element*  pointer;

    public:

        explicit basic_element_iterator()
        {
            // Do nothing.
        }

        explicit basic_element_iterator(basic_node_ptr<element> parent): base_type(parent)
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.
        // Use auto-generated destructor.

        element& operator*() const
        {
            return const_cast<element&>(base_type::operator*());
        }

        element* operator->() const
        {
            return const_cast<element*>(base_type::operator->());
        }

        this_type& operator++()
        {
            mover_.increment();
            return *this;
        }

        this_type operator++(int)
        {
            this_type tmp = *this;
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
            this_type tmp = *this;
            mover_.decrement();
            return tmp;
        }

        basic_node_ptr<element> ptr() const
        {
            return const_node_cast<element>(base_type::ptr());
        }

    };


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // subelement_mover
    //


    class subelement_mover
    {

    public:

        explicit subelement_mover(): current_()
        {
            // Do nothing.
        }

        explicit subelement_mover(basic_node_ptr<const element> parent): current_()
        {
            if (parent != 0)
            {
                current_ = parent->find_first_elem();
            }
        }

        const basic_node_ptr<const element>& get() const
        {
            return current_;
        }

        void increment()
        {
            // Ensure the current position is not end.
            if (current_ == 0)
            {
                throw bad_dom_operation("increment() called on end position");
            }
            current_ = current_->get_next_sibling_elem();
        }

        void decrement()
        {
            // Ensure the current position is not end.
            if (current_ == 0)
            {
                throw bad_dom_operation("decrement() called on end position");
            }
            // Ensure the current position is not begin.
            basic_node_ptr<const element> prev = current_->get_prev_sibling_elem();
            if (prev == 0)
            {
                throw bad_dom_operation("decrement() called on begin position");
            }
            current_ = prev;
        }

    private:

        basic_node_ptr<const element> current_;

    };


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // subelement_mover
    //


    class df_element_mover
    {

    public:

        explicit df_element_mover(): parent_(), current_()
        {
            // Do nothing.
        }

        explicit df_element_mover(basic_node_ptr<const element> parent)
        : parent_(parent), current_(parent)
        {
            // Do nothing.
        }

        const basic_node_ptr<const element>& get() const
        {
            return current_;
        }

        void increment()
        {
            // Ensure the current position is not end.
            if (current_ == 0)
            {
                throw bad_dom_operation("increment() called on end position");
            }
            // Try to move deeper.
            basic_node_ptr<const element> next = current_->find_first_elem();
            if (next != 0)
            {
                current_ = next;
                return;
            }
            // Try to move to next sibling element.
            next = current_->get_next_sibling_elem();
            if (next != 0)
            {
                current_ = next;
                return;
            }
            // Try to move to next sibling element of ancestors.
            while (current_ != parent_ && current_->parent() != parent_)
            {
                current_ = current_->parent();
                next = current_->get_next_sibling_elem();
                if (next != 0)
                {
                    current_ = next;
                    return;
                }
            }
            // Move to end.
            current_ = basic_node_ptr<const element>();
        }

        void decrement()
        {
            // Ensure the current position is neither begin nor end.
            if (current_ == 0)
            {
                throw bad_dom_operation("decrement() called on end position");
            }
            if (current_ == parent_)
            {
                throw bad_dom_operation("decrement() called on begin position");
            }
            // Try to move to last subelement of previous sibling element.
            basic_node_ptr<const element> prev = current_->get_prev_sibling_elem();
            if (prev != 0)
            {
                basic_node_ptr<const element> last_elem = prev;
                while (last_elem->find_last_elem() != 0)
                {
                    last_elem = last_elem->find_last_elem();
                }
                current_ = last_elem;
                return;
            }
            // Try to move up.
            current_ = current_->parent();
        }

    private:

        basic_node_ptr<const element> parent_;
        basic_node_ptr<const element> current_;

    };


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // element iterator typedefs
    //


    typedef basic_const_element_iterator<subelement_mover>  const_subelement_iterator;
    typedef basic_element_iterator<subelement_mover>        subelement_iterator;

    typedef basic_const_element_iterator<df_element_mover>  const_df_element_iterator;
    typedef basic_element_iterator<df_element_mover>        df_element_iterator;


} // namespace xtree


#endif // XTREE_ELEMENT_ITERATOR_HPP_20110318__


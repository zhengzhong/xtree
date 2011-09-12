//
// Created by ZHENG Zhong on 2011-03-18.
//

#ifndef XTREE_ELEMENT_ITERATOR_HPP_20110318__
#define XTREE_ELEMENT_ITERATOR_HPP_20110318__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/element.hpp"

#include <iterator>


namespace xtree {


    //! This class template represents an iterator to XML elements.
    template<class E, class Mover>
    class basic_element_iterator: public std::iterator<std::bidirectional_iterator_tag, E>
    {

        template<class E_, class Mover_>
        friend class basic_element_iterator;

        typedef basic_element_iterator<E, Mover>  this_type;
        typedef std::iterator<std::bidirectional_iterator_tag, E>  base_type;
        typedef Mover  mover_type;

    public:

        typedef typename base_type::value_type  value_type;
        typedef typename base_type::pointer     pointer;
        typedef typename base_type::reference   reference;

    public:

        explicit basic_element_iterator(): mover_()
        {
            // Do nothing.
        }

        explicit basic_element_iterator(basic_node_ptr<E> parent): mover_(parent)
        {
            // Do nothing.
        }

        template<class E_, class Mover_>
        basic_element_iterator(const basic_element_iterator<E_, Mover_>& rhs)
        : mover_(rhs.mover_)
        {
            // Do nothing.
        }

        template<class E_, class Mover_>
        this_type& operator=(const basic_element_iterator<E_, Mover_>& rhs)
        {
            mover_ = rhs.mover_;
            return *this;
        }

        template<class E_, class Mover_>
        bool operator==(const basic_element_iterator<E_, Mover_>& rhs) const
        {
            return mover_ == rhs.mover_;
        }

        template<class E_, class Mover_>
        bool operator!=(const basic_element_iterator<E_, Mover_>& rhs) const
        {
            return !operator==(rhs);
        }

        reference operator*() const
        {
            return (*mover_.get());
        }

        pointer operator->() const
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

        basic_node_ptr<E> ptr() const
        {
            return mover_.get();
        }

    protected:

        mover_type mover_;

    };


    ///////////////////////////////////////////////////////////////////////////////////////////////


    //! \cond DEV


    //! Given a parent element, this class is used with basic_element_iterator to iterate over its
    //! direct child elements.
    template<class E>
    class subelement_mover
    {

        template<class E_>
        friend class subelement_mover;

        typedef subelement_mover<E>  this_type;

    public:

        explicit subelement_mover(): current_()
        {
            // Do nothing.
        }

        explicit subelement_mover(basic_node_ptr<E> parent): current_()
        {
            if (parent != 0)
            {
                current_ = parent->find_first_elem();
            }
        }

        template<class E_>
        subelement_mover(const subelement_mover<E_>& rhs): current_(rhs.current_)
        {
            // Do nothing.
        }

        template<class E_>
        this_type& operator=(const subelement_mover<E_>& rhs)
        {
            current_ = rhs.current_;
            return *this;
        }

        template<class E_>
        bool operator==(const subelement_mover<E_>& rhs) const
        {
            return current_ == rhs.current_;
        }

        template<class E_>
        bool operator!=(const subelement_mover<E_>& rhs) const
        {
            return !operator==(rhs);
        }

        const basic_node_ptr<E>& get() const
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
            basic_node_ptr<E> prev = current_->get_prev_sibling_elem();
            if (prev == 0)
            {
                throw bad_dom_operation("decrement() called on begin position");
            }
            current_ = prev;
        }

    private:

        basic_node_ptr<E> current_;

    };


    ///////////////////////////////////////////////////////////////////////////////////////////////


    //! Given a parent element, this class is used with basic_element_iterator to iterate over all
    //! its descendant elements in a depth-first order.
    template<class E>
    class df_element_mover
    {

        template<class E_>
        friend class df_element_mover;

        typedef df_element_mover<E>  this_type;

    public:

        explicit df_element_mover(): parent_(), current_()
        {
            // Do nothing.
        }

        explicit df_element_mover(basic_node_ptr<E> parent)
        : parent_(parent), current_(parent)
        {
            // Do nothing.
        }

        template<class E_>
        df_element_mover(const df_element_mover<E_>& rhs)
        : parent_(rhs.parent_), current_(rhs.current_)
        {
            // Do nothing.
        }

        template<class E_>
        this_type& operator=(const df_element_mover<E_>& rhs)
        {
            parent_ = rhs.parent_;
            current_ = rhs.current_;
            return *this;
        }

        template<class E_>
        bool operator==(const df_element_mover<E_>& rhs) const
        {
            if (current_ == 0 && rhs.current_ == 0)
            {
                return true;  // end iterators (current_ == 0) are considered to be equal.
            }
            else
            {
                return parent_ == rhs.parent_ && current_ == rhs.current_;
            }
        }

        template<class E_>
        bool operator!=(const df_element_mover<E_>& rhs) const
        {
            return !operator==(rhs);
        }

        const basic_node_ptr<E>& get() const
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
            basic_node_ptr<E> next = current_->find_first_elem();
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
            current_ = basic_node_ptr<E>();
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
            basic_node_ptr<E> prev = current_->get_prev_sibling_elem();
            if (prev != 0)
            {
                basic_node_ptr<E> last_elem = prev;
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

        basic_node_ptr<E> parent_;
        basic_node_ptr<E> current_;

    };


    //! \endcond


    ///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Element Iterators
    //! \{


    typedef basic_element_iterator<
        const element,
        subelement_mover<const element>
    > const_subelement_iterator;

    typedef basic_element_iterator<
        element,
        subelement_mover<element>
    > subelement_iterator;


    typedef basic_element_iterator<
        const element,
        df_element_mover<const element>
    > const_df_element_iterator;

    typedef basic_element_iterator<
        element,
        df_element_mover<element>
    > df_element_iterator;


    //! \}


} // namespace xtree


#endif // XTREE_ELEMENT_ITERATOR_HPP_20110318__


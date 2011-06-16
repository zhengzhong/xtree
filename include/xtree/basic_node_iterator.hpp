//
// Created by ZHENG Zhong on 2008-07-21.
//

#ifndef XTREE_BASIC_NODE_ITERATOR_HPP_20080721__
#define XTREE_BASIC_NODE_ITERATOR_HPP_20080721__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/basic_node_ptr.hpp"

#include <iterator>


namespace xtree {


    //! This class template represents an iterator to an XML node object. Increasing the iterator
    //! will move to the next sibling node of the current node; decreasing the iterator will move
    //! to the previous sibling node of the current node. It is required the template parameter T
    //! provide the following member functions:
    //!
    //! \code
    //! // Required by basic_node_iterator<T>:
    //! basic_node_ptr<T> T::next_sibling();
    //! basic_node_ptr<T> T::prev_sibling();
    //!
    //! // Required by basic_node_iterator<const T>:
    //! basic_node_ptr<const T> T::next_sibling() const;
    //! basic_node_ptr<const T> T::prev_sibling() const;
    //! \endcode
    //!
    //! This class template is implemented in term of basic_node_ptr class template.
    //!
    //! \todo TODO: decreasing an end iterator may result in undefined behavior, fix this.
    template<class T>
    class basic_node_iterator: public std::iterator<std::bidirectional_iterator_tag, T>
    {

        template<class U>
        friend class basic_node_iterator;

        typedef basic_node_iterator<T>  this_type;

        typedef std::iterator<std::bidirectional_iterator_tag, T>  base_type;

    public:

        typedef typename base_type::value_type  value_type;
        typedef typename base_type::pointer     pointer;
        typedef typename base_type::reference   reference;

    public:

        explicit basic_node_iterator(): current_()
        {
            // Do nothing.
        }

        explicit basic_node_iterator(value_type* current): current_(current)
        {
            // Do nothing.
        }

        //! Constructs a basic_node_iterator from another basic_node_iterator. This non-explicit
        //! constructor requires that type U* is implicitly convertible to type T* (e.g. U is a
        //! subclass of T).
        template<class U>
        basic_node_iterator(const basic_node_iterator<U>& rhs): current_(rhs.current_)
        {
            // Do nothing.
        }

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.
        // Use auto-generated destructor.

        bool operator==(const this_type& rhs) const
        {
            return (current_ == rhs.current_);
        }

        bool operator!=(const this_type& rhs) const
        {
            return (current_ != rhs.current_);
        }

        reference operator*() const
        {
            return (*current_);
        }

        pointer operator->() const
        {
            return (current_.operator->());
        }

        //! Prefix-increment operator. If the iterator exceeds the end node, it will be set to
        //! empty (which equals to a default-constructed iterator object).
        this_type& operator++()
        {
            increment();
            return *this;
        }

        this_type operator++(int)
        {
            this_type tmp = *this;
            increment();
            return tmp;
        }

        this_type& operator--()
        {
            decrement();
            return *this;
        }

        this_type operator--(int)
        {
            this_type tmp = *this;
            decrement();
            return tmp;
        }

        basic_node_ptr<T> ptr() const
        {
            return current_;
        }

    private:

        void increment()
        {
            if (current_ == 0)
            {
                throw bad_dom_operation("increment() called at end position");
            }
            current_ = current_->next_sibling();
        }

        void decrement()
        {
            if (current_ == 0)
            {
                throw bad_dom_operation("decrement() called at end position");
            }
            current_ = current_->prev_sibling();
        }

    private:

        basic_node_ptr<T> current_;

    };


} // namespace xtree


#endif // XTREE_BASIC_NODE_ITERATOR_HPP_20080721__





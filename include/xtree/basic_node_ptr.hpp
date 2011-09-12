//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_BASIC_NODE_PTR_HPP_20080717__
#define XTREE_BASIC_NODE_PTR_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"


namespace xtree {


    //! This class template wraps a raw pointer to XML node object and provides a pointer-like
    //! interface. By using this class template, users are disallowed to delete the raw pointer.
    template<class T>
    class basic_node_ptr
    {

        template<class U> friend class basic_node_ptr;

    public:

        ///////////////////////////////////////////////////////////////////////////////////////////
        //! \name Constructors
        //! \{

        //! Constructs a pointer wrapper to null.
        explicit basic_node_ptr(): ptr_(0)
        {
            // Do nothing.
        }

        //! Constructs a pointer wrapper to the specified raw pointer.
        //! \param raw  the raw pointer to wrap.
        explicit basic_node_ptr(T* raw): ptr_(raw)
        {
            // Do nothing.
        }

        //! Constructs a basic_node_ptr from another basic_node_ptr. This non-explicit constructor
        //! requires that type U* is implicitly convertible to type T* (e.g. U is a subclass of T).
        template<class U>
        basic_node_ptr(const basic_node_ptr<U>& rhs): ptr_(rhs.ptr_)
        {
            // Do nothing.
        }

        //! \}

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.
        // Use auto-generated destructor.

        //! \cond DEV

        ///////////////////////////////////////////////////////////////////////////////////////////
        //! \name Cast Constructors (should not be used by client code)
        //! \{

        struct static_cast_tag { };   //!< Tag struct identifying a static node cast.
        struct const_cast_tag { };    //!< Tag struct identifying a const node cast.
        struct dynamic_cast_tag { };  //!< Tag struct identifying a dynamic node cast.

        template<class U>
        explicit basic_node_ptr(const basic_node_ptr<U>& rhs, static_cast_tag)
        : ptr_(static_cast<T*>(rhs.ptr_))
        {
            // Do nothing.
        }

        template<class U>
        explicit basic_node_ptr(const basic_node_ptr<U>& rhs, const_cast_tag)
        : ptr_(const_cast<T*>(rhs.ptr_))
        {
            // Do nothing.
        }

        template<class U>
        explicit basic_node_ptr(const basic_node_ptr<U>& rhs, dynamic_cast_tag)
        : ptr_(0)
        {
            if (rhs != 0)
            {
                ptr_ = T::cast(rhs->raw());
            }
        }

        //! \}

        //! \endcond

        ///////////////////////////////////////////////////////////////////////////////////////////
        //! \name Pointer Operators
        //! \{

        T& operator*() const
        {
            if (ptr_ == 0)
            {
                throw null_pointer();
            }
            return (*ptr_);
        }

        T* operator->() const
        {
            if (ptr_ == 0)
            {
                throw null_pointer();
            }
            return ptr_;
        }

        operator bool() const
        {
            return (ptr_ != 0);
        }

        bool operator!() const
        {
            return (ptr_ == 0);
        }

        template<class U>
        bool operator==(const basic_node_ptr<U>& rhs) const
        {
            return (ptr_ == rhs.ptr_);
        }

        template<class U>
        bool operator!=(const basic_node_ptr<U>& rhs) const
        {
            return (ptr_ != rhs.ptr_);
        }

        //! \}

        ///////////////////////////////////////////////////////////////////////////////////////////
        //! \name Deletion
        //! \{

        //! Deletes the underlying node pointer and sets the pointer to null. Calling this function
        //! is different from using the delete keyword on a pointer. This function delegates the
        //! deletion to XML implementation, which should unlink the node from the DOM tree and free
        //! the node.
        void delete_()
        {
            if (ptr_ != 0) {
                T::delete_(ptr_);
                ptr_ = 0;
            }
        }

        //! \}

    private:

        T* ptr_;

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Node Cast Functions
    //! \{


    //! Performs a static cast from pointer-to-U to pointer-to-T. Static cast is typically used to
    //! cast from subclass to superclass (up-cast), or to add constness to a node pointer.
    //!
    //! Typical use cases:
    //!
    //! \code
    //! // Add constness to pointer:
    //! basic_node_ptr<const element> ptr = static_node_cast<const element>(ptr_to_element);
    //! // Perform an up-cast:
    //! basic_node_ptr<node> ptr = static_node_cast<node>(ptr_to_element);
    //! \endcode
    //!
    //! \param ptr  the pointer-to-U to cast.
    //! \return the resulting pointer-to-T.
    template<class T, class U>
    inline basic_node_ptr<T> static_node_cast(const basic_node_ptr<U>& ptr)
    {
        return basic_node_ptr<T>( ptr, typename basic_node_ptr<T>::static_cast_tag() );
    }


    //! Performs a const cast from pointer-to-U to pointer-to-T. Const cast is used to cast away
    //! the constness of a pointer.
    //!
    //! Typical use cases:
    //!
    //! \code
    //! basic_node_ptr<element> ptr = const_node_cast<element>(ptr_to_const_element);
    //! \endcode
    //!
    //! \param ptr  the pointer-to-U to cast.
    //! \return the resulting pointer-to-T.
    template<class T, class U>
    inline basic_node_ptr<T> const_node_cast(const basic_node_ptr<U>& ptr)
    {
        return basic_node_ptr<T>( ptr, typename basic_node_ptr<T>::const_cast_tag() );
    }


    //! Performs a dynamic cast from pointer-to-U to pointer-to-T. Dynamic cast is used to cast
    //! from superclass to subclass (down-cast). Such cast requires that RTTI be activated. If
    //! the dynamic cast fails, this function returns a null pointer.
    //!
    //! Typical use cases:
    //!
    //! \code
    //! basic_node_ptr<element> ptr = dynamic_node_cast<element>(ptr_to_node);
    //! \endcode
    //!
    //! \param ptr  the pointer-to-U to cast.
    //! \return the resulting pointer-to-T.
    template<class T, class U>
    inline basic_node_ptr<T> dynamic_node_cast(const basic_node_ptr<U>& ptr)
    {
        return basic_node_ptr<T>( ptr, typename basic_node_ptr<T>::dynamic_cast_tag() );
    }


    //! \}


} // namespace xtree


#endif  // XTREE_BASIC_NODE_PTR_HPP_20080717__


//
// Created by ZHENG Zhong on 2011-08-24.
//

#ifndef XTREE_BASIC_XMLNS_PTR_HPP_20110824__
#define XTREE_BASIC_XMLNS_PTR_HPP_20110824__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"


namespace xtree {


    //! This class template wraps a raw pointer to XML namespace object and provides a pointer-like
    //! interface. By using this class template, users are disallowed to delete the raw pointer.
    template<class T>
    class basic_xmlns_ptr
    {

        template<class U> friend class basic_xmlns_ptr;

    public:

        ///////////////////////////////////////////////////////////////////////////////////////////
        //! \name Constructors
        //

        //! Constructs a pointer wrapper to null.
        explicit basic_xmlns_ptr(): ptr_(0)
        {
            // Do nothing.
        }

        //! Constructs a pointer wrapper to the specified raw pointer.
        //! \param raw  the raw pointer to wrap.
        explicit basic_xmlns_ptr(T* raw): ptr_(raw)
        {
            // Do nothing.
        }

        //! Constructs a basic_xmlns_ptr from another basic_xmlns_ptr. This non-explicit
        //! constructor requires that U* is implicitly convertible to T* (e.g. U is const T).
        template<class U>
        basic_xmlns_ptr(const basic_xmlns_ptr<U>& rhs): ptr_(rhs.ptr_)
        {
            // Do nothing.
        }

        //! \}

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.
        // Use auto-generated destructor.

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
        bool operator==(const basic_xmlns_ptr<U>& rhs) const
        {
            return (ptr_ == rhs.ptr_);
        }

        template<class U>
        bool operator!=(const basic_xmlns_ptr<U>& rhs) const
        {
            return (ptr_ != rhs.ptr_);
        }

        //! \}

    private:

        T* ptr_;

    };


} // namespace xtree


#endif  // XTREE_BASIC_XMLNS_PTR_HPP_20110824__


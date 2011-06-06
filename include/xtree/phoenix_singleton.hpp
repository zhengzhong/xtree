//
// Created by ZHENG Zhong on 2008-07-21.
//

#ifndef XTREE_PHOENIX_SINGLETON_HPP_20080721__
#define XTREE_PHOENIX_SINGLETON_HPP_20080721__

#include "xtree/config.hpp"

#include <cstdlib>  // for std::atexit()
#include <new>      // for placement new


//! \cond DEV

namespace xtree {
namespace detail {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // phoenix_singleton class template
    //

    //! This class represents a phoenix singleton holder (see Modern C++ Design [MC++D], by Andrei
    //! Alexandrescu). A phoenix singleton class has the following states:
    //!
    //! - non-initialized: ( instance_ == 0 && destroyed_ == false )
    //!
    //!   This is the initial state guaranteed by the compiler. In this state, the singleton has
    //!   not been created, thus the compiler will never destroy the singleton on this state.
    //!
    //!   If instance() is called on this state, it will firstly delegate to create_singleton() to
    //!   create the singleton and change the state to initialized. Then it will return a reference
    //!   to the singleton instance.
    //!
    //! - initialized ( instance_ != 0 && destroyed_ == false )
    //!
    //!   In this state, the singleton has been created yet not been destroyed. The compiler may
    //!   decide to destroy the singleton at program exit, in an arbitrary order comparing to the
    //!   destructions of other singletons or static/global variables. The compiler destroy the
    //!   singleton by calling its destructor, which will change the state to destroyed.
    //!
    //!   If instance() is called on this state, it returns a reference to the singleton instance
    //!   directly.
    //!
    //! - destroyed: ( instance_ == 0 && destroyed_ == true )
    //!
    //!   In this state, the singleton has been destroyed by the compiler at program exit. Since
    //!   the singleton is declared as a static local variable in create_singleton(), although it
    //!   has already been destroyed, the compiler guarantee that static objects' memory lasts for
    //!   the duration of the program (MC++D, p120).
    //!
    //!   If instance() is called on this state, a dead reference is detected (someone is trying to
    //!   access the singleton instance after its destruction). It will firstly delegate to
    //!   rebear_singleton() to re-create the singleton instance in the shell of the old one,
    //!   register this new singleton's destruction with std::atexit(), and change the state to
    //!   initialized. Then it will return a reference to the re-born singleton instance.
    template<class T>
    class phoenix_singleton
    {

        typedef T                    singleton_type; //!< The underlying singleton type.
        typedef phoenix_singleton<T> this_type;      //!< Type of the templatized class.

    public:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // public static singleton instance getter
        //

        //! Returns the singleton instance.
        //! \return the singleton instance.
        static singleton_type& instance()
        {
            if (instance_ == 0)
            {
                if (!destroyed_)
                {
                    // state is 'non-initialized': delegate to create_singleton().
                    create_singleton();
                }
                else
                {
                    // state is 'destroyed': delegate to reborn_singleton().
                    rebear_singleton();
                }
            }
            return *instance_;
        }

    protected:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // protected constructor and destructor
        //

        //! Protected constructor to prevent external instantiation.
        explicit phoenix_singleton()
        {
            // Do nothing.
        }

        //! Protected constructor to prevent external destruction. This function sets the static
        //! pointer to the singleton instance to 0, and the static destroyed flag to true. In this
        //! way, the current state will change to destroyed after the destructor.
        virtual ~phoenix_singleton()
        {
            destroyed_ = true;
            instance_ = 0;
        }

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // static life-cycle management functions
        //

        //! Creates the (static) singleton object. This function may be called on two states:
        //! when the singleton is 'non-initialized'; or is 'destroyed'.
        //!
        //! In the first case, the static singleton variable will be created (this is the first
        //! time this function is invoked), and the destroyed flag was initialized to false by the
        //! compiler. Thus this function sets the static singleton pointer to the address of the
        //! newly-created static singleton variable.
        //!
        //! In the second case, this function is called by rebear_singleton(). The static singleton
        //! variable has already been created, and has been destroyed, and the destroyed flag was
        //! set to true (by the destructor). Thus this function sets the static singleton pointer
        //! to the original address (the shell) of the (destroyed) static singleton variable. It is
        //! then up to the rebear_singleton() to re-create the singleton in the shell.
        //!
        //! Note: although the static singleton object may be destroyed, its memory will last for
        //! the duration of the program (the property of static variables inside a function).
        static void create_singleton()
        {
            static singleton_type singleton;
            instance_ = &singleton;
        }

        //! This function is called when the current state is 'destroyed'. It re-creates the
        //! singleton in the shell of the old one, register the destruction of the new singleton
        //! object with std::atexit(), and change the state to 'initialized'.
        static void rebear_singleton()
        {
            create_singleton();               // obtain the shell of the destroyed singleton.
            new(instance_) singleton_type();  // placement new: re-create singleton in the shell.
            std::atexit(kill_singleton);      // register the destruction with std::atexit().
            destroyed_ = false;               // now the state is changed to 'initialized'.
        }

        //! If this function was registered with std::exit(), it will get called by the standard C
        //! library at program exit. It destroys the singleton (but does not free the memory --
        //! because the memory where the singleton is seated in was not allocated by the new
        //! operator), and change the state to 'destroyed'.
        static void kill_singleton()
        {
            // Call the destructor on the singleton instance pointer. Since the concrete singleton
            // class derives from this class template, the destructor of the phoenix singleton
            // class template will also get called, which will change the state to 'destroyed'.
            instance_->~singleton_type();
        }

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // noncopyable
        //

        //! Non-implemented copy constructor.
        phoenix_singleton(const this_type&);

        //! Non-implemented copy assignment.
        this_type& operator=(const this_type&);

    private:

        static singleton_type* instance_;  //!< The singleton instance.
        static bool            destroyed_; //!< Flag indicating if the singleton is destroyed.

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // phoenix_singleton class template static member initialization
    //


    template<class T>
    T* phoenix_singleton<T>::instance_ = 0;


    template<class T>
    bool phoenix_singleton<T>::destroyed_ = false;


} // namespace xtree::detail
} // namespace xtree

//! \endcond


#endif // XTREE_PHOENIX_SINGLETON_HPP_20080721__




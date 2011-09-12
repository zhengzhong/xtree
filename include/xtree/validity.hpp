//
// Created by ZHENG Zhong on 2009-07-15.
//

#ifndef XTREE_VALIDITY_HPP_20090715__
#define XTREE_VALIDITY_HPP_20090715__

#include "xtree/config.hpp"

#include <string>
#include <vector>


namespace xtree {


    //! \cond DEV

    namespace detail {

        void on_validity_error(void* ctxt, const char* message, ...);

        void on_validity_warning(void* ctxt, const char* message, ...);

    }  // namespace xtree::detail

    //! \endcond


    ////////////////////////////////////////////////////////////////////////////////////////////////


    //! This class contains validation result of an XML document. A validity object is immutable.
    //! It can be implicitly converted to a boolean value, and it supports the "not" operator.
    //! The validation is considered to be passed if there are no errors. Validation warnings do
    //! not affect the passed/failed status.
    class XTREE_DECL validity
    {

        //! \cond DEV

        friend void detail::on_validity_error(void* ctxt, const char* message, ...);
        friend void detail::on_validity_warning(void* ctxt, const char* message, ...);

        //! \endcond

    public:

        typedef std::vector<std::string>  messages;
        typedef messages::size_type       size_type;
        typedef messages::const_iterator  const_iterator;

    public:

        explicit validity();

        // Use auto-generated copy constructor.
        // Use auto-generated copy assignment.
        // Use auto-generated destructor.

        const messages& errors() const;

        const messages& warnings() const;

        bool is_valid() const;

        operator bool() const
        {
            return is_valid();
        }

        bool operator!() const
        {
            return !is_valid();
        }

        void add_error(const std::string& message);

        void add_warning(const std::string& message);

    private:

        std::vector<std::string> errors_;
        std::vector<std::string> warnings_;

    };


}  // namespace xtree


#endif  // XTREE_VALIDITY_HPP_20090715__


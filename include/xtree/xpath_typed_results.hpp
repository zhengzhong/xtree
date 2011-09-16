//
// Created by ZHENG Zhong on 2011-09-16.
//

#ifndef XTREE_XPATH_TYPED_RESULTS_HPP_20110916__
#define XTREE_XPATH_TYPED_RESULTS_HPP_20110916__

#include "xtree/config.hpp"
#include "xtree/xpath_result.hpp"

#include <string>


namespace xtree {


    class XTREE_DECL xpath_boolean: public xpath_result
    {

    public:

        //! Default constructor.
        explicit xpath_boolean();

        // Use auto-generated destructor.

        //! Returns the result value.
        //! \return the result value.
        bool value() const;

        //! Implicit conversion to bool.
        operator bool() const
        {
            return value();
        }

    private:

        virtual bool is_acceptable_(xpath_result_t type) const;

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////


    class XTREE_DECL xpath_number: public xpath_result
    {

    public:

        //! Default constructor.
        explicit xpath_number();

        // Use auto-generated destructor.

        //! Returns the result value.
        //! \return the result value.
        double value() const;

        //! Implicit conversion to int.
        operator double() const
        {
            return value();
        }

    private:

        virtual bool is_acceptable_(xpath_result_t type) const;

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////


    class XTREE_DECL xpath_string: public xpath_result
    {

    public:

        //! Default constructor.
        explicit xpath_string();

        // Use auto-generated destructor.

        //! Returns the result value.
        //! \return the result value.
        std::string value() const;

        //! Implicit conversion to int.
        operator std::string() const
        {
            return value();
        }

    private:

        virtual bool is_acceptable_(xpath_result_t type) const;

    };


}  // namespace xtree


#endif  // XTREE_XPATH_TYPED_RESULTS_HPP_20110916__


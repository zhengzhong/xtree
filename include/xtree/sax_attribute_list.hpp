//
// Created by ZHENG Zhong on 2009-01-22.
//

#ifndef XTREE_SAX_ATTRIBUTE_LIST_HPP_20090122__
#define XTREE_SAX_ATTRIBUTE_LIST_HPP_20090122__

#include "xtree/config.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <string>
#include <vector>


namespace xtree {


    //! This class represents an XML attribute during the SAX parsing.
    class XTREE_DECL sax_attribute
    {

    public:

        explicit sax_attribute(int index, const xmlChar** attrs);

        const std::string& name() const
        {
            return name_;
        }

        const std::string& prefix() const
        {
            return prefix_;
        }

        const std::string& uri() const
        {
            return uri_;
        }

        const std::string& value() const
        {
            return value_;
        }

    private:

        std::string name_;
        std::string prefix_;
        std::string uri_;
        std::string value_;

    };


    typedef std::vector<sax_attribute> sax_attribute_list;


} // namespace xtree


#endif // XTREE_SAX_ATTRIBUTE_HPP_20090122__


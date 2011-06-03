//
// Created by ZHENG Zhong on 2009-01-22.
//

#ifndef XTREE_SAX_ATTRIBUTE_LIST_HPP_20090122__
#define XTREE_SAX_ATTRIBUTE_LIST_HPP_20090122__

#include "xtree/config.hpp"
#include "xtree/sax_xmlns_context.hpp"

#include <string>
#include <vector>


namespace xtree {


    class XTREE_DECL sax_attribute
    {

    public:

        explicit sax_attribute();

        explicit sax_attribute(const std::string& name,
                               const std::string& uri,
                               const std::string& prefix,
                               const std::string& value);

        const std::string& name() const
        {
            return name_;
        }

        const std::string& uri() const
        {
            return uri_;
        }

        const std::string& prefix() const
        {
            return prefix_;
        }

        std::string qname() const
        {
            return (prefix_.empty() ? name_ : prefix_ + ":" + name_);
        }

        const std::string& value() const
        {
            return value_;
        }

    private:

        std::string name_;
        std::string uri_;
        std::string prefix_;
        std::string value_;
    };


    typedef std::vector<sax_attribute> sax_attribute_list;


} // namespace xtree


#endif // XTREE_SAX_ATTRIBUTE_HPP_20090122__


//
// Created by ZHENG Zhong on 2008-07-16.
//

#ifndef XTREE_LIBXML2_UTILITY_HPP_20080716__
#define XTREE_LIBXML2_UTILITY_HPP_20080716__

#include "xtree/config.hpp"
#include "xtree/types.hpp"

#include <libxml/tree.h>
#include <cassert>
#include <string>
#include <utility>


//! \cond DEV

namespace xtree {
namespace detail {


    //! Converts the XML node type enumeration.
    //! \param source_type  the libxml2-defined node type.
    //! \return the xml node type defined as node_t.
    inline node_t to_node_t(xmlElementType source_type)
    {
        node_t target_type = unknown_node;
        switch (source_type) {
        case XML_ATTRIBUTE_NODE:
            target_type = attribute_node;
            break;
        case XML_ELEMENT_NODE:
            target_type = element_node;
            break;
        case XML_TEXT_NODE:
            target_type = text_node;
            break;
        case XML_CDATA_SECTION_NODE:
            target_type = cdata_node;
            break;
        case XML_COMMENT_NODE:
            target_type = comment_node;
            break;
        case XML_PI_NODE:
            target_type = xml_pi_node;
            break;
        default:
            assert(! "Unknown libxml2 node type");
            break;
        }
        return target_type;
    }


    //! Converts libxml2's xmlChar* to const char*.
    inline const char* to_chars(const xmlChar* chars)
    {
        return reinterpret_cast<const char*>(chars);
    }


    //! Converts const char* to libxml2's xmlChar*.
    inline const xmlChar* to_xml_chars(const char* chars)
    {
        return reinterpret_cast<const xmlChar*>(chars);
    }


    //! Splits QName into a pair of namespace prefix and local name.
    //! \param qname  the QName to split.
    //! \return a pair of namespace prefix and local name.
    inline std::pair<std::string, std::string> split_qname(const std::string& qname)
    {
        std::string::size_type pos = qname.find_first_of(':');
        if (pos != std::string::npos)
        {
            return std::make_pair(qname.substr(0, pos), qname.substr(pos + 1));
        }
        else
        {
            return std::make_pair(std::string(), qname);
        }
    }


    //! Splits QName into a pair of namespace prefix and local name.
    //! \param qname  the QName to split.
    //! \return a pair of namespace prefix and local name.
    inline std::pair<std::string, std::string> split_qname(const xmlChar* qname)
    {
        std::string qname_str = to_chars(qname);
        return split_qname(qname_str);
    }


    //! Builds an error message from an error code.
    //! \param code  the error code.
    //! \return an error message.
    std::string build_error_message(int code);


    //! Builds an error message from an xmlError object.
    //! \param err  the xmlError object.
    //! \return an error message.
    std::string build_error_message(const xmlError& err);


}  // namespace xtree::detail
}  // namespace xtree

//! \endcond


#endif  // XTREE_LIBXML2_UTILITY_HPP_20080716__




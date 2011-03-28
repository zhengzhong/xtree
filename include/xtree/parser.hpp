//
// Created by ZHENG Zhong on 2008-01-17.
//

#ifndef XTREE_PARSER_HPP_20080717__
#define XTREE_PARSER_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/dom_base.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <string>


namespace xtree {


    class XTREE_DECL document;


    //! This class represents an XML DOM parser. A parser is used to parse an XML file or string
    //! and return the document object.
    class XTREE_DECL parser: private dom_base
    {

    public:

        //! Default constructor.
        explicit parser();

        //! Destructor.
        ~parser();

        //! Parses an XML file to a document object. It is up to the user to free the document
        //! object. It is recommended that user use the RAII idiom and put the returned document
        //! into a smart pointer (std::auto_ptr or boost::shared_ptr).
        //! \param file_name  the name of the xml file to parse.
        //! \return a document object parsed from the file, never null.
        //! \throws dom_error  if fail to parse the XML file.
        document* parse_file(const std::string& file_name);

        //! Parses a memory block containing XML to a document object. It is up to the user to free
        //! the document object. It is recommended that user use the RAII idiom and put the
        //! returned document into a smart pointer (std::auto_ptr or boost::shared_ptr).
        //! \param str  the string containing XML to parse.
        //! \return a document object parsed from the string, never null.
        //! \throws dom_error  if fail to parse the XML string.
        document* parse_string(char const* str);

    private:

        //! Non-implemented copy constructor.
        parser(const parser&);

        //! Non-implemented copy assignment.
        parser& operator=(const parser&);

    };


    //! Parses an XML file to a document object.
    inline document* parse_file(const std::string& file_name)
    {
        parser p;
        return p.parse_file(file_name);
    }


    //! Parses an XML string to a document object.
    inline document* parse_string(const char* str)
    {
        parser p;
        return p.parse_string(str);
    }


}  // namespace xtree


#endif  // XTREE_PARSER_HPP_20080717__




//
// Created by ZHENG Zhong on 2008-01-17.
//

#ifndef XTREE_DOM_PARSER_HPP_20080717__
#define XTREE_DOM_PARSER_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/xml_base.hpp"
#include "xtree/document.hpp"
#include "xtree/libxml2_fwd.hpp"

#if defined(XTREE_GNUC) && XTREE_GNUC >= 4
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"  // std::auto_ptr is deprecated.
#endif

#include <memory>
#include <string>


namespace xtree {


    //! This class represents an XML DOM parser. A parser is used to parse an XML file or string
    //! and return the document object.
    class XTREE_DECL dom_parser: private xml_base
    {

    public:

        //! Default constructor.
        explicit dom_parser();

        //! Destructor.
        ~dom_parser();

        //! Parses an XML file to a document object.
        //! \param file_name  the name of the xml file to parse.
        //! \return a document object parsed from the file, never null.
        //! \throws dom_error  if fail to parse the XML file.
        std::auto_ptr<document> parse_file(const std::string& file_name);

        //! Parses a memory block containing XML to a document object.
        //! \param str  the string containing XML to parse.
        //! \return a document object parsed from the string, never null.
        //! \throws dom_error  if fail to parse the XML string.
        std::auto_ptr<document> parse_string(char const* str);

        //! Parses an XML file from a given URL to a document object.
        //! \param url    the URL to the XML file to parse.
        //! \param proxy  the HTTP proxy. If empty (by default), use system variable "http_proxy".
        //! \return a document object parsed from the remote XML, never null.
        //! \throws dom_error  if fail to parse the remote XML.
        std::auto_ptr<document> parse_url(const std::string& url,
                                          const std::string& proxy = std::string());

    private:

        //! Non-implemented copy constructor.
        dom_parser(const dom_parser&);

        //! Non-implemented copy assignment.
        dom_parser& operator=(const dom_parser&);

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name XML Parsing Functions
    //! \{


    //! Parses an XML file to a document object.
    inline std::auto_ptr<document> parse_file(const std::string& file_name)
    {
        dom_parser p;
        return p.parse_file(file_name);
    }


    //! Parses an XML string to a document object.
    inline std::auto_ptr<document> parse_string(const char* str)
    {
        dom_parser p;
        return p.parse_string(str);
    }


    //! Parses an XML file from a URL to a document object.
    inline std::auto_ptr<document> parse_url(const std::string& url,
                                             const std::string& proxy = std::string())
    {
        dom_parser p;
        return p.parse_url(url, proxy);
    }


    //! \}


}  // namespace xtree


#endif  // XTREE_DOM_PARSER_HPP_20080717__




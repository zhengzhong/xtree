//
// Created by ZHENG Zhong on 2009-07-15.
//

#ifndef XTREE_SCHEMA_HPP_20090715__
#define XTREE_SCHEMA_HPP_20090715__

#include "xtree/config.hpp"
#include "xtree/xml_base.hpp"
#include "xtree/validity.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <string>


namespace xtree {


    class XTREE_DECL document;  // forward declaration.


    //! This class represents an XML schema.
    class XTREE_DECL schema: private xml_base
    {

    public:

        //! Constructs an unloaded schema.
        explicit schema();

        //! Destructor.
        ~schema();

        //! Checks if an XSD schema has been loaded in this object.
        //! \return true if an XSD has been loaded in this object, false otherwise.
        bool loaded() const;

        //! Loads an XSD schema from an XML file.
        //! \param file_name  the XSD schema file to load.
        void load_from_file(const std::string& file_name);

        //! Loads an XSD schema from an XML string.
        //! \param str  the XSD schema string to load.
        void load_from_string(const char* str);

        //! Validates the XML document against this schema.
        //! \param doc  the XML document to be validated.
        //! \return a validity object containing the validation result.
        validity validate(document& doc) const;

    private:

        //! Non-implemented copy constructor.
        schema(const schema&);

        //! Non-implemented copy assignment.
        schema& operator=(const schema&);

        //! Frees the underlying libxml2 schema object and set the pointer to null.
        void free_();

    private:

        xmlSchema* raw_;  //!< The underlying libxml2 schema object.

    };


}  // namespace xtree


#endif  // XTREE_SCHEMA_HPP_20090715__




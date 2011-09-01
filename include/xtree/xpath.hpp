#ifndef XTREE_XPATH_HPP_20110901__
#define XTREE_XPATH_HPP_20110901__

#include "xtree/config.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <map>
#include <string>


namespace xtree {


    //! This class wraps a compiled XPath expression.
    class XTREE_DECL xpath
    {

    public:

        //! The XML namespace registry.
        typedef std::map<std::string, std::string> xmlns_registry;

    public:

        //! Constructs an XPath from an expression.
        //! \param str  the XPath expression.
        explicit xpath(const std::string& str);

        //! Constructs an XPath from an expression.
        //! \param str     the XPath expression.
        //! \param prefix  the XML namespace prefix to register.
        //! \param uri     the XML namespace URI to register.
        xpath(const std::string& str, const std::string& prefix, const std::string& uri);

        //! Copy constructor.
        xpath(const xpath& rhs);

        //! Copy assignment.
        xpath& operator=(const xpath& rhs);

        //! Destructs the compiled XPath and frees its underlying object.
        ~xpath();

        //! Registers an XML namespace. If the namespace prefix already exists, the mapping URI
        //! will be overwritten.
        //! \param prefix  the XML namespace prefix to register.
        //! \param uri     the XML namespace URI to register.
        void register_xmlns(const std::string& prefix, const std::string& uri)
        {
            registry_[prefix] = uri;
        }

        //! Returns the XML namespace registry.
        //! \return the XML namespace registry.
        const xmlns_registry& get_xmlns_registry() const
        {
            return registry_;
        }

        //! Returns the XPath expression.
        //! \return the XPath expression.
        const std::string& str() const
        {
            return str_;
        }

    public:

        //! Returns the underlying libxml2 XPath object. This function should NOT be called by
        //! client code.
        const xmlXPathCompExpr* raw() const
        {
            return raw_;
        }

    private:

        std::string       str_;       //!< The string representation of the XPath expression.
        xmlXPathCompExpr* raw_;       //!< The underlying libxml2 XPath object.
        xmlns_registry    registry_;  //!< The XML namespace registry.

    };


}  // namespace xtree


#endif  // XTREE_XPATH_HPP_20110901__

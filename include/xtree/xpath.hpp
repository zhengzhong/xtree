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

        //! Constructs an XPath from an expression. This non-explicit constructor allows implicit
        //! conversion from std::string.
        //! \param str  the XPath expression.
        xpath(const std::string& str);

        //! Constructs an XPath from an expression. This non-explicit constructor allows implicit
        //! conversion from const char*.
        //! \param str  the XPath expression.
        xpath(const char* str);

        //! Constructs an XPath from an expression.
        //! \param str     the XPath expression.
        //! \param prefix  the XML namespace prefix to register.
        //! \param uri     the XML namespace URI to register.
        xpath(const std::string& str, const std::string& prefix, const std::string& uri);

        //! Constructs an XPath from an expression.
        //! \param str       the XPath expression.
        //! \param registry  the XML namespace registry.
        xpath(const std::string& str, const xmlns_registry& registry);

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

        //! Registers XML namespaces. If one namespace prefix already exists, the mapping URI
        //! will be overwritten.
        //! \param registry  the XML namespace registry.
        void register_xmlns(const xmlns_registry& registry)
        {
            for (xmlns_registry::const_iterator i = registry.begin(); i != registry.end(); ++i)
            {
                registry_[i->first] = i->second;
            }
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

        //! Initializes the underlying libxml2 xpath object. This function is used by constructors.
        //! \param str  the XPath expression.
        void init_raw_(const std::string& str);

    private:

        std::string       str_;       //!< The string representation of the XPath expression.
        xmlns_registry    registry_;  //!< The XML namespace registry.
        xmlXPathCompExpr* raw_;       //!< The underlying libxml2 XPath object.

    };


}  // namespace xtree


#endif  // XTREE_XPATH_HPP_20110901__


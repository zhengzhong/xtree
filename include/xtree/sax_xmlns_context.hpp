//
// Created by ZHENG Zhong on 2011-05-12.
//

#ifndef XTREE_SAX_XMLNS_CONTEXT_HPP_20110512__
#define XTREE_SAX_XMLNS_CONTEXT_HPP_20110512__

#include "xtree/config.hpp"

#include <map>
#include <string>


namespace xtree {


    //! XML namespace context for SAX parser.
    class XTREE_DECL sax_xmlns_context
    {

        //! The XML namespace mappings. Key is the element level in the XML document (the root
        //! element has the level 1). Values associated to a certain key are namespace prefix-URI
        //! pairs taking effects from the level.
        typedef std::multimap<int, std::pair<std::string, std::string> > xmlns_map;

    public:

        //! Default constructor.
        explicit sax_xmlns_context();

        //! Resets the context.
        void reset();

        //! Pushes an XML namespace mapping into the context.
        //! \param level   the level from which the namespace mapping takes effect.
        //! \param prefix  the namespace prefix.
        //! \param uri     the namespace URI.
        void push(int level, const std::string& prefix, const std::string& uri);

        //! Pops all XML namespace mappings that take effects from the specified level.
        //! \param level  the level to pop.
        void pop(int level);

        //! Finds the XML namespace URI by the given prefix. This function looks up the namespace
        //! prefix from the nearest level, and returns the first URI found.
        //! \param prefix  the XML namespace prefix to look up.
        //! \return the XML namespace URI associated to the prefix.
        //! \throws sax_error  if no XML namespace URI is found.
        const std::string& find_uri(const std::string& prefix) const;

    private:

        int       max_level_;  //! The upper bound of the levels.
        xmlns_map context_;

    };


}  // namespace xtree


#endif  // XTREE_SAX_XMLNS_CONTEXT_HPP_20110512__


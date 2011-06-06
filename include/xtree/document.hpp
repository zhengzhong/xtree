//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_DOCUMENT_HPP_20080717__
#define XTREE_DOCUMENT_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/types.hpp"
#include "xtree/basic_node_ptr.hpp"

#include "xtree/xml_base.hpp"
#include "xtree/child_node_list.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <cassert>
#include <map>
#include <string>
#include <vector>


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // forward declarations
    //


    class XTREE_DECL node;
    class XTREE_DECL element;
    class XTREE_DECL attribute;
    class XTREE_DECL text;
    class XTREE_DECL comment;

    class XTREE_DECL node_set;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // document
    //


    //! This class represents an XML document.
    class XTREE_DECL document: private xml_base
    {

    public:

        //! Default constructor. Constructs an empty XML document.
        explicit document();

        //! Constructs a document from a libxml2 document. This function should NOT be called by
        //! client code.
        explicit document(xmlDoc* px);

        //! Destroys the DOM tree and releases all resources.
        ~document();

        ////////////////////////////////////////////////////////////////////////////////////////////
        // document information access
        //

        //! Returns the encoding of this document.
        //! \return the encoding of this document.
        std::string encoding() const;

        //! Returns the XML version number of this document.
        //! \return the XML version number of this document.
        std::string version() const;

        //! Checks if this document is empty. An empty document does not contain a root element.
        //! \return true if this document is empty, false otherwise.
        bool empty() const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children access
        //

        //! Returns a reference to the child node list of this document.
        //! \return a reference to the child node list of this document.
        child_node_list& children();

        //! Const version of children().
        const child_node_list& children() const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // root element access
        //

        //! Returns the root element of this document.
        //! \return the root element of this document.
        basic_node_ptr<element> root();

        //! Const version of root().
        basic_node_ptr<const element> root() const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // serialization
        //

        //! Returns a string representation of the whole XML document.
        //! \return a string representation of the whole XML document.
        std::string str() const;

        //! Retrieves a string representation of the whole XML document.
        //! \param str  a string to hold the result.
        void str(std::string& str) const;

        //! Saves the whole XML document to a file.
        //! \param file_name  the file name.
        void save_to_file(const std::string& file_name) const;

        //! Clones the document.
        //! \return the cloned document.
        document* clone() const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // modifiers
        //

        //! Clears the document. This function destroys the underlying DOM tree and releases all
        //! the resources, thus invalidates all the nodes (including orphan nodes).
        //! \post empty() == true
        void clear();

        //! Resets the root element to this document. This function will firstly clear the document
        //! then reset the root element.
        //! \post all the original nodes within this document are invalidated.
        //! \param name  the name of the element to reset as root.
        //! \return pointer to the new root element of this document.
        //! \throws bad_dom_operation  if $root is not an element.
        basic_node_ptr<element> reset_root(const std::string& name);

        //! Resets the root element to this document. This function will firstly clear the document
        //! then reset the root element.
        //! \post all the original nodes within this document are invalidated.
        //! \param name  the name of the element to reset as root.
        //! \param uri   the namespace URI of the element to reset as root.
        //! \return pointer to the new root element of this document.
        //! \throws bad_dom_operation  if $root is not an element.
        basic_node_ptr<element> reset_root(const std::string& name, const std::string& uri);

        //! Resets the root element to this document (the document will firstly be cleared).
        //! This function makes a recursive clone of the element and resets the cloned one as the
        //! root element.
        //! \post all the original nodes within this document are invalidated.
        //! \param elem  the element to reset as root.
        //! \return pointer to the new root element of this document.
        basic_node_ptr<element> reset_root_clone(const element& elem);

        //! Resets the root element to this document (the document will firstly be cleared).
        //! This function takes the ownership of the element from the source DOM tree.
        //! \post all the original nodes within this document are invalidated.
        //! \param elem  the element to reset as root.
        //! \return pointer to the new root element of this document.
        basic_node_ptr<element> reset_root_adopt(element& elem);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // XPath
        //

        //! Selects a node set by an XPath expression.
        //! \param xpath   the XPath expression that evaluates to a node set.
        //! \param result  output argument to hold the result node set.
        void select_nodes(const std::string& xpath, node_set& result);

        //! Selects a node set by an XPath expression.
        //! \param xpath       the XPath expression that evaluates to a node set.
        //! \param ns_mapping  namespace mapping, a pair of prefix and URI.
        //! \param result      output argument to hold the result node set.
        void select_nodes(const std::string& xpath,
                          const std::pair<std::string, std::string>& ns_mapping,
                          node_set& result);

        //! Selects a node set by an XPath expression.
        //! \param xpath        the XPath expression that evaluates to a node set.
        //! \param ns_mappings  namespace mappings containing prefix and URI pairs.
        //! \param result       output argument to hold the result node set.
        void select_nodes(const std::string& xpath,
                          const std::map<std::string, std::string>& ns_mappings,
                          node_set& result);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // node creation
        //

        basic_node_ptr<element> make_element(const std::string& name) const;

        basic_node_ptr<text> make_text(const std::string& value) const;

        basic_node_ptr<text> make_cdata(const std::string& value) const;

        basic_node_ptr<comment> make_comment(const std::string& value) const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // namespace prefix generation
        //

        //! Generates a namespace prefix which is guaranteed to be unique in the document scope.
        //! \return a unique namespace prefix.
        //! \todo TODO: how to generate document-scope unique namespace prefix?
        std::string next_namespace_prefix() const;

    public:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // underlying libxml2 document access
        //

        xmlDoc* raw()
        {
            assert(raw_ != 0);
            return raw_;
        }

        const xmlDoc* raw() const
        {
            assert(raw_ != 0);
            return raw_;
        }

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // private
        //

        //! Non-implemented copy constructor.
        document(const document&);

        //! Non-implemented copy assignment.
        document& operator=(const document&);

        //! Returns the root element of this document, or null if this document is empty.
        //! \return the root element of this document, or null if this document is empty.
        const element* root_() const;

        //! Resets the libxml2 element node as the root element of this document.
        //! \post all the original nodes within this document are invalidated.
        //! \param px  the libxml2 element to reset as root, should not be null.
        //! \return pointer to the new root element of this document.
        basic_node_ptr<element> reset_root_(xmlNode* px);

        //! Checks if the specified node is an orphan of this document.
        //! \param orphan  the node to check.
        //! \return true if the specified node is an orphan of this document, false otherwise.
        bool is_my_orphan_(const node* orphan) const;

        void add_orphan_(node* orphan) const;

        void remove_orphan_(node* orphan) const;

        void reset_doc_(xmlDoc* px);

    private:

        xmlDoc* raw_;                         //!< Underlying libxml2 document.
        child_node_list children_;            //!< The child node list under this document.
        mutable int counter_;                 //!< Namespace prefix counter.
        mutable std::vector<node*> orphans_;  //!< Orphans that belong to this document.

    };


}  // namespace xtree


#endif  // XTREE_DOCUMENT_HPP_20080717__


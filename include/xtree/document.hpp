//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_DOCUMENT_HPP_20080717__
#define XTREE_DOCUMENT_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/types.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/child_node.hpp"
#include "xtree/child_node_list.hpp"
#include "xtree/xml_base.hpp"
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
    class XTREE_DECL xml_pi;

    class XTREE_DECL node_set;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // document
    //


    //! This class represents an XML document.
    class XTREE_DECL document: private xml_base
    {

    public:

        typedef child_node_list::iterator        child_iterator;
        typedef child_node_list::const_iterator  const_child_iterator;

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

        ////////////////////////////////////////////////////////////////////////////////////////////
        // root element
        //

        //! Returns the root element of this document.
        //! \return the root element of this document.
        basic_node_ptr<element> root();

        //! Const version of root().
        basic_node_ptr<const element> root() const;

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
        // children: capacity
        //

        //! Checks if this document is empty. An empty document does not contain any child node.
        //! Note that a document without a root element is NOT necessarily empty. It may also
        //! contain other child nodes such as processing instructions or comments.
        //! \return true if this document is empty, false otherwise.
        bool empty() const
        {
            return children_.empty();
        }

        //! Returns number of child nodes in this document.
        size_type size() const
        {
            return children_.size();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: iterators
        //

        child_iterator begin()
        {
            return children_.begin();
        }

        child_iterator end()
        {
            return children_.end();
        }

        const_child_iterator begin() const
        {
            return children_.begin();
        }

        const_child_iterator end() const
        {
            return children_.end();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: front and back
        //

        child_node& front()
        {
            return children_.front();
        }

        const child_node& front() const
        {
            return children_.front();
        }

        child_node& back()
        {
            return children_.back();
        }

        const child_node& back() const
        {
            return children_.back();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers/clear
        //

        //! Clears the document. This function destroys the underlying DOM tree, including all the
        //! orphan nodes, and releases all the resources, thus invalidates all the nodes.
        //! \post empty() == true
        void clear();

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers/push_back
        //

        //! Pushes a new element to the end of the document's child node list.
        //! \param name  the local name of the element to push.
        //! \return a pointer to the new element.
        //! \throws bad_dom_operation  if the document already has a root element.
        basic_node_ptr<element> push_back_element(const std::string& name)
        {
            if (root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.push_back_element(name);
            }
        }

        //! Pushes a new element to the end of the document's child node list.
        //! \param name  the local name of the element to push.
        //! \param uri   the namespace URI of the element to push.
        //! \return a pointer to the new element.
        //! \throws bad_dom_operation  if the document already has a root element.
        basic_node_ptr<element> push_back_element(const std::string& name, const std::string& uri)
        {
            if (root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.push_back_element(name, uri);
            }
        }

        //! Pushes a new comment to the end of the document's child node list.
        //! \param value  the content of the comment to push.
        //! \return a pointer to the new comment.
        basic_node_ptr<comment> push_back_comment(const std::string& value)
        {
            return children_.push_back_comment(value);
        }

        //! Pushes a new processing instruction to the end of the document's child node list.
        //! \param target  the target of the process instruction to push.
        //! \param value   the value of the process instruction to push.
        //! \return a pointer to the new processing instruction.
        basic_node_ptr<xml_pi> push_back_xml_pi(const std::string& target, const std::string& value)
        {
            return children_.push_back_xml_pi(target, value);
        }

        //! Clones and pushes the child node to the end of the document's child node list.
        //! \param child  the child node to clone and push.
        //! \return a pointer to the pushed child node.
        //! \throws bad_dom_operation  if the child node cannot be pushed (e.g. the child node is
        //!                            a text or CData node, or the child node is an element while
        //!                            the document already has a root element).
        basic_node_ptr<child_node> push_back_clone(const child_node& child)
        {
            if (child.type() == text_node || child.type() == cdata_node)
            {
                throw bad_dom_operation("document does not accept text or CData node");
            }
            else if (child.type() == element_node && root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.push_back_clone(child);
            }
        }

        //! Adopts and pushes the child node to the end of the document's child node list.
        //! \param child  the child node to adopt and push.
        //! \return a pointer to the pushed child node.
        //! \throws bad_dom_operation  if the child node cannot be pushed (e.g. the child node is
        //!                            a text or CData node, or the child node is an element while
        //!                            the document already has a root element).
        basic_node_ptr<child_node> push_back_adopt(child_node& child)
        {
            if (child.type() == text_node || child.type() == cdata_node)
            {
                throw bad_dom_operation("document does not accept text or CData node");
            }
            else if (child.type() == element_node && root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.push_back_adopt(child);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers/push_front
        //

        //! Pushes a new element to the front of the document's child node list.
        //! \param name  the local name of the element to push.
        //! \return a pointer to the new element.
        //! \throws bad_dom_operation  if the document already has a root element.
        basic_node_ptr<element> push_front_element(const std::string& name)
        {
            if (root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.push_front_element(name);
            }
        }

        //! Pushes a new element to the front of the document's child node list.
        //! \param name  the local name of the element to push.
        //! \param uri   the namespace URI of the element to push.
        //! \return a pointer to the new element.
        //! \throws bad_dom_operation  if the document already has a root element.
        basic_node_ptr<element> push_front_element(const std::string& name, const std::string& uri)
        {
            if (root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.push_front_element(name, uri);
            }
        }

        //! Pushes a new comment to the front of the document's child node list.
        //! \param value  the content of the comment to push.
        //! \return a pointer to the new comment.
        basic_node_ptr<comment> push_front_comment(const std::string& value)
        {
            return children_.push_front_comment(value);
        }

        //! Pushes a new processing instruction to the front of the document's child node list.
        //! \param target  the target of the process instruction to push.
        //! \param value   the value of the process instruction to push.
        //! \return a pointer to the new processing instruction.
        basic_node_ptr<xml_pi> push_front_xml_pi(const std::string& target, const std::string& value)
        {
            return children_.push_front_xml_pi(target, value);
        }

        //! Clones and pushes the child node to the front of the document's child node list.
        //! \param child  the child node to clone and push.
        //! \return a pointer to the pushed child node.
        //! \throws bad_dom_operation  if the child node cannot be pushed (e.g. the child node is
        //!                            a text or CData node, or the child node is an element while
        //!                            the document already has a root element).
        basic_node_ptr<child_node> push_front_clone(const child_node& child)
        {
            if (child.type() == text_node || child.type() == cdata_node)
            {
                throw bad_dom_operation("document does not accept text or CData node");
            }
            else if (child.type() == element_node && root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.push_front_clone(child);
            }
        }

        //! Adopts and pushes the child node to the front of the document's child node list.
        //! \param child  the child node to adopt and push.
        //! \return a pointer to the pushed child node.
        //! \throws bad_dom_operation  if the child node cannot be pushed (e.g. the child node is
        //!                            a text or CData node, or the child node is an element while
        //!                            the document already has a root element).
        basic_node_ptr<child_node> push_front_adopt(child_node& child)
        {
            if (child.type() == text_node || child.type() == cdata_node)
            {
                throw bad_dom_operation("document does not accept text or CData node");
            }
            else if (child.type() == element_node && root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.push_front_adopt(child);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers/pop
        //

        void pop_back()
        {
            children_.pop_back();
        }

        void pop_front()
        {
            children_.pop_front();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers/insert
        //

        //! Inserts a new element to the position of the document's child node list.
        //! \param pos   the position before which the element is to be inserted.
        //! \param name  the local name of the element to insert.
        //! \return a pointer to the new element.
        //! \throws bad_dom_operation  if the document already has a root element.
        basic_node_ptr<element> insert_element(child_iterator pos, const std::string& name)
        {
            if (root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.insert_element(pos, name);
            }
        }

        //! Inserts a new element to the position of the document's child node list.
        //! \param pos   the position before which the element is to be inserted.
        //! \param name  the local name of the element to insert.
        //! \param uri   the namespace URI of the element to insert.
        //! \return a pointer to the new element.
        //! \throws bad_dom_operation  if the document already has a root element.
        basic_node_ptr<element> insert_element(child_iterator pos,
                                               const std::string& name,
                                               const std::string& uri)
        {
            if (root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.insert_element(pos, name, uri);
            }
        }

        //! Inserts a new comment to the position of the document's child node list.
        //! \param pos   the position before which the comment is to be inserted.
        //! \param value  the content of the comment to insert.
        //! \return a pointer to the new comment.
        basic_node_ptr<comment> insert_comment(child_iterator pos, const std::string& value)
        {
            return children_.insert_comment(pos, value);
        }

        //! Inserts a new processing instruction to the position of the document's child node list.
        //! \param pos     the position before which the processing instruction is to be inserted.
        //! \param target  the target of the processing instruction to insert.
        //! \param value   the content of the processing instruction to insert.
        //! \return a pointer to the new processing instruction.
        basic_node_ptr<xml_pi> insert_xml_pi(child_iterator pos,
                                             const std::string& target,
                                             const std::string& value)
        {
            return children_.insert_xml_pi(pos, target, value);
        }

        //! Clones and inserts the child node to the position of the document's child node list.
        //! \param pos    the position before which the child node is to be inserted.
        //! \param child  the child node to clone and insert.
        //! \return an iterator to the inserted child node.
        //! \throws bad_dom_operation  if the child node cannot be inserted (e.g. the child node is
        //!                            a text or CData node, or the child node is an element while
        //!                            the document already has a root element).
        child_iterator insert_clone(child_iterator pos, const child_node& child)
        {
            if (child.type() == text_node || child.type() == cdata_node)
            {
                throw bad_dom_operation("document does not accept text or CData node");
            }
            else if (child.type() == element_node && root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.insert_clone(pos, child);
            }
        }

        //! Adopts and inserts the child node to the position of the document's child node list.
        //! \param pos    the position before which the child node is to be inserted.
        //! \param child  the child node to clone and insert.
        //! \return an iterator to the inserted child node.
        //! \throws bad_dom_operation  if the child node cannot be inserted (e.g. the child node is
        //!                            a text or CData node, or the child node is an element while
        //!                            the document already has a root element).
        child_iterator insert_adopt(child_iterator pos, child_node& child)
        {
            if (child.type() == text_node || child.type() == cdata_node)
            {
                throw bad_dom_operation("document does not accept text or CData node");
            }
            else if (child.type() == element_node && root_() != 0)
            {
                throw bad_dom_operation("document already has a root element");
            }
            else
            {
                return children_.insert_adopt(pos, child);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // children: modifiers/erase
        //

        child_iterator erase(child_iterator pos)
        {
            return children_.erase(pos);
        }

        child_iterator erase(child_iterator first, child_iterator last)
        {
            return children_.erase(first, last);
        }

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


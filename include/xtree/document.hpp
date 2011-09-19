//
// Created by ZHENG Zhong on 2008-07-17.
//

#ifndef XTREE_DOCUMENT_HPP_20080717__
#define XTREE_DOCUMENT_HPP_20080717__

#include "xtree/config.hpp"
#include "xtree/node.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/types.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/child_node.hpp"
#include "xtree/child_node_list.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <cassert>
#include <memory>
#include <string>


namespace xtree {


    class XTREE_DECL element;
    class XTREE_DECL attribute;
    class XTREE_DECL text;
    class XTREE_DECL comment;
    class XTREE_DECL instruction;

    class XTREE_DECL xpath;
    class XTREE_DECL xpath_result;
    class XTREE_DECL node_set;


    //! This class represents an XML document.
    class XTREE_DECL document: public node
    {

    public:

        typedef child_node_list::iterator        child_iterator;
        typedef child_node_list::const_iterator  const_child_iterator;

    public:

        //! \cond DEV

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Constructor and Destructor
        //! \{

        //! Constructs a document from a libxml2 document. This function should NOT be called by
        //! client code.
        explicit document(xmlDoc* px);

        //! Destroys the DOM tree and releases all resources.
        ~document();

        //! \}

        //! \endcond

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Property Access
        //! \{

        //! Returns the encoding of this document.
        //! \return the encoding of this document.
        std::string encoding() const;

        //! Returns the XML version number of this document.
        //! \return the XML version number of this document.
        std::string version() const;

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Conversion to Pointer
        //! \{

        basic_node_ptr<document> as_ptr()
        {
            return basic_node_ptr<document>(this);
        }

        basic_node_ptr<const document> as_ptr() const
        {
            return basic_node_ptr<const document>(this);
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Root Element Access
        //! \{

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
        //! \param qname  the QName of the element to reset as root.
        //! \param uri    the namespace URI of the element to reset as root.
        //! \return pointer to the new root element of this document.
        //! \throws bad_dom_operation  if $root is not an element.
        basic_node_ptr<element> reset_root(const std::string& qname, const std::string& uri);

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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Capacity
        //! \{

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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Iterators
        //! \{

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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Front and Back
        //! \{

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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Modifiers: push_back
        //! \{

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
        basic_node_ptr<instruction> push_back_instruction(const std::string& target,
                                                          const std::string& value)
        {
            return children_.push_back_instruction(target, value);
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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Modifiers: push_front
        //! \{

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
        basic_node_ptr<instruction> push_front_instruction(const std::string& target,
                                                           const std::string& value)
        {
            return children_.push_front_instruction(target, value);
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

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Modifiers: pop
        //! \{

        void pop_back()
        {
            children_.pop_back();
        }

        void pop_front()
        {
            children_.pop_front();
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Modifiers: insert
        //! \{

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
        basic_node_ptr<instruction> insert_instruction(child_iterator pos,
                                                       const std::string& target,
                                                       const std::string& value)
        {
            return children_.insert_instruction(pos, target, value);
        }

        //! Clones and inserts the child node to the document.
        //! \param pos    the position before which the child node is to be inserted.
        //! \param child  the child node to clone and insert.
        //! \return an iterator to the inserted child node.
        //! \throws bad_dom_operation  if the child node cannot be inserted.
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

        //! Clones and inserts a range of child nodes to the document.
        //! \param pos    the position before which the child nodes are to be inserted.
        //! \param first  iterator to the first child node in the range.
        //! \param last   iterator to the last child node (not included) in the range.
        //! \throws bad_dom_operation  if any of the child nodes cannot be inserted.
        void insert_clone(child_iterator pos, const_child_iterator first, const_child_iterator last)
        {
            int root_count = (root_() != 0 ? 1 : 0);
            for (const_child_iterator i = first; i != last; ++i)
            {
                if (i->type() == text_node || i->type() == cdata_node)
                {
                    throw bad_dom_operation("document does not accept text or CData node");
                }
                else if (i->type() == element_node)
                {
                    if (root_count >= 1)
                    {
                        throw bad_dom_operation("document does not accept more than one root");
                    }
                    else
                    {
                        ++root_count;
                    }
                }
            }
            children_.insert_clone(pos, first, last);
        }

        //! Adopts and inserts the child node to the document.
        //! \param pos    the position before which the child node is to be inserted.
        //! \param child  the child node to clone and insert.
        //! \return an iterator to the inserted child node.
        //! \throws bad_dom_operation  if the child node cannot be inserted.
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

        //! Adopts and inserts a range of child nodes to the document.
        //! \param pos    the position before which the child node is to be inserted.
        //! \param first  iterator to the first child node in the range.
        //! \param last   iterator to the last child node (not included) in the range.
        //! \throws bad_dom_operation  if any of the child nodes cannot be inserted.
        void insert_adopt(child_iterator pos, child_iterator first, child_iterator last)
        {
            int root_count = (root_() != 0 ? 1 : 0);
            for (child_iterator i = first; i != last; ++i)
            {
                if (i->type() == text_node || i->type() == cdata_node)
                {
                    throw bad_dom_operation("document does not accept text or CData node");
                }
                else if (i->type() == element_node)
                {
                    if (root_count >= 1)
                    {
                        throw bad_dom_operation("document does not accept more than one root");
                    }
                    else
                    {
                        ++root_count;
                    }
                }
            }
            children_.insert_adopt(pos, first, last);
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Children Access: Modifiers: erase and clear
        //! \{

        child_iterator erase(child_iterator pos)
        {
            return children_.erase(pos);
        }

        child_iterator erase(child_iterator first, child_iterator last)
        {
            return children_.erase(first, last);
        }

        //! Clears the document. This function destroys the underlying DOM tree and releases all
        //! the resources, thus invalidates all the nodes.
        //! \post empty() == true
        void clear()
        {
            children_.clear();
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Serialization
        //! \{

        //! Saves the whole XML document to a file.
        //! \param file_name  the file name.
        //! \param encoding   the encoding to use.
        void save_to_file(const std::string& file_name,
                          const std::string& encoding = std::string()) const;

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name XPath
        //! \{

        //! Evaluates an XPath expression.
        //! \param expr  the XPath expression.
        //! \return the generic XPath result.
        //! \throws xpath_error  if the XPath expression is invalid.
        void eval(const xpath& expr, xpath_result& result);

        //! Evaluates an XPath expression to a boolean.
        //! \param expr  the XPath expression.
        //! \return the boolean result.
        //! \throws xpath_error  if the XPath expression is invalid.
        bool eval_boolean(const xpath& expr) const;

        //! Evaluates an XPath expression to a number.
        //! \param expr  the XPath expression.
        //! \return the number result.
        //! \throws xpath_error  if the XPath expression is invalid.
        double eval_number(const xpath& expr) const;

        //! Evaluates an XPath expression to a string.
        //! \param expr  the XPath expression.
        //! \return the string result.
        //! \throws xpath_error  if the XPath expression is invalid.
        std::string eval_string(const xpath& expr) const;

        //! Evaluates the XPath expression to a node set. This is an alias to eval().
        //! \param expr   the XPath expression.
        //! \param nodes  the result node set.
        //! \throws xpath_error  if the XPath expression is invalid.
        void select_nodes(const xpath& expr, node_set& nodes);

        //! \}

    public:

        //! \cond DEV

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Underlying Libxml2 Object Access
        //! \{

        xmlDoc* raw_doc()
        {
            return reinterpret_cast<xmlDoc*>(raw());
        }

        const xmlDoc* raw_doc() const
        {
            return reinterpret_cast<const xmlDoc*>(raw());
        }

        //! \}

        //! \endcond

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Private Functions
        //! \{

        //! Returns the root element of this document, or null if this document is empty.
        //! \return the root element of this document, or null if this document is empty.
        const element* root_() const;

        //! Resets the libxml2 element node as the root element of this document.
        //! \post all the original nodes within this document are invalidated.
        //! \param px  the libxml2 element to reset as root, should not be null.
        //! \return pointer to the new root element of this document.
        basic_node_ptr<element> reset_root_(xmlNode* px);

        //! \}

    private:

        child_node_list children_;  //!< The child node list under this document.

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Document Creation Functions
    //! \{


    //! Creates a new and empty XML document.
    //! \return the new XML document.
    XTREE_DECL std::auto_ptr<document> create_document();


    //! Creates a new XML document with a root element.
    //! \param name  the local name of the root element.
    //! \return the new XML document.
    XTREE_DECL std::auto_ptr<document> create_document(const std::string& name);


    //! Creates a new XML document with a root element.
    //! \param qname  the QName of the root element.
    //! \param uri    the namespace URI of the root element.
    //! \return the new XML document.
    XTREE_DECL std::auto_ptr<document> create_document(const std::string& qname,
                                                       const std::string& uri);


    //! Clones an XML document.
    //! \return the cloned document.
    XTREE_DECL std::auto_ptr<document> clone_document(const document& doc);


    //! \}


}  // namespace xtree


#endif  // XTREE_DOCUMENT_HPP_20080717__


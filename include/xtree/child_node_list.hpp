//
// Created by ZHENG Zhong on 2008-07-26.
//

#ifndef XTREE_CHILD_NODE_LIST_HPP_20080726__
#define XTREE_CHILD_NODE_LIST_HPP_20080726__

#include "xtree/config.hpp"
#include "xtree/types.hpp"
#include "xtree/child_node.hpp"
#include "xtree/basic_node_ptr.hpp"
#include "xtree/basic_node_iterator.hpp"
#include "xtree/basic_xmlns_ptr.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <cassert>


namespace xtree {


    class XTREE_DECL document;
    class XTREE_DECL element;
    class XTREE_DECL text;
    class XTREE_DECL comment;
    class XTREE_DECL instruction;
    class XTREE_DECL xmlns;


    //! This class represents a child node list under an element or a document. It may be viewed as
    //! a container of pointers to child_node. This class provides a similar API as std::list.
    class XTREE_DECL child_node_list
    {

    public:

        typedef basic_node_ptr<child_node>            auto_type;

        typedef child_node&                           reference;
        typedef const child_node&                     const_reference;

        typedef basic_node_iterator<child_node>       iterator;
        typedef basic_node_iterator<const child_node> const_iterator;

    public:

        //! \cond DEV

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Constructor and Destructor
        //! \{

        //! Constructs a child_node_list whose owner is a libxml2 document. This function should
        //! NOT be called by client code.
        //! \param px  pointer to libxml2 document who owns the child nodes in this list.
        explicit child_node_list(xmlDoc* px);

        //! Constructs a child_node_list whose owner is a libxml2 element. This function should
        //! NOT be called by client code.
        //! \param px  pointer to libxml2 element who owns the child nodes in this list.
        explicit child_node_list(xmlNode* px);

        //! Destructor. This function should NOT be called by client code.
        ~child_node_list();

        //! \}

        //! \endcond

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Owner Access
        //! \{

        //! Returns the owner document of this child node list.
        //! \return the owner document of this child node list.
        document& doc()
        {
            return const_cast<document&>(doc_());
        }

        //! Const version of doc().
        const document& doc() const
        {
            return doc_();
        }

        //! Returns the owner element of this child node list.
        //! \return the owner element of this child node list.
        basic_node_ptr<element> owner_element()
        {
            return basic_node_ptr<element>(const_cast<element*>(owner_element_()));
        }

        //! Const version of parent().
        basic_node_ptr<const element> owner_element() const
        {
            return basic_node_ptr<const element>(owner_element_());
        }

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Capacity
        //! \{

        //! Checks if this container is empty.
        //! \return true if this container is empty, false otherwise.
        bool empty() const;

        //! Returns the number of items in this list.
        //! \return the number of items in this list.
        size_type size() const;

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Iterators
        //! \{

        //! Returns an iterator to the first child_node of the list.
        //! \return an iterator to the first child_node of the list.
        iterator begin();

        //! Returns an iterator just past the last child_node of the list.
        //! \return an iterator just past the last child_node of the list.
        iterator end();

        //! Const version of begin().
        const_iterator begin() const;

        //! Const version of end().
        const_iterator end() const;

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Front and Back Access
        //! \{

        //! Returns a reference to the first child node of this list.
        //! \pre !empty()
        //! \return a reference to the first child node of this list.
        //! \throws bad_dom_operation  if the list is empty.
        child_node& front();

        //! Const version of front().
        const child_node& front() const;

        //! Returns a reference to the last child node of this list.
        //! \pre !empty()
        //! \return a reference to the last child node of this list.
        //! \throws bad_dom_operation  if the list is empty.
        child_node& back();

        //! Const version of back().
        const child_node& back() const;

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Modifiers: push_back
        //! \{

        //! Pushes an element node to the end of this list.
        //! \post size() is one more.
        //! \param qname  the QName of the element.
        //! \return pointer to the inserted element.
        basic_node_ptr<element> push_back_element(const std::string& qname);

        //! Pushes an element node to the end of this list.
        //! \post size() is one more.
        //! \param qname  the QName of the element.
        //! \param uri    the namespace URI of the element.
        //! \return pointer to the inserted element.
        basic_node_ptr<element> push_back_element(const std::string& qname,
                                                  const std::string& uri);

        //! Pushes an element node to the end of this list.
        //! \post size() is one more.
        //! \param name  the local name of the element.
        //! \param ns    the namespace of the element.
        //! \return pointer to the inserted element.
        basic_node_ptr<element> push_back_element(const std::string& name,
                                                  basic_xmlns_ptr<const xmlns> ns);

        //! Pushes a text node to the end of this list.
        //! \post size() is one more.
        //! \param value  the value of the text.
        //! \return pointer to the inserted text.
        basic_node_ptr<text> push_back_text(const std::string& value);

        //! Pushes a CData node to the end of this list.
        //! \post size() is one more.
        //! \param value  the value of the CData.
        //! \return pointer to the inserted CData.
        basic_node_ptr<text> push_back_cdata(const std::string& value);

        //! Pushes a comment node to the end of this list.
        //! \post size() is one more.
        //! \param value  the value of the comment.
        //! \return pointer to the inserted comment.
        basic_node_ptr<comment> push_back_comment(const std::string& value);

        //! Pushes a processing instruction (PI) node to the end of this list.
        //! \post size() is one more.
        //! \param target  the target of the PI node.
        //! \param value   the value of the PI node.
        //! \return pointer to the inserted PI.
        basic_node_ptr<instruction> push_back_instruction(const std::string& target,
                                                          const std::string& value);

        //! Pushes a child node to the end of this list. This function makes a recursive clone of
        //! the child node and pushes the cloned one.
        //! \param child  the child node to clone and push.
        //! \return pointer to the pushed child node.
        basic_node_ptr<child_node> push_back_clone(const child_node& child);

        //! Pushes a child node to the end of this list. This function takes the ownership of the
        //! child node from the source DOM tree.
        //! \param child  the child node to push.
        //! \return pointer to the pushed child node.
        basic_node_ptr<child_node> push_back_adopt(child_node& child);

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Modifiers: push_front
        //! \{

        //! Pushes an element node to the beginning of this list.
        //! \post size() is one more.
        //! \param qname  the QName of the element.
        //! \return pointer to the inserted element.
        basic_node_ptr<element> push_front_element(const std::string& qname);

        //! Pushes an element node to the beginning of this list.
        //! \post size() is one more.
        //! \param qname  the QName of the element.
        //! \param uri    the namespace URI of the element.
        //! \return pointer to the inserted element.
        basic_node_ptr<element> push_front_element(const std::string& qname,
                                                   const std::string& uri);

        //! Pushes an element node to the beginning of this list.
        //! \post size() is one more.
        //! \param name  the local name of the element.
        //! \param ns    the namespace of the element.
        //! \return pointer to the inserted element.
        basic_node_ptr<element> push_front_element(const std::string& name,
                                                   basic_xmlns_ptr<const xmlns> ns);

        //! Pushes a text node to the beginning of this list.
        //! \post size() is one more.
        //! \param value  the value of the text.
        //! \return pointer to the inserted text.
        basic_node_ptr<text> push_front_text(const std::string& value);

        //! Pushes a CData node to the beginning of this list.
        //! \post size() is one more.
        //! \param value  the value of the CData.
        //! \return pointer to the inserted CData.
        basic_node_ptr<text> push_front_cdata(const std::string& value);

        //! Pushes a comment node to the beginning of this list.
        //! \post size() is one more.
        //! \param value  the value of the comment.
        //! \return pointer to the inserted comment.
        basic_node_ptr<comment> push_front_comment(const std::string& value);

        //! Pushes a processing instruction (PI) node to the beginning of this list.
        //! \post size() is one more.
        //! \param target  the target of the PI node.
        //! \param value   the value of the PI node.
        //! \return pointer to the inserted PI.
        basic_node_ptr<instruction> push_front_instruction(const std::string& target,
                                                           const std::string& value);

        //! Pushes a child node to the front of this list. This function makes a recursive clone of
        //! the child node and pushes the cloned one.
        //! \param child  the child node to clone and push.
        //! \return pointer to the pushed child node.
        basic_node_ptr<child_node> push_front_clone(const child_node& child);

        //! Pushes a child node to the front of this list. This function takes the ownership of the
        //! child node from the source DOM tree.
        //! \param child  the child node to push.
        //! \return pointer to the pushed child node.
        basic_node_ptr<child_node> push_front_adopt(child_node& child);

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Modifiers: pop
        //! \{

        //! Pops the last child node from the list. The child node popped out is destroyed.
        //! \pre  !empty()
        //! \post size() is one less.
        //! \throws bad_dom_operation  if the list is empty.
        void pop_back();

        //! Pops the first child node from the list. The child node popped out is destroyed.
        //! \pre  !empty()
        //! \post size() is one less.
        //! \throws bad_dom_operation  if the list is empty.
        void pop_front();

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Modifiers: insert
        //! \{

        //! Inserts an element node to the node list before the specified position.
        //! \post size() is one more.
        //! \param pos    the position before which the element is to be inserted.
        //! \param qname  the QName of the element.
        //! \return pointer to the inserted element.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        basic_node_ptr<element> insert_element(iterator pos, const std::string& qname);

        //! Inserts an element node to the node list before the specified position.
        //! \post size() is one more.
        //! \param pos    the position before which the element is to be inserted.
        //! \param qname  the QName of the element.
        //! \param uri    the namespace URI of the element.
        //! \return pointer to the inserted element.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        basic_node_ptr<element> insert_element(iterator pos,
                                               const std::string& qname,
                                               const std::string& uri);

        //! Inserts an element node to the node list before the specified position.
        //! \post size() is one more.
        //! \param pos   the position before which the element is to be inserted.
        //! \param name  the local name of the element.
        //! \param ns    the namespace of the element.
        //! \return pointer to the inserted element.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        basic_node_ptr<element> insert_element(iterator pos,
                                               const std::string& name,
                                               basic_xmlns_ptr<const xmlns> ns);

        //! Inserts a text node to the node list before the specified position.
        //! \post size() is one more.
        //! \param pos    the position before which the text is to be inserted.
        //! \param value  the value of the text.
        //! \return pointer to the inserted text.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        basic_node_ptr<text> insert_text(iterator pos, const std::string& value);

        //! Inserts a CData node to the node list before the specified position.
        //! \post size() is one more.
        //! \param pos    the position before which the text is to be inserted.
        //! \param value  the value of the text.
        //! \return pointer to the inserted text.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        basic_node_ptr<text> insert_cdata(iterator pos, const std::string& value);

        //! Inserts a comment node to the node list before the specified position.
        //! \post size() is one more.
        //! \param pos    the position before which the comment is to be inserted.
        //! \param value  the value of the comment.
        //! \return pointer to the inserted comment.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        basic_node_ptr<comment> insert_comment(iterator pos, const std::string& value);

        //! Inserts a processing instruction (PI) node to the node list before the specified
        //! position.
        //! \post size() is one more.
        //! \param pos     the position before which the PI node is to be inserted.
        //! \param target  the target of the PI node.
        //! \param value   the target of the PI node.
        //! \return pointer to the inserted PI.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        basic_node_ptr<instruction> insert_instruction(iterator pos,
                                                       const std::string& target,
                                                       const std::string& value);

        //! Inserts a child node before a position of this list. This function makes a recursive
        //! clone of the child node and inserts the cloned one.
        //! \param pos    the position before which the child node is to be inserted.
        //! \param child  the child node to clone and insert.
        //! \return an iterator to the inserted child node.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        iterator insert_clone(iterator pos, const child_node& child);

        //! Inserts a range of child nodes before a position of this list. This function makes a
        //! recursive clone of the child nodes from the source DOM tree.
        //! \param pos    the position before which the range of child nodes are to be inserted.
        //! \param first  the first child node in the range.
        //! \param last   the last (not including) child node in the range.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        void insert_clone(iterator pos, const_iterator first, const_iterator last);

        //! Inserts a child node before a position of this list. This function takes the ownership
        //! of the child node from the source DOM tree.
        //! \param pos    the position before which the child node is to be inserted.
        //! \param child  the child node to insert.
        //! \return an iterator to the inserted child node.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        iterator insert_adopt(iterator pos, child_node& child);

        //! Inserts a range of child nodes before a position of this list. This function takes the
        //! ownership of the child nodes from the source DOM tree.
        //! \param pos    the position before which the range of child nodes are to be inserted.
        //! \param first  the first child node in the range.
        //! \param last   the last (not including) child node in the range.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        void insert_adopt(iterator pos, iterator first, iterator last);

        //! \}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Modifiers: erase and clear
        //! \{

        //! Removes and destroys the child node at the given position from the list.
        //! \post size() is one less.
        //! \param pos  the position of the child node to erase.
        //! \return an iterator to the child node after the erased one.
        iterator erase(iterator pos);

        //! Removes and destroys all the child nodes between the first position (included) and the
        //! last position (excluded). This function has undefined behavior if the first iterator
        //! cannot reach the last iterator by increasing itself.
        //! \post size() is N less (N is the number of child nodes erased).
        //! \param first  the first position (included) of the child node to erase.
        //! \param last   the last position (excluded) of the child node to erase.
        //! \return an iterator to the child node after the last erased one (thus, ==last).
        iterator erase(iterator first, iterator last);

        //! Removes all the child_node objects from this list.
        //! \post empty() == true
        void clear()
        {
            erase(begin(), end());
        }

        //! \}

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Non-Copyable
        //! \{

        //! Non-implemented copy constructor.
        child_node_list(const child_node_list&);

        //! Non-implemented copy assignment.
        child_node_list& operator=(const child_node_list&);

        //! \}

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \name Private Functions
        //! \{

        //! Returns the owner document of this child node list.
        //! \return the owner document of this child node list.
        const document& doc_() const;

        //! Returns the owner element of this child node list.
        //! \return the owner element of this child node list, or null if it's owned by a document.
        const element* owner_element_() const;

        //! Returns a pointer to the first child_node object, or null if this list is empty.
        const child_node* first_() const;

        //! Checks if the given iterator belongs to this child node list.
        //! \param pos  the iterator to check.
        //! \throws bad_dom_operation  if the iterator does not belong to this child node list.
        void check_ownership_(iterator pos);

        //! Inserts a libxml2 node as a child to the list, before the specified position.
        //! \post size() is one more.
        //! \param pos    the position before which the libxml2 node is to be inserted.
        //! \param child  the libxml2 node to insert.
        //! \return pointer to the inserted libxml2 node.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        xmlNode* insert_(iterator pos, xmlNode* child);

        //! Creates a libxml2 element node.
        xmlNode* create_element_(const std::string& qname);

        //! Creates a libxml2 element node under namespace.
        xmlNode* create_element_(const std::string& qname, const std::string& uri);

        //! Creates a libxml2 element node under namespace.
        xmlNode* create_element_(const std::string& name, basic_xmlns_ptr<const xmlns> ns);

        //! Creates a libxml2 text node.
        xmlNode* create_text_(const std::string& value);

        //! Creates a libxml2 CData node.
        xmlNode* create_cdata_(const std::string& value);

        //! Creates a libxml2 comment node.
        xmlNode* create_comment_(const std::string& value);

        //! Creates a libxml2 processing instruction (PI) node.
        xmlNode* create_instruction_(const std::string& target, const std::string& value);

        //! \}

    private:

        xmlNode* raw_;  //!< The libxml2 element (or document) holding the child node list.

    };


}  // namespace xtree


#endif  // XTREE_CHILD_NODE_LIST_HPP_20080726__


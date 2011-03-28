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
#include "xtree/libxml2_fwd.hpp"

#include <cassert>


namespace xtree {


    class XTREE_DECL document;
    class XTREE_DECL element;
    class XTREE_DECL text;
    class XTREE_DECL comment;
    class XTREE_DECL xml_pi;


    //! This class represents a child node list under an element or a document. It may be viewed as
    //! a container of pointers to child_node. This class provides a similar API as std::list.
    class XTREE_DECL child_node_list
    {

    public:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // typedefs
        //

        typedef basic_node_ptr<child_node>            auto_type;

        typedef child_node&                           reference;
        typedef const child_node&                     const_reference;

        typedef basic_node_iterator<child_node>       iterator;
        typedef basic_node_iterator<const child_node> const_iterator;

    public:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // constructor/destructor
        //

        //! Constructs a child_node_list whose owner is a libxml2 document. This function should
        //! NOT be called by client code.
        //! \param px  pointer to libxml2 document who owns the child nodes in this list.
        explicit child_node_list(xmlDoc* px);

        //! Constructs a child_node_list whose owner is a libxml2 element. This function should
        //! NOT be caleld by client code.
        //! \param px  pointer to libxml2 element who owns the child nodes in this list.
        explicit child_node_list(xmlNode* px);

        //! Destructor. This function should NOT be called by client code.
        ~child_node_list();

        ////////////////////////////////////////////////////////////////////////////////////////////
        // capacity
        //

        //! Checks if this container is empty.
        //! \return true if this container is empty, false otherwise.
        bool empty() const;

        //! Returns the number of items in this list.
        //! \return the number of items in this list.
        size_type size() const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // iterators
        //

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

        ////////////////////////////////////////////////////////////////////////////////////////////
        // child nodes
        //

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

        ////////////////////////////////////////////////////////////////////////////////////////////
        // modifiers/clear
        //

        //! Removes all the child_node objects from this list.
        //! \post empty() == true
        void clear();

        ////////////////////////////////////////////////////////////////////////////////////////////
        // modifiers/push_back
        //

        //! Pushes an element node to the end of this list.
        //! \post size() is one more.
        //! \param name  the name of the element.
        //! \return pointer to the inserted element.
        basic_node_ptr<element> push_back_element(const std::string& name);

        //! Pushes an element node to the end of this list.
        //! \post size() is one more.
        //! \param name  the name of the element.
        //! \param uri   the namespace URI of the element.
        //! \return pointer to the inserted element.
        basic_node_ptr<element> push_back_element(const std::string& name,
                                                  const std::string& uri);

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
        basic_node_ptr<xml_pi> push_back_xml_pi(const std::string& target,
                                                const std::string& value);

        //! Pushes a child node to the end of this list. This function makes a recursive clone of
        //! the child node and pushes the cloned one.
        //! \param child  the child node to clone and push.
        void push_back_clone(const child_node& child);

        //! Pushes a child node to the end of this list. This function takes the ownership of the
        //! child node from the source DOM tree.
        //! \param child  the child node to push.
        void push_back_adopt(child_node& child);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // modifiers/push_front
        //

        //! Pushes an element node to the beginning of this list.
        //! \post size() is one more.
        //! \param name  the name of the element.
        //! \return pointer to the inserted element.
        basic_node_ptr<element> push_front_element(const std::string& name);

        //! Pushes an element node to the beginning of this list.
        //! \post size() is one more.
        //! \param name  the name of the element.
        //! \param uri   the namespace URI of the element.
        //! \return pointer to the inserted element.
        basic_node_ptr<element> push_front_element(const std::string& name,
                                                   const std::string& uri);

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
        basic_node_ptr<xml_pi> push_front_xml_pi(const std::string& target,
                                                 const std::string& value);

        //! Pushes a child node to the front of this list. This function makes a recursive clone of
        //! the child node and pushes the cloned one.
        //! \param child  the child node to clone and push.
        void push_front_clone(const child_node& child);

        //! Pushes a child node to the front of this list. This function takes the ownership of the
        //! child node from the source DOM tree.
        //! \param child  the child node to push.
        void push_front_adopt(child_node& child);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // modifiers/pop_*
        //

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

        ////////////////////////////////////////////////////////////////////////////////////////////
        // modifiers/insert
        //

        //! Inserts an element node to the node list before the specified position.
        //! \post size() is one more.
        //! \param pos   the position before which the element is to be inserted.
        //! \param name  the name of the element.
        //! \return pointer to the inserted element.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        basic_node_ptr<element> insert_element(iterator pos, const std::string& name);

        //! Inserts an element node to the node list before the specified position.
        //! \post size() is one more.
        //! \param pos   the position before which the element is to be inserted.
        //! \param name  the name of the element.
        //! \param uri   the name of the element.
        //! \return pointer to the inserted element.
        //! \throws bad_dom_operation  if $pos does not belong to this list.
        basic_node_ptr<element> insert_element(iterator pos,
                                               const std::string& name,
                                               const std::string& uri);

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
        basic_node_ptr<xml_pi> insert_xml_pi(iterator pos,
                                             const std::string& target,
                                             const std::string& value);

        //! Inserts a child node before a position of this list. This function makes a recursive
        //! clone of the child node and inserts the cloned one.
        //! \param pos    the position before which the child_node object is to be inserted.
        //! \param child  the child node to clone and insert.
        //! \return an iterator to the inserted child_node object.
        iterator insert_clone(iterator pos, const child_node& child);

        //! Inserts a child node before a position of this list. This function takes the ownership
        //! of the child node from the source DOM tree.
        //! \param pos    the position before which the child_node object is to be inserted.
        //! \param child  the child node to insert.
        iterator insert_adopt(iterator pos, child_node& child);


        ////////////////////////////////////////////////////////////////////////////////////////////
        // modifiers/erase
        //

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

    private:

        ////////////////////////////////////////////////////////////////////////////////////////////
        // noncopyable
        //

        //! Non-implemented copy constructor.
        child_node_list(const child_node_list&);

        //! Non-implemented copy assignment.
        child_node_list& operator=(const child_node_list&);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // private functions
        //

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

        //! Creates a libxml2 element node without namespace. Note: after the libxml2 node is
        //! created, this function sets its namespace to null. This is because namespace URI
        //! should be looked up under the context of the element. This function creates a
        //! stand-alone element but does not put the element into the DOM tree, so it is up to the
        //! caller function to set the element's namespace URI properly, after this element has
        //! been put into the DOM tree.
        xmlNode* create_element_(const std::string& name);

        //! Creates a libxml2 text node.
        xmlNode* create_text_(const std::string& value);

        //! Creates a libxml2 CData node.
        xmlNode* create_cdata_(const std::string& value);

        //! Creates a libxml2 comment node.
        xmlNode* create_comment_(const std::string& value);

        //! Creates a libxml2 processing instruction (PI) node.
        xmlNode* create_xml_pi_(const std::string& target, const std::string& value);

    private:

        xmlDoc*  raw_doc_;   //!< The libxml2 document who owns the child node list.
        xmlNode* raw_elem_;  //!< The libxml2 element who owns the child node list.

    };


}  // namespace xtree


#endif  // XTREE_CHILD_NODE_LIST_HPP_20080726__


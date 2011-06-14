//
// Created by ZHENG Zhong on 2008-07-16.
//

#ifndef XTREE_NODE_HPP_20080716__
#define XTREE_NODE_HPP_20080716__

#include "xtree/config.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/types.hpp"
#include "xtree/basic_node_ptr.hpp"

#include "xtree/xml_base.hpp"
#include "xtree/libxml2_fwd.hpp"

#include <cassert>
#include <string>


namespace xtree {


    class XTREE_DECL document;
    class XTREE_DECL element;  // TODO: how to remove this?
    class XTREE_DECL xpath;


    //! This class is the abstract base class representing an XML node. It is non-copyable.
    class XTREE_DECL node: private xml_base
    {

        typedef basic_node_ptr<node>       pointer;
        typedef basic_node_ptr<const node> const_pointer;

    public:

        ///////////////////////////////////////////////////////////////////////////////////////////
        // static
        //

        static node* cast(xmlNode* px)
        {
            return const_cast<node*>(cast_(px));
        }

        static const node* cast(const xmlNode* px)
        {
            return cast_(px);
        }

        //! Deletes the node pointer. This function frees the underlying libxml2 node object, which
        //! in turn deletes the wrapper object. After the function, the node pointer will point to
        //! a deleted memory, so it is up to the caller to reset the pointer to null.
        //! \param ptr  the node pointer to be deleted.
        static void delete_(node* ptr);

    public:

        ///////////////////////////////////////////////////////////////////////////////////////////
        // constructor/destructor
        //

        //! Constructs an object to wrap the underlying libxml2 node object. This function should
        //! NOT be caleld by client code.
        //! \param px  the underlying libxml2 node object to wrap.
        explicit node(xmlNode* px);

        //! Pure virtual destructor. Note: this function does NOT free the underlying libxml2 node
        //! on destruction. On the contrary, the underlying libxml2 node should firstly be freed,
        //! and the wrapper object is then destructed by the callback function.
        virtual ~node() = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////
        // node information access
        //

        //! Returns the type of this node.
        //! \return the type of this node.
        node_t type() const;

        //! Returns the content of this node. This can be either the text carried directly by this
        //! node if it's a text node or comment node; or the aggregate string of the values carried
        //! by this node child's (if it's an attribute or element). Entity references are
        //! substituted.
        //! \return the value of this child node.
        std::string content() const;

        //! Sets the content of this node. Special characters (e.g. &lt;, &gt;, &amp; etc.) will be
        //! escaped automatically.
        //!
        //! For comment or text or CData section, this function sets the text carried directly by
        //! this node. For attribute or element, this function will destroy all its child nodes,
        //! and then add a text node carrying the content as the node's new child.
        //!
        //! Note: for element node, after this function is called, all its original child nodes
        //! will be destroyed and can no longer be used.
        //!
        //! \param content  the content to set.
        void set_content(const std::string& content);

        //! Checks if this node is an orphan node. An orphan node should satisfy the following
        //! conditions:
        //! - it is not the root element of the document.
        //! - it does not have a parent node.
        //! \return true if this node is an orphan node, false otherwise.
        bool is_orphan() const;

        //! Returns the name of this node. For element or attribute, this function returns the
        //! local name of the node.
        //! \return the name of this node.
        std::string name() const;

        //! Returns a structure-based path to this node.
        //! \return a structure-based path to this node.
        std::string path() const;

        //! Returns a string representation (an XML fragment) of this node.
        //! \return a string representation (an XML fragment) of this node.
        std::string str() const;

        //! Retrieves a string representation (an XML fragment) of this node.
        //! \param str  a string to hold the result.
        void str(std::string& str) const;

        ///////////////////////////////////////////////////////////////////////////////////////////
        // owner/parent access
        //

        //! Returns the owner document of this node.
        //! \return the owner document of this node.
        document& doc();

        //! Const version of doc().
        const document& doc() const;

        //! Returns the parent element node.
        //! \return the parent element node.
        basic_node_ptr<element> parent();

        //! Const version of parent().
        basic_node_ptr<const element> parent() const;

    public:

        //! Returns the underlying libxml node. This function should NOT be called by client code.
        xmlNode* raw()
        {
            assert(raw_ != 0 && "underlying libxml2 node should not be null");
            return raw_;
        }

        //! Const version of raw().
        const xmlNode* raw() const
        {
            assert(raw_ != 0 && "underlying libxml2 node should not be null");
            return raw_;
        }

        //! Clones the underlying libxml2 node. This function should NOT be called by client code.
        //! \param recursive  true to make a recursive (deep) clone of the node.
        //! \return the cloned libxml2 node.
        xmlNode* clone_raw(bool recursive) const;

    private:

        ///////////////////////////////////////////////////////////////////////////////////////////
        // private static
        //

        static const node* cast_(const xmlNode* px);

    private:

        //! Non-implemented copy constructor.
        node(const node&);

        //! Non-implemented copy assignment.
        node& operator=(const node&);

        //! Clones a copy of this node. If it is a recursive clone, copies all attributes,
        //! namespace declarations and children when applicable. Otherwise, copies only attributes
        //! and namespace declarations when applicable. This function throws a dom_error if fail to
        //! clone the node.
        //! \param recursive  true to perform a recursive clone, false otherwise.
        //! \return the cloned node object.
        //! \throws dom_error  if fail to clone the node.
        virtual node* clone(bool recursive) const = 0;

        //! Returns a const pointer to the parent element.
        const element* parent_() const;

        const node* prev_sibling_() const;

        const node* next_sibling_() const;

    private:

        xmlNode* raw_;  //!< The underlying libxml2 node object.

    };


}  // namespace xtree


#endif // XTREE_NODE_HPP_20080716__





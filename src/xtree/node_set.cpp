//
// Created by ZHENG Zhong on 2008-07-26.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/node_set.hpp"
#include "xtree/exceptions.hpp"
#include "xtree/node.hpp"

#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>


namespace xtree {


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // node_set_mover
    //


    node_set_mover::node_set_mover(xmlNodeSet* nodes, int index)
    : nodes_(nodes), index_(index)
    {
        // Do nothing.
    }


    void node_set_mover::increment()
    {
        if (nodes_ == 0)
        {
            throw xpath_error("increment() called on empty node set");
        }
        if (index_ >= nodes_->nodeNr)
        {
            throw xpath_error("increment() called at end position");
        }
        ++index_;
    }


    void node_set_mover::decrement()
    {
        if (nodes_ == 0)
        {
            throw xpath_error("decrement() called on empty node set");
        }
        if (index_ == 0)
        {
            throw xpath_error("decrement() called at begin position");
        }
        --index_;
    }


    node* node_set_mover::current() const
    {
        if (nodes_ == 0)
        {
            throw xpath_error("current() called on empty node set");
        }
        if (index_ < 0 || index_ >= nodes_->nodeNr)
        {
            throw xpath_error("current() called on invalid position");
        }
        return static_cast<node*>( nodes_->nodeTab[index_]->_private );
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // node_set_element_mover
    //


    node_set_element_mover::node_set_element_mover(xmlNodeSet* nodes, int index)
    : nodes_(nodes), index_(index)
    {
        if (nodes_ != 0)
        {
            while (index_ < nodes_->nodeNr && nodes_->nodeTab[index_]->type != XML_ELEMENT_NODE)
            {
                ++index_;
            }
        }
    }


    void node_set_element_mover::increment()
    {
        if (nodes_ == 0)
        {
            throw xpath_error("increment() called on empty node set");
        }
        if (index_ >= nodes_->nodeNr)
        {
            throw xpath_error("increment() called at end position");
        }
        do 
        {
            ++index_;
        }
        while (index_ < nodes_->nodeNr && nodes_->nodeTab[index_]->type != XML_ELEMENT_NODE);
    }


    void node_set_element_mover::decrement()
    {
        if (nodes_ == 0)
        {
            throw xpath_error("decrement() called on empty node set");
        }
        int new_index = index_;
        do
        {
            --new_index;
        }
        while (new_index >= 0 && nodes_->nodeTab[new_index]->type != XML_ELEMENT_NODE);
        if (new_index < 0)
        {
            throw xpath_error("decrement() called at begin position");
        }
        index_ = new_index;
    }


    element* node_set_element_mover::current() const
    {
        if (nodes_ == 0)
        {
            throw xpath_error("current() called on empty node set");
        }
        if (index_ < 0 || index_ >= nodes_->nodeNr)
        {
            throw xpath_error("current() called on invalid position");
        }
        return static_cast<element*>( nodes_->nodeTab[index_]->_private );
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // node_set :: helper functions
    //


    namespace {

        template<class Iterator>
        inline Iterator make_iterator_(const xmlXPathObject* raw, bool is_begin)
        {
            const xmlNodeSet* px = (raw != 0 ? raw->nodesetval : 0);
            if (px == 0)
            {
                return Iterator();
            }
            else
            {
                int index = (is_begin ? 0 : px->nodeNr);
                return Iterator(const_cast<xmlNodeSet*>(px), index);
            }
        }

    }  // anonymous namespace


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // node_set
    //


    node_set::node_set()
    {
        // Do nothing.
    }


    bool node_set::empty() const
    {
        const xmlNodeSet* px = (raw() != 0 ? raw()->nodesetval : 0);
        return (px == 0 || px->nodeNr <= 0);
    }


    size_type node_set::size() const
    {
        const xmlNodeSet* px = (raw() != 0 ? raw()->nodesetval : 0);
        if (px == 0 || px->nodeNr <= 0)
        {
            return 0;
        }
        else
        {
            return px->nodeNr;
        }
    }


    node_set::iterator node_set::begin()
    {
        return make_iterator_<iterator>(raw(), true);
    }


    node_set::iterator node_set::end()
    {
        return make_iterator_<iterator>(raw(), false);
    }


    node_set::const_iterator node_set::begin() const
    {
        return make_iterator_<const_iterator>(raw(), true);
    }


    node_set::const_iterator node_set::end() const
    {
        return make_iterator_<const_iterator>(raw(), false);
    }


    node_set::element_iterator node_set::begin_element()
    {
        return make_iterator_<element_iterator>(raw(), true);
    }


    node_set::element_iterator node_set::end_element()
    {
        return make_iterator_<element_iterator>(raw(), false);
    }


    node_set::const_element_iterator node_set::begin_element() const
    {
        return make_iterator_<const_element_iterator>(raw(), true);
    }


    node_set::const_element_iterator node_set::end_element() const
    {
        return make_iterator_<const_element_iterator>(raw(), false);
    }


    bool node_set::is_acceptable_(xpath_result_t type) const
    {
        return (type == node_set_result);
    }


} // namespace xtree


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
    // const_node_set_iterator
    //


    void const_node_set_iterator::increment()
    {
        if (nodes_ == 0)
        {
            throw xpath_error("increment() called on iterator of empty node_set");
        }
        if (index_ >= nodes_->nodeNr)
        {
            throw xpath_error("increment() called at end position");
        }
        ++index_;
    }


    void const_node_set_iterator::decrement()
    {
        if (nodes_ == 0)
        {
            throw xpath_error("decrement() called on iterator of empty node_set");
        }
        if (index_ == 0)
        {
            throw xpath_error("decrement() called at begin position");
        }
        --index_;
    }


    const node* const_node_set_iterator::current() const
    {
        if (nodes_ == 0)
        {
            throw xpath_error("current() called on iterator of empty node_set");
        }
        if (index_ < 0 || index_ >= nodes_->nodeNr)
        {
            throw xpath_error("current() called on invalid iterator");
        }
        return static_cast<const node*>( nodes_->nodeTab[index_]->_private );
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // node_set
    //


    node_set::node_set()
    {
        // Do nothing.
    }


    bool node_set::empty() const
    {
        const xmlNodeSet* px = raw()->nodesetval;
        return (px == 0 || px->nodeNr <= 0);
    }


    size_type node_set::size() const
    {
        const xmlNodeSet* px = raw()->nodesetval;
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
        const xmlNodeSet* px = raw()->nodesetval;
        if (px == 0)
        {
            return iterator();
        }
        else
        {
            return iterator(const_cast<xmlNodeSet*>(px), 0);
        }
    }


    node_set::iterator node_set::end()
    {
        const xmlNodeSet* px = raw()->nodesetval;
        if (px == 0)
        {
            return iterator();
        }
        else
        {
            return iterator(const_cast<xmlNodeSet*>(px), px->nodeNr);
        }
    }


    node_set::const_iterator node_set::begin() const
    {
        const xmlNodeSet* px = raw()->nodesetval;
        if (px == 0)
        {
            return const_iterator();
        }
        else
        {
            return const_iterator(const_cast<xmlNodeSet*>(px), 0);
        }
    }


    node_set::const_iterator node_set::end() const
    {
        const xmlNodeSet* px = raw()->nodesetval;
        if (px == 0)
        {
            return const_iterator();
        }
        else
        {
            return const_iterator(const_cast<xmlNodeSet*>(px), px->nodeNr);
        }
    }


    void node_set::check_acceptable(const xpath_result& result)
    {
        const xmlXPathObject* px = result.raw();
        if (px == 0)
        {
            throw xpath_error("fail to convert empty xpath_result to node_set");
        }
        if (px->type != XPATH_NODESET)
        {
            throw xpath_error(result.xpath() + " does not evaluate to a node_set");
        }
    }


} // namespace xtree


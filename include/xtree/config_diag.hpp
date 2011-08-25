//
// Created by ZHENG Zhong on 2011-06-07.
//

#ifndef XTREE_CONFIG_DIAG_HPP_20110607__
#define XTREE_CONFIG_DIAG_HPP_20110607__

#include "xtree/config.hpp"


#define XTREE_CONFIG_DIAG_STR_(x)  #x
#define XTREE_CONFIG_DIAG_STR(x)   XTREE_CONFIG_DIAG_STR_(x)


#if defined(XTREE_MSVC)
#  pragma message("[xtree config] compiled using Microsoft Visual C++")
#elif defined(XTREE_GNUC)
#  pragma message("[xtree config] compiled using GNU C compiler")
#else
#  pragma message("[xtree config] compiled using unknown compiler")
#endif


#ifdef XTREE_DEBUG
#  pragma message("[xtree config] xtree debug flag is on")
#else
#  pragma message("[xtree config] xtree debug flag is off")
#endif


#if defined(XTREE_STATIC_LIB)
#  pragma message("[xtree config] using xtree static library")
#elif defined(XTREE_SHARED_LIB)
#  pragma message("[xtree config] using xtree shared library")
#endif


#ifdef XTREE_DECL
#  define XTREE_DECL_STR [XTREE_DECL]
#  pragma message("[xtree config] XTREE_DECL is defined to " XTREE_CONFIG_DIAG_STR(XTREE_DECL_STR))
#  undef XTREE_DECL_STR
#else
#  pragma message("[xtree config] XTREE_DECL is undefined")
#endif


#ifdef LIBXML_STATIC
#  pragma message("[xtree config] libxml2: LIBXML_STATIC is defined")
#else
#  pragma message("[xtree config] libxml2: LIBXML_STATIC is undefined")
#endif


#ifdef LIBXSLT_STATIC
#  pragma message("[xtree config] libxml2: LIBXSLT_STATIC is defined")
#else
#  pragma message("[xtree config] libxml2: LIBXSLT_STATIC is undefined")
#endif


#ifdef LIBEXSLT_STATIC
#  pragma message("[xtree config] libxml2: LIBEXSLT_STATIC is defined")
#else
#  pragma message("[xtree config] libxml2: LIBEXSLT_STATIC is undefined")
#endif


#ifdef XMLSEC_STATIC
#  pragma message("[xtree config] libxml2: XMLSEC_STATIC is defined")
#else
#  pragma message("[xtree config] libxml2: XMLSEC_STATIC is undefined")
#endif


#undef XTREE_CONFIG_DIAG_STR
#undef XTREE_CONFIG_DIAG_STR_


#endif  // XTREE_CONFIG_DIAG_HPP_20110607__


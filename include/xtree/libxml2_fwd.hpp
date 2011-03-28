//
// Created by ZHENG Zhong on 2008-07-26.
//

#ifndef XTREE_LIBXML2_FWD_HPP_20080726__
#define XTREE_LIBXML2_FWD_HPP_20080726__

#include "xtree/config.hpp"


#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned char xmlChar;

struct _xmlError;
struct _xmlNode;
struct _xmlNs;
struct _xmlDoc;
struct _xmlSchema;
struct _xmlXPathContext;
struct _xmlXPathObject;
struct _xmlNodeSet;

typedef struct _xmlError         xmlError;
typedef struct _xmlNode          xmlNode;
typedef struct _xmlNs            xmlNs;
typedef struct _xmlDoc           xmlDoc;
typedef struct _xmlSchema        xmlSchema;
typedef struct _xmlXPathContext  xmlXPathContext;
typedef struct _xmlXPathObject   xmlXPathObject;
typedef struct _xmlNodeSet       xmlNodeSet;


#ifdef __cplusplus
}  // extern "C"
#endif


#endif // XTREE_LIBXML2_FWD_HPP_20080726__


//
// Created by ZHENG Zhong on 2008-07-16.
//

#ifndef XTREE_SOURCE
#define XTREE_SOURCE
#endif

#include "xtree/libxml2_utility.hpp"

#include <libxml/xmlerror.h>

#include <sstream>
#include <string>


namespace xtree {
namespace detail {


    namespace {

        //! Converts domain code to string.
        std::string domain_code_to_string(int domain)
        {
            std::ostringstream oss;
            switch (domain)
            {
            case XML_FROM_NONE:
                oss << "None domain";
                break;
            case XML_FROM_PARSER:
                oss << "XML parser error";
                break;
            case XML_FROM_TREE:
                oss << "DOM tree error";
                break;
            case XML_FROM_NAMESPACE:
                oss << "Namespace error";
                break;
            case XML_FROM_DTD:
                oss << "DTD error";
                break;
            case XML_FROM_HTML:
                oss << "HTML parser error";
                break;
            case XML_FROM_MEMORY:
                oss << "Memory allocator error";
                break;
            case XML_FROM_OUTPUT:
                oss << "Serialization error";
                break;
            case XML_FROM_IO:
                oss << "Input/Output error";
                break;
            case XML_FROM_FTP:
                oss << "FTP error";
                break;
            case XML_FROM_HTTP:
                oss << "HTTP error";
                break;
            case XML_FROM_XINCLUDE:
                oss << "XInclude error";
                break;
            case XML_FROM_XPATH:
                oss << "XPath error";
                break;
            case XML_FROM_XPOINTER:
                oss << "XPointer error";
                break;
            case XML_FROM_REGEXP:
                oss << "Regular expression error";
                break;
            case XML_FROM_DATATYPE:
                oss << "W3C XML Schemas Datatype error";
                break;
            case XML_FROM_SCHEMASP:
                oss << "W3C XML Schemas parser error";
                break;
            case XML_FROM_SCHEMASV:
                oss << "W3C XML Schemas validation error";
                break;
            case XML_FROM_RELAXNGP:
                oss << "Relax-NG parser error";
                break;
            case XML_FROM_RELAXNGV:
                oss << "Relax-NG validator error";
                break;
            case XML_FROM_CATALOG:
                oss << "Catalog error";
                break;
            case XML_FROM_C14N:
                oss << "Canonicalization error";
                break;
            case XML_FROM_XSLT:
                oss << "XSLT error";
                break;
            case XML_FROM_VALID:
                oss << "DTD validation error";
                break;
            case XML_FROM_CHECK:
                oss << "Error checking error";
                break;
            case XML_FROM_WRITER:
                oss << "XML writer error";
                break;
            case XML_FROM_MODULE:
                oss << "Dynamically loaded module error";
                break;
            case XML_FROM_I18N:
                oss << "I18N character conversion error";
                break;
            default:
                oss << "Unknown domain";
                break;
            }
            oss << " (domain=" << domain << ")";
            return oss.str();
        }


    }  // anonymous namespace


    ////////////////////////////////////////////////////////////////////////////////////////////////


    std::string error_code_to_string(int code)
    {
        std::ostringstream oss;
        switch (code)
        {
        case XML_ERR_OK:
            oss << "no error";
            break;
        case XML_ERR_INTERNAL_ERROR:
            oss << "libxml2 internal error";
            break;
        case XML_ERR_NO_MEMORY:
            oss << "no memory";
            break;
        /*
        case XML_ERR_DOCUMENT_START = 3 : 3
        case XML_ERR_DOCUMENT_EMPTY = 4 : 4
        case XML_ERR_DOCUMENT_END = 5 : 5
        case XML_ERR_INVALID_HEX_CHARREF = 6 : 6
        case XML_ERR_INVALID_DEC_CHARREF = 7 : 7
        case XML_ERR_INVALID_CHARREF = 8 : 8
        case XML_ERR_INVALID_CHAR = 9 : 9
        case XML_ERR_CHARREF_AT_EOF = 10 : 10
        case XML_ERR_CHARREF_IN_PROLOG = 11 : 11
        case XML_ERR_CHARREF_IN_EPILOG = 12 : 12
        case XML_ERR_CHARREF_IN_DTD = 13 : 13
        case XML_ERR_ENTITYREF_AT_EOF = 14 : 14
        case XML_ERR_ENTITYREF_IN_PROLOG = 15 : 15
        case XML_ERR_ENTITYREF_IN_EPILOG = 16 : 16
        case XML_ERR_ENTITYREF_IN_DTD = 17 : 17
        case XML_ERR_PEREF_AT_EOF = 18 : 18
        case XML_ERR_PEREF_IN_PROLOG = 19 : 19
        case XML_ERR_PEREF_IN_EPILOG = 20 : 20
        case XML_ERR_PEREF_IN_INT_SUBSET = 21 : 21
        case XML_ERR_ENTITYREF_NO_NAME = 22 : 22
        case XML_ERR_ENTITYREF_SEMICOL_MISSING = 23 : 23
        case XML_ERR_PEREF_NO_NAME = 24 : 24
        case XML_ERR_PEREF_SEMICOL_MISSING = 25 : 25
        case XML_ERR_UNDECLARED_ENTITY = 26 : 26
        case XML_WAR_UNDECLARED_ENTITY = 27 : 27
        case XML_ERR_UNPARSED_ENTITY = 28 : 28
        case XML_ERR_ENTITY_IS_EXTERNAL = 29 : 29
        case XML_ERR_ENTITY_IS_PARAMETER = 30 : 30
        case XML_ERR_UNKNOWN_ENCODING = 31 : 31
        case XML_ERR_UNSUPPORTED_ENCODING = 32 : 32
        case XML_ERR_STRING_NOT_STARTED = 33 : 33
        case XML_ERR_STRING_NOT_CLOSED = 34 : 34
        case XML_ERR_NS_DECL_ERROR = 35 : 35
        case XML_ERR_ENTITY_NOT_STARTED = 36 : 36
        case XML_ERR_ENTITY_NOT_FINISHED = 37 : 37
        case XML_ERR_LT_IN_ATTRIBUTE = 38 : 38
        case XML_ERR_ATTRIBUTE_NOT_STARTED = 39 : 39
        case XML_ERR_ATTRIBUTE_NOT_FINISHED = 40 : 40
        case XML_ERR_ATTRIBUTE_WITHOUT_VALUE = 41 : 41
        case XML_ERR_ATTRIBUTE_REDEFINED = 42 : 42
        case XML_ERR_LITERAL_NOT_STARTED = 43 : 43
        case XML_ERR_LITERAL_NOT_FINISHED = 44 : 44
        case XML_ERR_COMMENT_NOT_FINISHED = 45 : 45
        case XML_ERR_PI_NOT_STARTED = 46 : 46
        case XML_ERR_PI_NOT_FINISHED = 47 : 47
        case XML_ERR_NOTATION_NOT_STARTED = 48 : 48
        case XML_ERR_NOTATION_NOT_FINISHED = 49 : 49
        case XML_ERR_ATTLIST_NOT_STARTED = 50 : 50
        case XML_ERR_ATTLIST_NOT_FINISHED = 51 : 51
        case XML_ERR_MIXED_NOT_STARTED = 52 : 52
        case XML_ERR_MIXED_NOT_FINISHED = 53 : 53
        case XML_ERR_ELEMCONTENT_NOT_STARTED = 54 : 54
        case XML_ERR_ELEMCONTENT_NOT_FINISHED = 55 : 55
        case XML_ERR_XMLDECL_NOT_STARTED = 56 : 56
        case XML_ERR_XMLDECL_NOT_FINISHED = 57 : 57
        case XML_ERR_CONDSEC_NOT_STARTED = 58 : 58
        case XML_ERR_CONDSEC_NOT_FINISHED = 59 : 59
        case XML_ERR_EXT_SUBSET_NOT_FINISHED = 60 : 60
        case XML_ERR_DOCTYPE_NOT_FINISHED = 61 : 61
        case XML_ERR_MISPLACED_CDATA_END = 62 : 62
        case XML_ERR_CDATA_NOT_FINISHED = 63 : 63
        case XML_ERR_RESERVED_case XML_NAME = 64 : 64
        case XML_ERR_SPACE_REQUIRED = 65 : 65
        case XML_ERR_SEPARATOR_REQUIRED = 66 : 66
        case XML_ERR_NMTOKEN_REQUIRED = 67 : 67
        case XML_ERR_NAME_REQUIRED = 68 : 68
        case XML_ERR_PCDATA_REQUIRED = 69 : 69
        case XML_ERR_URI_REQUIRED = 70 : 70
        case XML_ERR_PUBID_REQUIRED = 71 : 71
        case XML_ERR_LT_REQUIRED = 72 : 72
        case XML_ERR_GT_REQUIRED = 73 : 73
        case XML_ERR_LTSLASH_REQUIRED = 74 : 74
        case XML_ERR_EQUAL_REQUIRED = 75 : 75
        case XML_ERR_TAG_NAME_MISMATCH = 76 : 76
        case XML_ERR_TAG_NOT_FINISHED = 77 : 77
        case XML_ERR_STANDALONE_VALUE = 78 : 78
        case XML_ERR_ENCODING_NAME = 79 : 79
        case XML_ERR_HYPHEN_IN_COMMENT = 80 : 80
        case XML_ERR_INVALID_ENCODING = 81 : 81
        case XML_ERR_EXT_ENTITY_STANDALONE = 82 : 82
        case XML_ERR_CONDSEC_INVALID = 83 : 83
        case XML_ERR_VALUE_REQUIRED = 84 : 84
        case XML_ERR_NOT_WELL_BALANCED = 85 : 85
        case XML_ERR_EXTRA_CONTENT = 86 : 86
        case XML_ERR_ENTITY_CHAR_ERROR = 87 : 87
        case XML_ERR_ENTITY_PE_INTERNAL = 88 : 88
        case XML_ERR_ENTITY_LOOP = 89 : 89
        case XML_ERR_ENTITY_BOUNDARY = 90 : 90
        case XML_ERR_INVALID_URI = 91 : 91
        case XML_ERR_URI_FRAGMENT = 92 : 92
        case XML_WAR_CATALOG_PI = 93 : 93
        case XML_ERR_NO_DTD = 94 : 94
        case XML_ERR_CONDSEC_INVALID_KEYWORD = 95 : 95
        case XML_ERR_VERSION_MISSING = 96 : 96
        case XML_WAR_UNKNOWN_VERSION = 97 : 97
        case XML_WAR_LANG_VALUE = 98 : 98
        case XML_WAR_NS_URI = 99 : 99
        case XML_WAR_NS_URI_RELATIVE = 100 : 100
        case XML_ERR_MISSING_ENCODING = 101 : 101
        case XML_WAR_SPACE_VALUE = 102 : 102
        case XML_ERR_NOT_STANDALONE = 103 : 103
        case XML_ERR_ENTITY_PROCESSING = 104 : 104
        case XML_ERR_NOTATION_PROCESSING = 105 : 105
        case XML_WAR_NS_COLUMN = 106 : 106
        case XML_WAR_ENTITY_REDEFINED = 107 : 107
        case XML_NS_ERR_XML_NAMESPACE = 200
        case XML_NS_ERR_UNDEFINED_NAMESPACE = 201 : 201
        case XML_NS_ERR_QNAME = 202 : 202
        */

        case XML_NS_ERR_ATTRIBUTE_REDEFINED:
            oss << "namespaced attribute redefined";
            break;

        /*
        case XML_NS_ERR_EMPTY = 204 : 204
        case XML_DTD_ATTRIBUTE_DEFAULT = 500
        case XML_DTD_ATTRIBUTE_REDEFINED = 501 : 501
        case XML_DTD_ATTRIBUTE_VALUE = 502 : 502
        case XML_DTD_CONTENT_ERROR = 503 : 503
        case XML_DTD_CONTENT_MODEL = 504 : 504
        case XML_DTD_CONTENT_NOT_DETERMINIST = 505 : 505
        case XML_DTD_DIFFERENT_PREFIX = 506 : 506
        case XML_DTD_ELEM_DEFAULT_NAMESPACE = 507 : 507
        case XML_DTD_ELEM_NAMESPACE = 508 : 508
        case XML_DTD_ELEM_REDEFINED = 509 : 509
        case XML_DTD_EMPTY_NOTATION = 510 : 510
        case XML_DTD_ENTITY_TYPE = 511 : 511
        case XML_DTD_ID_FIXED = 512 : 512
        case XML_DTD_ID_REDEFINED = 513 : 513
        case XML_DTD_ID_SUBSET = 514 : 514
        case XML_DTD_INVALID_CHILD = 515 : 515
        case XML_DTD_INVALID_DEFAULT = 516 : 516
        case XML_DTD_LOAD_ERROR = 517 : 517
        case XML_DTD_MISSING_ATTRIBUTE = 518 : 518
        case XML_DTD_MIXED_CORRUPT = 519 : 519
        case XML_DTD_MULTIPLE_ID = 520 : 520
        case XML_DTD_NO_DOC = 521 : 521
        case XML_DTD_NO_DTD = 522 : 522
        case XML_DTD_NO_ELEM_NAME = 523 : 523
        case XML_DTD_NO_PREFIX = 524 : 524
        case XML_DTD_NO_ROOT = 525 : 525
        case XML_DTD_NOTATION_REDEFINED = 526 : 526
        case XML_DTD_NOTATION_VALUE = 527 : 527
        case XML_DTD_NOT_EMPTY = 528 : 528
        case XML_DTD_NOT_PCDATA = 529 : 529
        case XML_DTD_NOT_STANDALONE = 530 : 530
        case XML_DTD_ROOT_NAME = 531 : 531
        case XML_DTD_STANDALONE_WHITE_SPACE = 532 : 532
        case XML_DTD_UNKNOWN_ATTRIBUTE = 533 : 533
        case XML_DTD_UNKNOWN_ELEM = 534 : 534
        case XML_DTD_UNKNOWN_ENTITY = 535 : 535
        case XML_DTD_UNKNOWN_ID = 536 : 536
        case XML_DTD_UNKNOWN_NOTATION = 537 : 537
        case XML_DTD_STANDALONE_DEFAULTED = 538 : 538
        case XML_DTD_XMLID_VALUE = 539 : 539
        case XML_DTD_XMLID_TYPE = 540 : 540
        case XML_HTML_STRUCURE_ERROR = 800
        case XML_HTML_UNKNOWN_TAG = 801 : 801
        case XML_RNGP_ANYNAME_ATTR_ANCESTOR = 1000
        case XML_RNGP_ATTR_CONFLICT = 1001 : 1001
        case XML_RNGP_ATTRIBUTE_CHILDREN = 1002 : 1002
        case XML_RNGP_ATTRIBUTE_CONTENT = 1003 : 1003
        case XML_RNGP_ATTRIBUTE_EMPTY = 1004 : 1004
        case XML_RNGP_ATTRIBUTE_NOOP = 1005 : 1005
        case XML_RNGP_CHOICE_CONTENT = 1006 : 1006
        case XML_RNGP_CHOICE_EMPTY = 1007 : 1007
        case XML_RNGP_CREATE_FAILURE = 1008 : 1008
        case XML_RNGP_DATA_CONTENT = 1009 : 1009
        case XML_RNGP_DEF_CHOICE_AND_INTERLEAVE = 1010 : 1010
        case XML_RNGP_DEFINE_CREATE_FAILED = 1011 : 1011
        case XML_RNGP_DEFINE_EMPTY = 1012 : 1012
        case XML_RNGP_DEFINE_MISSING = 1013 : 1013
        case XML_RNGP_DEFINE_NAME_MISSING = 1014 : 1014
        case XML_RNGP_ELEM_CONTENT_EMPTY = 1015 : 1015
        case XML_RNGP_ELEM_CONTENT_ERROR = 1016 : 1016
        case XML_RNGP_ELEMENT_EMPTY = 1017 : 1017
        case XML_RNGP_ELEMENT_CONTENT = 1018 : 1018
        case XML_RNGP_ELEMENT_NAME = 1019 : 1019
        case XML_RNGP_ELEMENT_NO_CONTENT = 1020 : 1020
        case XML_RNGP_ELEM_TEXT_CONFLICT = 1021 : 1021
        case XML_RNGP_EMPTY = 1022 : 1022
        case XML_RNGP_EMPTY_CONSTRUCT = 1023 : 1023
        case XML_RNGP_EMPTY_CONTENT = 1024 : 1024
        case XML_RNGP_EMPTY_NOT_EMPTY = 1025 : 1025
        case XML_RNGP_ERROR_TYPE_LIB = 1026 : 1026
        case XML_RNGP_EXCEPT_EMPTY = 1027 : 1027
        case XML_RNGP_EXCEPT_MISSING = 1028 : 1028
        case XML_RNGP_EXCEPT_MULTIPLE = 1029 : 1029
        case XML_RNGP_EXCEPT_NO_CONTENT = 1030 : 1030
        case XML_RNGP_EXTERNALREF_EMTPY = 1031 : 1031
        case XML_RNGP_EXTERNAL_REF_FAILURE = 1032 : 1032
        case XML_RNGP_EXTERNALREF_RECURSE = 1033 : 1033
        case XML_RNGP_FORBIDDEN_ATTRIBUTE = 1034 : 1034
        case XML_RNGP_FOREIGN_ELEMENT = 1035 : 1035
        case XML_RNGP_GRAMMAR_CONTENT = 1036 : 1036
        case XML_RNGP_GRAMMAR_EMPTY = 1037 : 1037
        case XML_RNGP_GRAMMAR_MISSING = 1038 : 1038
        case XML_RNGP_GRAMMAR_NO_START = 1039 : 1039
        case XML_RNGP_GROUP_ATTR_CONFLICT = 1040 : 1040
        case XML_RNGP_HREF_ERROR = 1041 : 1041
        case XML_RNGP_INCLUDE_EMPTY = 1042 : 1042
        case XML_RNGP_INCLUDE_FAILURE = 1043 : 1043
        case XML_RNGP_INCLUDE_RECURSE = 1044 : 1044
        case XML_RNGP_INTERLEAVE_ADD = 1045 : 1045
        case XML_RNGP_INTERLEAVE_CREATE_FAILED = 1046 : 1046
        case XML_RNGP_INTERLEAVE_EMPTY = 1047 : 1047
        case XML_RNGP_INTERLEAVE_NO_CONTENT = 1048 : 1048
        case XML_RNGP_INVALID_DEFINE_NAME = 1049 : 1049
        case XML_RNGP_INVALID_URI = 1050 : 1050
        case XML_RNGP_INVALID_VALUE = 1051 : 1051
        case XML_RNGP_MISSING_HREF = 1052 : 1052
        case XML_RNGP_NAME_MISSING = 1053 : 1053
        case XML_RNGP_NEED_COMBINE = 1054 : 1054
        case XML_RNGP_NOTALLOWED_NOT_EMPTY = 1055 : 1055
        case XML_RNGP_NSNAME_ATTR_ANCESTOR = 1056 : 1056
        case XML_RNGP_NSNAME_NO_NS = 1057 : 1057
        case XML_RNGP_PARAM_FORBIDDEN = 1058 : 1058
        case XML_RNGP_PARAM_NAME_MISSING = 1059 : 1059
        case XML_RNGP_PARENTREF_CREATE_FAILED = 1060 : 1060
        case XML_RNGP_PARENTREF_NAME_INVALID = 1061 : 1061
        case XML_RNGP_PARENTREF_NO_NAME = 1062 : 1062
        case XML_RNGP_PARENTREF_NO_PARENT = 1063 : 1063
        case XML_RNGP_PARENTREF_NOT_EMPTY = 1064 : 1064
        case XML_RNGP_PARSE_ERROR = 1065 : 1065
        case XML_RNGP_PAT_ANYNAME_EXCEPT_ANYNAME = 1066 : 1066
        case XML_RNGP_PAT_ATTR_ATTR = 1067 : 1067
        case XML_RNGP_PAT_ATTR_ELEM = 1068 : 1068
        case XML_RNGP_PAT_DATA_EXCEPT_ATTR = 1069 : 1069
        case XML_RNGP_PAT_DATA_EXCEPT_ELEM = 1070 : 1070
        case XML_RNGP_PAT_DATA_EXCEPT_EMPTY = 1071 : 1071
        case XML_RNGP_PAT_DATA_EXCEPT_GROUP = 1072 : 1072
        case XML_RNGP_PAT_DATA_EXCEPT_INTERLEAVE = 1073 : 1073
        case XML_RNGP_PAT_DATA_EXCEPT_LIST = 1074 : 1074
        case XML_RNGP_PAT_DATA_EXCEPT_ONEMORE = 1075 : 1075
        case XML_RNGP_PAT_DATA_EXCEPT_REF = 1076 : 1076
        case XML_RNGP_PAT_DATA_EXCEPT_TEXT = 1077 : 1077
        case XML_RNGP_PAT_LIST_ATTR = 1078 : 1078
        case XML_RNGP_PAT_LIST_ELEM = 1079 : 1079
        case XML_RNGP_PAT_LIST_INTERLEAVE = 1080 : 1080
        case XML_RNGP_PAT_LIST_LIST = 1081 : 1081
        case XML_RNGP_PAT_LIST_REF = 1082 : 1082
        case XML_RNGP_PAT_LIST_TEXT = 1083 : 1083
        case XML_RNGP_PAT_NSNAME_EXCEPT_ANYNAME = 1084 : 1084
        case XML_RNGP_PAT_NSNAME_EXCEPT_NSNAME = 1085 : 1085
        case XML_RNGP_PAT_ONEMORE_GROUP_ATTR = 1086 : 1086
        case XML_RNGP_PAT_ONEMORE_INTERLEAVE_ATTR = 1087 : 1087
        case XML_RNGP_PAT_START_ATTR = 1088 : 1088
        case XML_RNGP_PAT_START_DATA = 1089 : 1089
        case XML_RNGP_PAT_START_EMPTY = 1090 : 1090
        case XML_RNGP_PAT_START_GROUP = 1091 : 1091
        case XML_RNGP_PAT_START_INTERLEAVE = 1092 : 1092
        case XML_RNGP_PAT_START_LIST = 1093 : 1093
        case XML_RNGP_PAT_START_ONEMORE = 1094 : 1094
        case XML_RNGP_PAT_START_TEXT = 1095 : 1095
        case XML_RNGP_PAT_START_VALUE = 1096 : 1096
        case XML_RNGP_PREFIX_UNDEFINED = 1097 : 1097
        case XML_RNGP_REF_CREATE_FAILED = 1098 : 1098
        case XML_RNGP_REF_CYCLE = 1099 : 1099
        case XML_RNGP_REF_NAME_INVALID = 1100 : 1100
        case XML_RNGP_REF_NO_DEF = 1101 : 1101
        case XML_RNGP_REF_NO_NAME = 1102 : 1102
        case XML_RNGP_REF_NOT_EMPTY = 1103 : 1103
        case XML_RNGP_START_CHOICE_AND_INTERLEAVE = 1104 : 1104
        case XML_RNGP_START_CONTENT = 1105 : 1105
        case XML_RNGP_START_EMPTY = 1106 : 1106
        case XML_RNGP_START_MISSING = 1107 : 1107
        case XML_RNGP_TEXT_EXPECTED = 1108 : 1108
        case XML_RNGP_TEXT_HAS_CHILD = 1109 : 1109
        case XML_RNGP_TYPE_MISSING = 1110 : 1110
        case XML_RNGP_TYPE_NOT_FOUND = 1111 : 1111
        case XML_RNGP_TYPE_VALUE = 1112 : 1112
        case XML_RNGP_UNKNOWN_ATTRIBUTE = 1113 : 1113
        case XML_RNGP_UNKNOWN_COMBINE = 1114 : 1114
        case XML_RNGP_UNKNOWN_CONSTRUCT = 1115 : 1115
        case XML_RNGP_UNKNOWN_TYPE_LIB = 1116 : 1116
        case XML_RNGP_URI_FRAGMENT = 1117 : 1117
        case XML_RNGP_URI_NOT_ABSOLUTE = 1118 : 1118
        case XML_RNGP_VALUE_EMPTY = 1119 : 1119
        case XML_RNGP_VALUE_NO_CONTENT = 1120 : 1120
        case XML_RNGP_XMLNS_NAME = 1121 : 1121
        case XML_RNGP_XML_NS = 1122 : 1122
        */
        case XML_XPATH_EXPRESSION_OK:
            oss << "XPath expression OK";
            break;
        case XML_XPATH_NUMBER_ERROR:
            oss << "XPath number error";
            break;
        case XML_XPATH_UNFINISHED_LITERAL_ERROR:
            oss << "Unfinished literal in XPath";
            break;
        case XML_XPATH_START_LITERAL_ERROR:
            oss << "Invalid start literal in XPath";
            break;
        case XML_XPATH_VARIABLE_REF_ERROR:
            oss << "Invalid variable reference in XPath";
            break;
        case XML_XPATH_UNDEF_VARIABLE_ERROR:
            oss << "Undefined variable in XPath";
            break;
        case XML_XPATH_INVALID_PREDICATE_ERROR:
            oss << "Invalid predicate in XPath";
            break;
        case XML_XPATH_EXPR_ERROR:
            oss << "Invalid XPath expression";
            break;
        case XML_XPATH_UNCLOSED_ERROR:
            oss << "Unclosed XPath";
            break;
        case XML_XPATH_UNKNOWN_FUNC_ERROR:
            oss << "Unknown function in XPath";
            break;
        case XML_XPATH_INVALID_OPERAND:
            oss << "Invalid operand in XPath";
            break;
        case XML_XPATH_INVALID_TYPE:
            oss << "Invalid type in XPath";
            break;
        case XML_XPATH_INVALID_ARITY:
            oss << "Invalid arity in XPath";
            break;
        case XML_XPATH_INVALID_CTXT_SIZE:
            oss << "Invalid context size in XPath";
            break;
        case XML_XPATH_INVALID_CTXT_POSITION:
            oss << "Invalid context position in XPath";
            break;
        case XML_XPATH_MEMORY_ERROR:
            oss << "XPath memory error";
            break;
        case XML_XPTR_SYNTAX_ERROR:
            oss << "Invalid XPointer syntax";
            break;
        case XML_XPTR_RESOURCE_ERROR:
            oss << "XPointer resource error";
            break;
        case XML_XPTR_SUB_RESOURCE_ERROR:
            oss << "XPointer sub resource error";
            break;
        case XML_XPATH_UNDEF_PREFIX_ERROR:
            oss << "Undefined prefix in XPath";
            break;
        case XML_XPATH_ENCODING_ERROR:
            oss << "Invalid XPath encoding";
            break;
        case XML_XPATH_INVALID_CHAR_ERROR:
            oss << "Invalid character in XPath";
            break;
        /*
        case XML_TREE_INVALID_HEX = 1300
        case XML_TREE_INVALID_DEC = 1301 : 1301
        case XML_TREE_UNTERMINATED_ENTITY = 1302 : 1302
        case XML_TREE_NOT_UTF8 = 1303 : 1303
        case XML_SAVE_NOT_UTF8 = 1400
        case XML_SAVE_CHAR_INVALID = 1401 : 1401
        case XML_SAVE_NO_DOCTYPE = 1402 : 1402
        case XML_SAVE_UNKNOWN_ENCODING = 1403 : 1403
        case XML_REGEXP_COMPILE_ERROR = 1450
        case XML_IO_UNKNOWN = 1500
        case XML_IO_EACCES = 1501 : 1501
        case XML_IO_EAGAIN = 1502 : 1502
        case XML_IO_EBADF = 1503 : 1503
        case XML_IO_EBADMSG = 1504 : 1504
        case XML_IO_EBUSY = 1505 : 1505
        case XML_IO_ECANCELED = 1506 : 1506
        case XML_IO_ECHILD = 1507 : 1507
        case XML_IO_EDEADLK = 1508 : 1508
        case XML_IO_EDOM = 1509 : 1509
        case XML_IO_EEXIST = 1510 : 1510
        case XML_IO_EFAULT = 1511 : 1511
        case XML_IO_EFBIG = 1512 : 1512
        case XML_IO_EINPROGRESS = 1513 : 1513
        case XML_IO_EINTR = 1514 : 1514
        case XML_IO_EINVAL = 1515 : 1515
        case XML_IO_EIO = 1516 : 1516
        case XML_IO_EISDIR = 1517 : 1517
        case XML_IO_EMFILE = 1518 : 1518
        case XML_IO_EMLINK = 1519 : 1519
        case XML_IO_EMSGSIZE = 1520 : 1520
        case XML_IO_ENAMETOOLONG = 1521 : 1521
        case XML_IO_ENFILE = 1522 : 1522
        case XML_IO_ENODEV = 1523 : 1523
        case XML_IO_ENOENT = 1524 : 1524
        case XML_IO_ENOEXEC = 1525 : 1525
        case XML_IO_ENOLCK = 1526 : 1526
        case XML_IO_ENOMEM = 1527 : 1527
        case XML_IO_ENOSPC = 1528 : 1528
        case XML_IO_ENOSYS = 1529 : 1529
        case XML_IO_ENOTDIR = 1530 : 1530
        case XML_IO_ENOTEMPTY = 1531 : 1531
        case XML_IO_ENOTSUP = 1532 : 1532
        case XML_IO_ENOTTY = 1533 : 1533
        case XML_IO_ENXIO = 1534 : 1534
        case XML_IO_EPERM = 1535 : 1535
        case XML_IO_EPIPE = 1536 : 1536
        case XML_IO_ERANGE = 1537 : 1537
        case XML_IO_EROFS = 1538 : 1538
        case XML_IO_ESPIPE = 1539 : 1539
        case XML_IO_ESRCH = 1540 : 1540
        case XML_IO_ETIMEDOUT = 1541 : 1541
        case XML_IO_EXDEV = 1542 : 1542
        case XML_IO_NETWORK_ATTEMPT = 1543 : 1543
        case XML_IO_ENCODER = 1544 : 1544
        case XML_IO_FLUSH = 1545 : 1545
        case XML_IO_WRITE = 1546 : 1546
        case XML_IO_NO_INPUT = 1547 : 1547
        case XML_IO_BUFFER_FULL = 1548 : 1548
        case XML_IO_LOAD_ERROR = 1549 : 1549
        case XML_IO_ENOTSOCK = 1550 : 1550
        case XML_IO_EISCONN = 1551 : 1551
        case XML_IO_ECONNREFUSED = 1552 : 1552
        case XML_IO_ENETUNREACH = 1553 : 1553
        case XML_IO_EADDRINUSE = 1554 : 1554
        case XML_IO_EALREADY = 1555 : 1555
        case XML_IO_EAFNOSUPPORT = 1556 : 1556
        case XML_XINCLUDE_RECURSION = 1600
        case XML_XINCLUDE_PARSE_VALUE = 1601 : 1601
        case XML_XINCLUDE_ENTITY_DEF_MISMATCH = 1602 : 1602
        case XML_XINCLUDE_NO_HREF = 1603 : 1603
        case XML_XINCLUDE_NO_FALLBACK = 1604 : 1604
        case XML_XINCLUDE_HREF_URI = 1605 : 1605
        case XML_XINCLUDE_TEXT_FRAGMENT = 1606 : 1606
        case XML_XINCLUDE_TEXT_DOCUMENT = 1607 : 1607
        case XML_XINCLUDE_INVALID_CHAR = 1608 : 1608
        case XML_XINCLUDE_BUILD_FAILED = 1609 : 1609
        case XML_XINCLUDE_UNKNOWN_ENCODING = 1610 : 1610
        case XML_XINCLUDE_MULTIPLE_ROOT = 1611 : 1611
        case XML_XINCLUDE_XPTR_FAILED = 1612 : 1612
        case XML_XINCLUDE_XPTR_RESULT = 1613 : 1613
        case XML_XINCLUDE_INCLUDE_IN_INCLUDE = 1614 : 1614
        case XML_XINCLUDE_FALLBACKS_IN_INCLUDE = 1615 : 1615
        case XML_XINCLUDE_FALLBACK_NOT_IN_INCLUDE = 1616 : 1616
        case XML_XINCLUDE_DEPRECATED_NS = 1617 : 1617
        case XML_XINCLUDE_FRAGMENT_ID = 1618 : 1618
        case XML_CATALOG_MISSING_ATTR = 1650
        case XML_CATALOG_ENTRY_BROKEN = 1651 : 1651
        case XML_CATALOG_PREFER_VALUE = 1652 : 1652
        case XML_CATALOG_NOT_CATALOG = 1653 : 1653
        case XML_CATALOG_RECURSION = 1654 : 1654
        case XML_SCHEMAP_PREFIX_UNDEFINED = 1700
        case XML_SCHEMAP_ATTRFORMDEFAULT_VALUE = 1701 : 1701
        case XML_SCHEMAP_ATTRGRP_NONAME_NOREF = 1702 : 1702
        case XML_SCHEMAP_ATTR_NONAME_NOREF = 1703 : 1703
        case XML_SCHEMAP_COMPLEXTYPE_NONAME_NOREF = 1704 : 1704
        case XML_SCHEMAP_ELEMFORMDEFAULT_VALUE = 1705 : 1705
        case XML_SCHEMAP_ELEM_NONAME_NOREF = 1706 : 1706
        case XML_SCHEMAP_EXTENSION_NO_BASE = 1707 : 1707
        case XML_SCHEMAP_FACET_NO_VALUE = 1708 : 1708
        case XML_SCHEMAP_FAILED_BUILD_IMPORT = 1709 : 1709
        case XML_SCHEMAP_GROUP_NONAME_NOREF = 1710 : 1710
        case XML_SCHEMAP_IMPORT_NAMESPACE_NOT_URI = 1711 : 1711
        case XML_SCHEMAP_IMPORT_REDEFINE_NSNAME = 1712 : 1712
        case XML_SCHEMAP_IMPORT_SCHEMA_NOT_URI = 1713 : 1713
        case XML_SCHEMAP_INVALID_BOOLEAN = 1714 : 1714
        case XML_SCHEMAP_INVALID_ENUM = 1715 : 1715
        case XML_SCHEMAP_INVALID_FACET = 1716 : 1716
        case XML_SCHEMAP_INVALID_FACET_VALUE = 1717 : 1717
        case XML_SCHEMAP_INVALID_MAXOCCURS = 1718 : 1718
        case XML_SCHEMAP_INVALID_MINOCCURS = 1719 : 1719
        case XML_SCHEMAP_INVALID_REF_AND_SUBTYPE = 1720 : 1720
        case XML_SCHEMAP_INVALID_WHITE_SPACE = 1721 : 1721
        case XML_SCHEMAP_NOATTR_NOREF = 1722 : 1722
        case XML_SCHEMAP_NOTATION_NO_NAME = 1723 : 1723
        case XML_SCHEMAP_NOTYPE_NOREF = 1724 : 1724
        case XML_SCHEMAP_REF_AND_SUBTYPE = 1725 : 1725
        case XML_SCHEMAP_RESTRICTION_NONAME_NOREF = 1726 : 1726
        case XML_SCHEMAP_SIMPLETYPE_NONAME = 1727 : 1727
        case XML_SCHEMAP_TYPE_AND_SUBTYPE = 1728 : 1728
        case XML_SCHEMAP_UNKNOWN_ALL_CHILD = 1729 : 1729
        case XML_SCHEMAP_UNKNOWN_ANYATTRIBUTE_CHILD = 1730 : 1730
        case XML_SCHEMAP_UNKNOWN_ATTR_CHILD = 1731 : 1731
        case XML_SCHEMAP_UNKNOWN_ATTRGRP_CHILD = 1732 : 1732
        case XML_SCHEMAP_UNKNOWN_ATTRIBUTE_GROUP = 1733 : 1733
        case XML_SCHEMAP_UNKNOWN_BASE_TYPE = 1734 : 1734
        case XML_SCHEMAP_UNKNOWN_CHOICE_CHILD = 1735 : 1735
        case XML_SCHEMAP_UNKNOWN_COMPLEXCONTENT_CHILD = 1736 : 1736
        case XML_SCHEMAP_UNKNOWN_COMPLEXTYPE_CHILD = 1737 : 1737
        case XML_SCHEMAP_UNKNOWN_ELEM_CHILD = 1738 : 1738
        case XML_SCHEMAP_UNKNOWN_EXTENSION_CHILD = 1739 : 1739
        case XML_SCHEMAP_UNKNOWN_FACET_CHILD = 1740 : 1740
        case XML_SCHEMAP_UNKNOWN_FACET_TYPE = 1741 : 1741
        case XML_SCHEMAP_UNKNOWN_GROUP_CHILD = 1742 : 1742
        case XML_SCHEMAP_UNKNOWN_IMPORT_CHILD = 1743 : 1743
        case XML_SCHEMAP_UNKNOWN_LIST_CHILD = 1744 : 1744
        case XML_SCHEMAP_UNKNOWN_NOTATION_CHILD = 1745 : 1745
        case XML_SCHEMAP_UNKNOWN_PROCESSCONTENT_CHILD = 1746 : 1746
        case XML_SCHEMAP_UNKNOWN_REF = 1747 : 1747
        case XML_SCHEMAP_UNKNOWN_RESTRICTION_CHILD = 1748 : 1748
        case XML_SCHEMAP_UNKNOWN_SCHEMAS_CHILD = 1749 : 1749
        case XML_SCHEMAP_UNKNOWN_SEQUENCE_CHILD = 1750 : 1750
        case XML_SCHEMAP_UNKNOWN_SIMPLECONTENT_CHILD = 1751 : 1751
        case XML_SCHEMAP_UNKNOWN_SIMPLETYPE_CHILD = 1752 : 1752
        case XML_SCHEMAP_UNKNOWN_TYPE = 1753 : 1753
        case XML_SCHEMAP_UNKNOWN_UNION_CHILD = 1754 : 1754
        case XML_SCHEMAP_ELEM_DEFAULT_FIXED = 1755 : 1755
        case XML_SCHEMAP_REGEXP_INVALID = 1756 : 1756
        case XML_SCHEMAP_FAILED_LOAD = 1757 : 1757
        case XML_SCHEMAP_NOTHING_TO_PARSE = 1758 : 1758
        case XML_SCHEMAP_NOROOT = 1759 : 1759
        case XML_SCHEMAP_REDEFINED_GROUP = 1760 : 1760
        case XML_SCHEMAP_REDEFINED_TYPE = 1761 : 1761
        case XML_SCHEMAP_REDEFINED_ELEMENT = 1762 : 1762
        case XML_SCHEMAP_REDEFINED_ATTRGROUP = 1763 : 1763
        case XML_SCHEMAP_REDEFINED_ATTR = 1764 : 1764
        case XML_SCHEMAP_REDEFINED_NOTATION = 1765 : 1765
        case XML_SCHEMAP_FAILED_PARSE = 1766 : 1766
        case XML_SCHEMAP_UNKNOWN_PREFIX = 1767 : 1767
        case XML_SCHEMAP_DEF_AND_PREFIX = 1768 : 1768
        case XML_SCHEMAP_UNKNOWN_INCLUDE_CHILD = 1769 : 1769
        case XML_SCHEMAP_INCLUDE_SCHEMA_NOT_URI = 1770 : 1770
        case XML_SCHEMAP_INCLUDE_SCHEMA_NO_URI = 1771 : 1771
        case XML_SCHEMAP_NOT_SCHEMA = 1772 : 1772
        case XML_SCHEMAP_UNKNOWN_MEMBER_TYPE = 1773 : 1773
        case XML_SCHEMAP_INVALID_ATTR_USE = 1774 : 1774
        case XML_SCHEMAP_RECURSIVE = 1775 : 1775
        case XML_SCHEMAP_SUPERNUMEROUS_LIST_ITEM_TYPE = 1776 : 1776
        case XML_SCHEMAP_INVALID_ATTR_COMBINATION = 1777 : 1777
        case XML_SCHEMAP_INVALID_ATTR_INLINE_COMBINATION = 1778 : 1778
        case XML_SCHEMAP_MISSING_SIMPLETYPE_CHILD = 1779 : 1779
        case XML_SCHEMAP_INVALID_ATTR_NAME = 1780 : 1780
        case XML_SCHEMAP_REF_AND_CONTENT = 1781 : 1781
        case XML_SCHEMAP_CT_PROPS_CORRECT_1 = 1782 : 1782
        case XML_SCHEMAP_CT_PROPS_CORRECT_2 = 1783 : 1783
        case XML_SCHEMAP_CT_PROPS_CORRECT_3 = 1784 : 1784
        case XML_SCHEMAP_CT_PROPS_CORRECT_4 = 1785 : 1785
        case XML_SCHEMAP_CT_PROPS_CORRECT_5 = 1786 : 1786
        case XML_SCHEMAP_DERIVATION_OK_RESTRICTION_1 = 1787 : 1787
        case XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_1_1 = 1788 : 1788
        case XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_1_2 = 1789 : 1789
        case XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_2 = 1790 : 1790
        case XML_SCHEMAP_DERIVATION_OK_RESTRICTION_3 = 1791 : 1791
        case XML_SCHEMAP_WILDCARD_INVALID_NS_MEMBER = 1792 : 1792
        case XML_SCHEMAP_INTERSECTION_NOT_EXPRESSIBLE = 1793 : 1793
        case XML_SCHEMAP_UNION_NOT_EXPRESSIBLE = 1794 : 1794
        case XML_SCHEMAP_SRC_IMPORT_3_1 = 1795 : 1795
        case XML_SCHEMAP_SRC_IMPORT_3_2 = 1796 : 1796
        case XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_1 = 1797 : 1797
        case XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_2 = 1798 : 1798
        case XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_3 = 1799 : 1799
        case XML_SCHEMAP_COS_CT_EXTENDS_1_3 = 1800 : 1800
        case XML_SCHEMAV_NOROOT = 1801
        case XML_SCHEMAV_UNDECLAREDELEM = 1802 : 1802
        case XML_SCHEMAV_NOTTOPLEVEL = 1803 : 1803
        case XML_SCHEMAV_MISSING = 1804 : 1804
        case XML_SCHEMAV_WRONGELEM = 1805 : 1805
        case XML_SCHEMAV_NOTYPE = 1806 : 1806
        case XML_SCHEMAV_NOROLLBACK = 1807 : 1807
        case XML_SCHEMAV_ISABSTRACT = 1808 : 1808
        case XML_SCHEMAV_NOTEMPTY = 1809 : 1809
        case XML_SCHEMAV_ELEMCONT = 1810 : 1810
        case XML_SCHEMAV_HAVEDEFAULT = 1811 : 1811
        case XML_SCHEMAV_NOTNILLABLE = 1812 : 1812
        case XML_SCHEMAV_EXTRACONTENT = 1813 : 1813
        case XML_SCHEMAV_INVALIDATTR = 1814 : 1814
        case XML_SCHEMAV_INVALIDELEM = 1815 : 1815
        case XML_SCHEMAV_NOTDETERMINIST = 1816 : 1816
        case XML_SCHEMAV_CONSTRUCT = 1817 : 1817
        case XML_SCHEMAV_INTERNAL = 1818 : 1818
        case XML_SCHEMAV_NOTSIMPLE = 1819 : 1819
        case XML_SCHEMAV_ATTRUNKNOWN = 1820 : 1820
        case XML_SCHEMAV_ATTRINVALID = 1821 : 1821
        case XML_SCHEMAV_VALUE = 1822 : 1822
        case XML_SCHEMAV_FACET = 1823 : 1823
        case XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1 = 1824 : 1824
        case XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2 = 1825 : 1825
        case XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_3 = 1826 : 1826
        case XML_SCHEMAV_CVC_TYPE_3_1_1 = 1827 : 1827
        case XML_SCHEMAV_CVC_TYPE_3_1_2 = 1828 : 1828
        case XML_SCHEMAV_CVC_FACET_VALID = 1829 : 1829
        case XML_SCHEMAV_CVC_LENGTH_VALID = 1830 : 1830
        case XML_SCHEMAV_CVC_MINLENGTH_VALID = 1831 : 1831
        case XML_SCHEMAV_CVC_MAXLENGTH_VALID = 1832 : 1832
        case XML_SCHEMAV_CVC_MININCLUSIVE_VALID = 1833 : 1833
        case XML_SCHEMAV_CVC_MAXINCLUSIVE_VALID = 1834 : 1834
        case XML_SCHEMAV_CVC_MINEXCLUSIVE_VALID = 1835 : 1835
        case XML_SCHEMAV_CVC_MAXEXCLUSIVE_VALID = 1836 : 1836
        case XML_SCHEMAV_CVC_TOTALDIGITS_VALID = 1837 : 1837
        case XML_SCHEMAV_CVC_FRACTIONDIGITS_VALID = 1838 : 1838
        case XML_SCHEMAV_CVC_PATTERN_VALID = 1839 : 1839
        case XML_SCHEMAV_CVC_ENUMERATION_VALID = 1840 : 1840
        case XML_SCHEMAV_CVC_COMPLEX_TYPE_2_1 = 1841 : 1841
        case XML_SCHEMAV_CVC_COMPLEX_TYPE_2_2 = 1842 : 1842
        case XML_SCHEMAV_CVC_COMPLEX_TYPE_2_3 = 1843 : 1843
        case XML_SCHEMAV_CVC_COMPLEX_TYPE_2_4 = 1844 : 1844
        case XML_SCHEMAV_CVC_ELT_1 = 1845 : 1845
        case XML_SCHEMAV_CVC_ELT_2 = 1846 : 1846
        case XML_SCHEMAV_CVC_ELT_3_1 = 1847 : 1847
        case XML_SCHEMAV_CVC_ELT_3_2_1 = 1848 : 1848
        case XML_SCHEMAV_CVC_ELT_3_2_2 = 1849 : 1849
        case XML_SCHEMAV_CVC_ELT_4_1 = 1850 : 1850
        case XML_SCHEMAV_CVC_ELT_4_2 = 1851 : 1851
        case XML_SCHEMAV_CVC_ELT_4_3 = 1852 : 1852
        case XML_SCHEMAV_CVC_ELT_5_1_1 = 1853 : 1853
        case XML_SCHEMAV_CVC_ELT_5_1_2 = 1854 : 1854
        case XML_SCHEMAV_CVC_ELT_5_2_1 = 1855 : 1855
        case XML_SCHEMAV_CVC_ELT_5_2_2_1 = 1856 : 1856
        case XML_SCHEMAV_CVC_ELT_5_2_2_2_1 = 1857 : 1857
        case XML_SCHEMAV_CVC_ELT_5_2_2_2_2 = 1858 : 1858
        case XML_SCHEMAV_CVC_ELT_6 = 1859 : 1859
        case XML_SCHEMAV_CVC_ELT_7 = 1860 : 1860
        case XML_SCHEMAV_CVC_ATTRIBUTE_1 = 1861 : 1861
        case XML_SCHEMAV_CVC_ATTRIBUTE_2 = 1862 : 1862
        case XML_SCHEMAV_CVC_ATTRIBUTE_3 = 1863 : 1863
        case XML_SCHEMAV_CVC_ATTRIBUTE_4 = 1864 : 1864
        case XML_SCHEMAV_CVC_COMPLEX_TYPE_3_1 = 1865 : 1865
        case XML_SCHEMAV_CVC_COMPLEX_TYPE_3_2_1 = 1866 : 1866
        case XML_SCHEMAV_CVC_COMPLEX_TYPE_3_2_2 = 1867 : 1867
        case XML_SCHEMAV_CVC_COMPLEX_TYPE_4 = 1868 : 1868
        case XML_SCHEMAV_CVC_COMPLEX_TYPE_5_1 = 1869 : 1869
        case XML_SCHEMAV_CVC_COMPLEX_TYPE_5_2 = 1870 : 1870
        case XML_SCHEMAV_ELEMENT_CONTENT = 1871 : 1871
        case XML_SCHEMAV_DOCUMENT_ELEMENT_MISSING = 1872 : 1872
        case XML_SCHEMAV_CVC_COMPLEX_TYPE_1 = 1873 : 1873
        case XML_SCHEMAV_CVC_AU = 1874 : 1874
        case XML_SCHEMAV_CVC_TYPE_1 = 1875 : 1875
        case XML_SCHEMAV_CVC_TYPE_2 = 1876 : 1876
        case XML_SCHEMAV_CVC_IDC = 1877 : 1877
        case XML_SCHEMAV_CVC_WILDCARD = 1878 : 1878
        case XML_SCHEMAV_MISC = 1879 : 1879
        case XML_XPTR_UNKNOWN_SCHEME = 1900
        case XML_XPTR_CHILDSEQ_START = 1901 : 1901
        case XML_XPTR_EVAL_FAILED = 1902 : 1902
        case XML_XPTR_EXTRA_OBJECTS = 1903 : 1903
        case XML_C14N_CREATE_CTXT = 1950
        case XML_C14N_REQUIRES_UTF8 = 1951 : 1951
        case XML_C14N_CREATE_STACK = 1952 : 1952
        case XML_C14N_INVALID_NODE = 1953 : 1953
        case XML_C14N_UNKNOW_NODE = 1954 : 1954
        case XML_C14N_RELATIVE_NAMESPACE = 1955 : 1955
        case XML_FTP_PASV_ANSWER = 2000
        case XML_FTP_EPSV_ANSWER = 2001 : 2001
        case XML_FTP_ACCNT = 2002 : 2002
        case XML_FTP_URL_SYNTAX = 2003 : 2003
        case XML_HTTP_URL_SYNTAX = 2020
        case XML_HTTP_USE_IP = 2021 : 2021
        case XML_HTTP_UNKNOWN_HOST = 2022 : 2022
        case XML_SCHEMAP_SRC_SIMPLE_TYPE_1 = 3000
        case XML_SCHEMAP_SRC_SIMPLE_TYPE_2 = 3001 : 3001
        case XML_SCHEMAP_SRC_SIMPLE_TYPE_3 = 3002 : 3002
        case XML_SCHEMAP_SRC_SIMPLE_TYPE_4 = 3003 : 3003
        case XML_SCHEMAP_SRC_RESOLVE = 3004 : 3004
        case XML_SCHEMAP_SRC_RESTRICTION_BASE_OR_SIMPLETYPE = 3005 : 3005
        case XML_SCHEMAP_SRC_LIST_ITEMTYPE_OR_SIMPLETYPE = 3006 : 3006
        case XML_SCHEMAP_SRC_UNION_MEMBERTYPES_OR_SIMPLETYPES = 3007 : 3007
        case XML_SCHEMAP_ST_PROPS_CORRECT_1 = 3008 : 3008
        case XML_SCHEMAP_ST_PROPS_CORRECT_2 = 3009 : 3009
        case XML_SCHEMAP_ST_PROPS_CORRECT_3 = 3010 : 3010
        case XML_SCHEMAP_COS_ST_RESTRICTS_1_1 = 3011 : 3011
        case XML_SCHEMAP_COS_ST_RESTRICTS_1_2 = 3012 : 3012
        case XML_SCHEMAP_COS_ST_RESTRICTS_1_3_1 = 3013 : 3013
        case XML_SCHEMAP_COS_ST_RESTRICTS_1_3_2 = 3014 : 3014
        case XML_SCHEMAP_COS_ST_RESTRICTS_2_1 = 3015 : 3015
        case XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_1 = 3016 : 3016
        case XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_2 = 3017 : 3017
        case XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_1 = 3018 : 3018
        case XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_2 = 3019 : 3019
        case XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_3 = 3020 : 3020
        case XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_4 = 3021 : 3021
        case XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_5 = 3022 : 3022
        case XML_SCHEMAP_COS_ST_RESTRICTS_3_1 = 3023 : 3023
        case XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1 = 3024 : 3024
        case XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1_2 = 3025 : 3025
        case XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_2 = 3026 : 3026
        case XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_1 = 3027 : 3027
        case XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_3 = 3028 : 3028
        case XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_4 = 3029 : 3029
        case XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_5 = 3030 : 3030
        case XML_SCHEMAP_COS_ST_DERIVED_OK_2_1 = 3031 : 3031
        case XML_SCHEMAP_COS_ST_DERIVED_OK_2_2 = 3032 : 3032
        case XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED = 3033 : 3033
        case XML_SCHEMAP_S4S_ELEM_MISSING = 3034 : 3034
        case XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED = 3035 : 3035
        case XML_SCHEMAP_S4S_ATTR_MISSING = 3036 : 3036
        case XML_SCHEMAP_S4S_ATTR_INVALID_VALUE = 3037 : 3037
        case XML_SCHEMAP_SRC_ELEMENT_1 = 3038 : 3038
        case XML_SCHEMAP_SRC_ELEMENT_2_1 = 3039 : 3039
        case XML_SCHEMAP_SRC_ELEMENT_2_2 = 3040 : 3040
        case XML_SCHEMAP_SRC_ELEMENT_3 = 3041 : 3041
        case XML_SCHEMAP_P_PROPS_CORRECT_1 = 3042 : 3042
        case XML_SCHEMAP_P_PROPS_CORRECT_2_1 = 3043 : 3043
        case XML_SCHEMAP_P_PROPS_CORRECT_2_2 = 3044 : 3044
        case XML_SCHEMAP_E_PROPS_CORRECT_2 = 3045 : 3045
        case XML_SCHEMAP_E_PROPS_CORRECT_3 = 3046 : 3046
        case XML_SCHEMAP_E_PROPS_CORRECT_4 = 3047 : 3047
        case XML_SCHEMAP_E_PROPS_CORRECT_5 = 3048 : 3048
        case XML_SCHEMAP_E_PROPS_CORRECT_6 = 3049 : 3049
        case XML_SCHEMAP_SRC_INCLUDE = 3050 : 3050
        case XML_SCHEMAP_SRC_ATTRIBUTE_1 = 3051 : 3051
        case XML_SCHEMAP_SRC_ATTRIBUTE_2 = 3052 : 3052
        case XML_SCHEMAP_SRC_ATTRIBUTE_3_1 = 3053 : 3053
        case XML_SCHEMAP_SRC_ATTRIBUTE_3_2 = 3054 : 3054
        case XML_SCHEMAP_SRC_ATTRIBUTE_4 = 3055 : 3055
        case XML_SCHEMAP_NO_XMLNS = 3056 : 3056
        case XML_SCHEMAP_NO_XSI = 3057 : 3057
        case XML_SCHEMAP_COS_VALID_DEFAULT_1 = 3058 : 3058
        case XML_SCHEMAP_COS_VALID_DEFAULT_2_1 = 3059 : 3059
        case XML_SCHEMAP_COS_VALID_DEFAULT_2_2_1 = 3060 : 3060
        case XML_SCHEMAP_COS_VALID_DEFAULT_2_2_2 = 3061 : 3061
        case XML_SCHEMAP_CVC_SIMPLE_TYPE = 3062 : 3062
        case XML_SCHEMAP_COS_CT_EXTENDS_1_1 = 3063 : 3063
        case XML_SCHEMAP_SRC_IMPORT_1_1 = 3064 : 3064
        case XML_SCHEMAP_SRC_IMPORT_1_2 = 3065 : 3065
        case XML_SCHEMAP_SRC_IMPORT_2 = 3066 : 3066
        case XML_SCHEMAP_SRC_IMPORT_2_1 = 3067 : 3067
        case XML_SCHEMAP_SRC_IMPORT_2_2 = 3068 : 3068
        case XML_SCHEMAP_INTERNAL = 3069 : 3069 non-W3C
        case XML_SCHEMAP_NOT_DETERMINISTIC = 3070 : 3070 non-W3C
        case XML_SCHEMAP_SRC_ATTRIBUTE_GROUP_1 = 3071 : 3071
        case XML_SCHEMAP_SRC_ATTRIBUTE_GROUP_2 = 3072 : 3072
        case XML_SCHEMAP_SRC_ATTRIBUTE_GROUP_3 = 3073 : 3073
        case XML_SCHEMAP_MG_PROPS_CORRECT_1 = 3074 : 3074
        case XML_SCHEMAP_MG_PROPS_CORRECT_2 = 3075 : 3075
        case XML_SCHEMAP_SRC_CT_1 = 3076 : 3076
        case XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_1_3 = 3077 : 3077
        case XML_SCHEMAP_AU_PROPS_CORRECT_2 = 3078 : 3078
        case XML_SCHEMAP_A_PROPS_CORRECT_2 = 3079 : 3079
        case XML_SCHEMAP_C_PROPS_CORRECT = 3080 : 3080
        case XML_SCHEMAP_SRC_REDEFINE = 3081 : 3081
        case XML_SCHEMAP_SRC_IMPORT = 3082 : 3082
        case XML_SCHEMAP_WARN_SKIP_SCHEMA = 3083 : 3083
        case XML_SCHEMAP_WARN_UNLOCATED_SCHEMA = 3084 : 3084
        case XML_SCHEMAP_WARN_ATTR_REDECL_PROH = 3085 : 3085
        case XML_SCHEMAP_WARN_ATTR_POINTLESS_PROH = 3086 : 3085
        case XML_SCHEMAP_AG_PROPS_CORRECT = 3087 : 3086
        case XML_SCHEMAP_COS_CT_EXTENDS_1_2 = 3088 : 3087
        case XML_SCHEMAP_AU_PROPS_CORRECT = 3089 : 3088
        case XML_SCHEMAP_A_PROPS_CORRECT_3 = 3090 : 3089
        case XML_SCHEMAP_COS_ALL_LIMITED = 3091 : 3090
        case XML_SCHEMATRONV_ASSERT = 4000 : 4000
        case XML_SCHEMATRONV_REPORT = 4001
        case XML_MODULE_OPEN = 4900 : 4900
        case XML_MODULE_CLOSE = 4901 : 4901
        case XML_CHECK_FOUND_ELEMENT = 5000
        case XML_CHECK_FOUND_ATTRIBUTE = 5001 : 5001
        case XML_CHECK_FOUND_TEXT = 5002 : 5002
        case XML_CHECK_FOUND_CDATA = 5003 : 5003
        case XML_CHECK_FOUND_ENTITYREF = 5004 : 5004
        case XML_CHECK_FOUND_ENTITY = 5005 : 5005
        case XML_CHECK_FOUND_PI = 5006 : 5006
        case XML_CHECK_FOUND_COMMENT = 5007 : 5007
        case XML_CHECK_FOUND_DOCTYPE = 5008 : 5008
        case XML_CHECK_FOUND_FRAGMENT = 5009 : 5009
        case XML_CHECK_FOUND_NOTATION = 5010 : 5010
        case XML_CHECK_UNKNOWN_NODE = 5011 : 5011
        case XML_CHECK_ENTITY_TYPE = 5012 : 5012
        case XML_CHECK_NO_PARENT = 5013 : 5013
        case XML_CHECK_NO_DOC = 5014 : 5014
        case XML_CHECK_NO_NAME = 5015 : 5015
        case XML_CHECK_NO_ELEM = 5016 : 5016
        case XML_CHECK_WRONG_DOC = 5017 : 5017
        case XML_CHECK_NO_PREV = 5018 : 5018
        case XML_CHECK_WRONG_PREV = 5019 : 5019
        case XML_CHECK_NO_NEXT = 5020 : 5020
        case XML_CHECK_WRONG_NEXT = 5021 : 5021
        case XML_CHECK_NOT_DTD = 5022 : 5022
        case XML_CHECK_NOT_ATTR = 5023 : 5023
        case XML_CHECK_NOT_ATTR_DECL = 5024 : 5024
        case XML_CHECK_NOT_ELEM_DECL = 5025 : 5025
        case XML_CHECK_NOT_ENTITY_DECL = 5026 : 5026
        case XML_CHECK_NOT_NS_DECL = 5027 : 5027
        case XML_CHECK_NO_HREF = 5028 : 5028
        case XML_CHECK_WRONG_PARENT = 5029 : 5029
        case XML_CHECK_NS_SCOPE = 5030 : 5030
        case XML_CHECK_NS_ANCESTOR = 5031 : 5031
        case XML_CHECK_NOT_UTF8 = 5032 : 5032
        case XML_CHECK_NO_DICT = 5033 : 5033
        case XML_CHECK_NOT_NCNAME = 5034 : 5034
        case XML_CHECK_OUTSIDE_DICT = 5035 : 5035
        case XML_CHECK_WRONG_NAME = 5036 : 5036
        case XML_CHECK_NAME_NOT_NULL = 5037 : 5037
        case XML_I18N_NO_NAME = 6000
        case XML_I18N_NO_HANDLER = 6001 : 6001
        case XML_I18N_EXCESS_HANDLER = 6002 : 6002
        case XML_I18N_CONV_FAILED = 6003 : 6003
        case XML_I18N_NO_OUTPUT = 6004 : 6004
        case XML_CHECK_ = 6005 : 5033
        case XML_CHECK_X = 6006 : 503
        */
        default:
            oss << "Not-OK";
            break;
        }
        oss << " (code=" << code << ")";
        return oss.str();
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////


    std::string build_error_message(const xmlError& err)
    {
        std::ostringstream oss;
        oss << domain_code_to_string(err.domain) << ": " << error_code_to_string(err.code);
        if (err.message != 0)
        {
            oss << ", details: " << err.message;
        }
        if (err.int1 > 0 && err.int2 > 0)
        {
            oss << ", location: " << err.int1 << ": " << err.int2;
        }
        return oss.str();
    }


}  // namespace xtree::detail
}  // namespace xtree


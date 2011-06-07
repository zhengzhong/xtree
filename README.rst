---------------------------------
xtree - a C++ wrapper for libxml2
---------------------------------

**xtree** is a C++ wrapper for the popular `libxml2 <http://xmlsoft.org/>`_ XML parser library.
Though libxml2 is famous for its high performance and compliance to standard specifications,
its C API is quite difficult to use. Built on top of libxml2, xtree provides an easy-to-use,
STL-like API for accessing and manipulating the underlying DOM tree.


Building xtree
==============

To build xtree, you'll need to download some required packages, including:

- libxml2 and its dependencies (zlib and iconv) : windows binaries can be found at
  `http://www.zlatkovic.com/libxml.en.html <http://www.zlatkovic.com/libxml.en.html>`_

- `boost test library <http://www.boost.org/doc/libs/1_34_0/libs/test/doc/index.html>`_ :
  required by xtree unit test project.

The xtree project contains following project files for Visual C++ 7.1 (2003):

- ``xtree-vc71.vcproj`` : xtree library project file.
- ``xtree_test-vc71.vcproj`` : xtree unit test project file.
- ``xtree_all.sln`` : solution file containing the above projects.

The xtree project files use following variables to locate required packages:

- ``LIBXML2_INC``, ``LIBXML2_LIB`` and ``LIBXML2_BIN`` for libxml2.
- ``ICONV_INC``, ``ICONV_LIB`` and ``ICONV_BIN`` for iconv.
- ``ZLIB_INC``, ``ZLIB_LIB`` and ``ZLIB_BIN`` for zlib.
- ``BOOST_INC`` and ``BOOST_LIB`` for boost test library.

where ``XXX_INC`` should point to the include directory, ``XXX_LIB`` should point to the library
directory, and ``XXX_BIN`` should point to the binary directory.

You can change these settings in xtree's project files, or set environment variables accordingly.

To run the test cases for xtree's dynamic library, you'll also need to add ``LIBXML2_BIN``,
``ICONV_BIN`` and ``ZLIB_BIN`` to your ``PATH``.

xtree has been tested with the following versions of libxml2 (and its dependencies):

========  ======  ======
libxml2   iconv   zlib
========  ======  ======
2.6.24    1.9.2   1.2.3
2.7.7     1.9.2   1.2.5
========  ======  ======


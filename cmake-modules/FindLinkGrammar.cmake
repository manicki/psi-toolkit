# - Try to find Link Grammar
# Once done this will define
#
#  LINK_GRAMMAR_FOUND - system has Link Grammar
#  LINK_GRAMMAR_INCLUDE_DIRS - the include directories for Link Grammar headers
#  LINK_GRAMMAR_LIBRARIES - link these to use Link Grammar

if (LINK_GRAMMAR_LIBRARIES)
   # in cache already
   SET(LINK_GRAMMAR_FIND_QUIETLY TRUE)
endif (LINK_GRAMMAR_LIBRARIES)

find_package(PkgConfig)

if (PKG_CONFIG_FOUND)
endif (PKG_CONFIG_FOUND)

pkg_check_modules(LINK_GRAMMAR_PKG link-grammar)

# Paths

find_path(LINK_GRAMMAR_INCLUDE_DIR
    NAMES link-includes.h
    HINTS ${LINK_GRAMMAR_PKG_INCLUDE_DIRS}
    PATHS /usr/include /usr/local/include
    PATH_SUFFIXES link-grammar
)
if (NOT LINK_GRAMMAR_INCLUDE_DIR)
    message(STATUS "Could not find link-grammar include dir")
    set(LINK_GRAMMAR_FOUND FALSE)
endif (NOT LINK_GRAMMAR_INCLUDE_DIR)

# Libraries

find_library(LINK_GRAMMAR_LIBRARY
    NAMES link-grammar liblink-grammar
    HINTS ${LINK_GRAMMAR_PKG_LIBRARY_DIRS}
    PATHS /usr/lib /usr/local/lib
)
if (NOT LINK_GRAMMAR_LIBRARY)
    message(STATUS "Could not find liblink-grammar")
    set(LINK_GRAMMAR_FOUND FALSE)
endif (NOT LINK_GRAMMAR_LIBRARY)


set(LINK_GRAMMAR_INCLUDE_DIRS ${LINK_GRAMMAR_INCLUDE_DIR})
set(LINK_GRAMMAR_LIBRARIES ${LINK_GRAMMAR_LIBRARY})

if (LINK_GRAMMAR_INCLUDE_DIRS AND LINK_GRAMMAR_LIBRARIES)
    set(LINK_GRAMMAR_FOUND TRUE)
else (LINK_GRAMMAR_INCLUDE_DIRS AND LINK_GRAMMAR_LIBRARIES)
    set(LINK_GRAMMAR_FOUND FALSE)
endif (LINK_GRAMMAR_INCLUDE_DIRS AND LINK_GRAMMAR_LIBRARIES)

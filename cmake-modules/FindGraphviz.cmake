# - Try to find Graphviz
# Once done this will define
#
#  GRAPHVIZ_FOUND - system has Graphviz
#  GRAPHVIZ_INCLUDE_DIRS - the include directories for Graphviz headers
#  GRAPHVIZ_LIBRARIES - link these to use Graphviz

if (GRAPHVIZ_LIBRARIES)
   # in cache already
   SET(GRAPHVIZ_FIND_QUIETLY TRUE)
endif (GRAPHVIZ_LIBRARIES)

find_package(PkgConfig)

pkg_check_modules(GRAPHVIZ_PKG libgvc)

# Paths

find_path(GRAPHVIZ_INCLUDE_DIR
    NAMES gvc.h graph.h cdt.h
    HINTS ${GRAPHVIZ_PKG_INCLUDE_DIRS}
    PATHS /usr/include /usr/local/include
)
if (NOT GRAPHVIZ_INCLUDE_DIR)
    message(STATUS "Could not find graphviz include dir")
    set(GRAPHVIZ_FOUND FALSE)
endif (NOT GRAPHVIZ_INCLUDE_DIR)

# Libraries

find_library(GRAPHVIZ_GVC_LIBRARY
    NAMES gvc
    HINTS ${GRAPHVIZ_PKG_LIBRARY_DIRS}
    PATHS /usr/lib /usr/local/lib
)
if (NOT GRAPHVIZ_GVC_LIBRARY)
    message(STATUS "Could not find graphviz gvc")
    set(GRAPHVIZ_FOUND FALSE)
endif (NOT GRAPHVIZ_GVC_LIBRARY)

find_library(GRAPHVIZ_GRAPH_LIBRARY
    NAMES graph
    HINTS ${GRAPHVIZ_PKG_LIBRARY_DIRS}
    PATHS /usr/lib /usr/local/lib
)
if (NOT GRAPHVIZ_GRAPH_LIBRARY)
    message(STATUS "Could not find graphviz graph")
    set(GRAPHVIZ_FOUND FALSE)
endif (NOT GRAPHVIZ_GRAPH_LIBRARY)

find_library(GRAPHVIZ_CDT_LIBRARY
    NAMES cdt
    HINTS ${GRAPHVIZ_PKG_LIBRARY_DIRS}
    PATHS /usr/lib /usr/local/lib
)
if (NOT GRAPHVIZ_CDT_LIBRARY)
    message(STATUS "Could not find graphviz cdt")
    set(GRAPHVIZ_FOUND FALSE)
endif (NOT GRAPHVIZ_CDT_LIBRARY)

find_library(GRAPHVIZ_PATHPLAN_LIBRARY
    NAMES pathplan
    HINTS ${GRAPHVIZ_PKG_LIBRARY_DIRS}
    PATHS /usr/lib /usr/local/lib
)
if (NOT GRAPHVIZ_PATHPLAN_LIBRARY)
    message(STATUS "Could not find graphviz pathplan")
    set(GRAPHVIZ_FOUND FALSE)
endif (NOT GRAPHVIZ_PATHPLAN_LIBRARY)


set(GRAPHVIZ_INCLUDE_DIRS ${GRAPHVIZ_INCLUDE_DIR})
set(GRAPHVIZ_LIBRARIES ${GRAPHVIZ_GVC_LIBRARY} ${GRAPHVIZ_GRAPH_LIBRARY} ${GRAPHVIZ_CDT_LIBRARY} ${GRAPHVIZ_PATHPLAN_LIBRARY})

if (GRAPHVIZ_INCLUDE_DIRS AND GRAPHVIZ_LIBRARIES)
    set(GRAPHVIZ_FOUND TRUE)
else (GRAPHVIZ_INCLUDE_DIRS AND GRAPHVIZ_LIBRARIES)
    set(GRAPHVIZ_FOUND FALSE)
endif (GRAPHVIZ_INCLUDE_DIRS AND GRAPHVIZ_LIBRARIES)

# - Try to find Poppler and Poppler-glib
# Once done this will define
#
#  POPPLER_FOUND - system has Poppler
#  POPPLER_INCLUDE_DIRS - the include directories for Poppler and Poppler-glib headers
#  POPPLER_LIBRARIES - link these to use Poppler and Poppler-glib

if (POPPLER_LIBRARIES)
   # in cache already
   SET(Poppler_FIND_QUIETLY TRUE)
endif (POPPLER_LIBRARIES)

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(GLIB_PKG glib-2.0)
    find_path(GTK2_GLIBCONFIG_INCLUDE_DIR
        NAMES glibconfig.h
        HINTS ${GLIB_PKG_INCLUDE_DIRS}
    )
endif (PKG_CONFIG_FOUND)

find_package(GTK2)

pkg_check_modules(POPPLER_PKG poppler poppler-glib)

find_library(POPPLER_LIBRARIES
    NAMES poppler libpoppler
    HINTS ${POPPLER_PKG_LIBRARY_DIRS}
    PATHS /usr/lib /usr/local/lib
)
if (NOT POPPLER_LIBRARIES)
    message(STATUS "Could not find libpoppler")
    set(POPPLER_FOUND FALSE)
else (NOT POPPLER_LIBRARIES)
    set(POPPLER_FOUND TRUE)
endif (NOT POPPLER_LIBRARIES)

find_path(POPPLER_GLIB_INCLUDE_DIR
    NAMES poppler.h
    HINTS ${POPPLER_PKG_INCLUDE_DIRS}
    PATHS /usr/include /usr/local/include
    PATH_SUFFIXES glib
)
if (NOT POPPLER_GLIB_INCLUDE_DIR)
    message(STATUS "Could not find poppler-glib include dir")
    set(POPPLER_FOUND FALSE)
else (NOT POPPLER_GLIB_INCLUDE_DIR)
    set(POPPLER_FOUND TRUE)
endif (NOT POPPLER_GLIB_INCLUDE_DIR)

set(POPPLER_INCLUDE_DIRS ${GTK2_INCLUDE_DIRS} ${POPPLER_GLIB_INCLUDE_DIR})

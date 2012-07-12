cmake_minimum_required(VERSION 2.8)

project(psi-toolkit CXX)

SET(MY_TARGETLIB_PREFIX "libpsitoolkit_")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

include_directories(${psi-toolkit_SOURCE_DIR})

# coverage mode
if(CMAKE_CONFIGURATION_TYPES)
  list(APPEND CMAKE_CONFIGURATION_TYPES Coverage)
  list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
    "Add the configurations that we need"
     FORCE)
endif()

SET( CMAKE_CXX_FLAGS_COVERAGE "-g -pg -Wall -Wundef -Wnon-virtual-dtor -Wextra -fprofile-arcs -ftest-coverage" CACHE STRING
    "Flags used by the C++ compiler during maintainer builds."
    FORCE )
SET( CMAKE_C_FLAGS_COVERAGE "-g -pg -Wall -pedantic -fprofile-arcs -ftest-coverage" CACHE STRING
    "Flags used by the C compiler during maintainer builds."
    FORCE )
SET( CMAKE_EXE_LINKER_FLAGS_COVERAGE
    "-g -pg -Wl,--warn-unresolved-symbols,--warn-once -fprofile-arcs -ftest-coverage" CACHE STRING
    "Flags used for linking binaries during maintainer builds."
    FORCE )
SET( CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    "-g -pg -Wl,--warn-unresolved-symbols,--warn-once -fprofile-arcs -ftest-coverage" CACHE STRING
    "Flags used by the shared libraries linker during maintainer builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE)

# extracting information from git, see http://stackoverflow.com/questions/1435953/how-can-i-pass-git-sha1-to-compiler-as-definition-using-cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/")
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
git_last_date(GIT_LAST_DATE)

# ====================================================
# Options for installable version
# ====================================================

set(PSITOOLKIT_INSTALL_DATA_DIR share/psi-toolkit)
set(INSTALL_DATA_DIR "${CMAKE_INSTALL_PREFIX}/${PSITOOLKIT_INSTALL_DATA_DIR}")

option(IS_INSTALLABLE "Is prepared for install" OFF)
message(STATUS "Is prepared for install ${IS_INSTALLABLE}")

set(PACKAGE_OS_NAME default)
option(PACKAGE_OS_NAME "Name of the operating system for package generation" default)

if(PACKAGE_OS_NAME STREQUAL ubuntu)
  set(PACKAGE_OS 1)
elseif(PACKAGE_OS_NAME STREQUAL archlinux)
  set(PACKAGE_OS 2)
else()
  set(PACKAGE_OS -1)
endif()

if(PACKAGE_OS GREATER 0)
  message("-- Setting package os variable to: ${PACKAGE_OS_NAME} (${PACKAGE_OS})")
else()
  message("-- Setting default package os")
endif()

option(DOWNLOAD_DATA "Downloading data (artifacts)" ON)
message(STATUS "Downloading data (artifacts) ${DOWNLOAD_DATA}")

# setting PSI-Server variables
if(IS_INSTALLABLE)
    # ./server/website content has to be moved to /var/psi-server/website during installation
    set(WEBSITE_ROOT "/var/lib/psi-server/website" CACHE STRING "The website root directory")
else()
    set(WEBSITE_ROOT "../server/website" CACHE STRING "The website root directory")
endif(IS_INSTALLABLE)

message(STATUS "The psi-server root directory is set to ${WEBSITE_ROOT}")


# ====================================================
# Finds and includes external libraries
# ====================================================

# ----------------------------------------------------
# Boost
# ----------------------------------------------------
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS program_options graph serialization filesystem system thread
    unit_test_framework prg_exec_monitor REQUIRED)

# ----------------------------------------------------
# Logging
# ----------------------------------------------------
find_library(LOG4CPP_LIB NAMES log4cpp)
find_path(LOG4CPP_INCLUDE log4cpp/Appender.hh)

if(EXISTS ${LOG4CPP_LIB} AND EXISTS ${LOG4CPP_INCLUDE})
  message(STATUS "Found Log4cpp")
  include_directories(${LOG4CPP_INCLUDE})
  link_directories(${LOG4CPP_LIB})
  set(HAVE_LOG4CPP 1)
else()
  message(WARNING "Log4cpp not found")
  set(HAVE_LOG4CPP 0)
endif(EXISTS ${LOG4CPP_LIB} AND EXISTS ${LOG4CPP_INCLUDE})

# ----------------------------------------------------
# Aspell
# ----------------------------------------------------
option(USE_ASPELL "Using aspell" ON)
message(STATUS "Using aspell ${USE_ASPELL}")

if (USE_ASPELL)
  find_library(ASPELL_LIB NAMES aspell)
  find_path(ASPELL_INCLUDE aspell.h)
  if(EXISTS ${ASPELL_LIB} AND EXISTS ${ASPELL_INCLUDE})
    message(STATUS "Found Aspell")
    include_directories(${ASPELL_INCLUDE})
    link_directories(${ASPELL_LIB})
    link_directories("${psi-toolkit_BINARY_DIR}/tools/spellcheckers/aspell/plugin")
    set(HAVE_ASPELL 1)
  else()
    message(WARNING "Aspell not found: aspell disabled")
    set(HAVE_ASPELL 0)
  endif(EXISTS ${ASPELL_LIB} AND EXISTS ${ASPELL_INCLUDE})
else()
  set(HAVE_ASPELL 0)
endif(USE_ASPELL)

# ----------------------------------------------------
# CMPH library (for perfect hashing)
# ----------------------------------------------------
find_library(CMPH_LIB NAMES cmph)
find_path(CMPH_INCLUDE cmph.h)

if(EXISTS ${CMPH_LIB} AND EXISTS ${CMPH_INCLUDE})
  message(STATUS "Found CMPH")
  include_directories(${CMPH_INCLUDE})
  link_directories(${CMPH_LIB})
  set(HAVE_CMPH 1)
else()
  message(WARNING "CMPH not found")
  set(HAVE_CMPH 0)
endif(EXISTS ${CMPH_LIB} AND EXISTS ${CMPH_INCLUDE})

# ----------------------------------------------------
# SFST library (for TRmorph)
# ----------------------------------------------------
option(USE_SFST "Using SFST" OFF)
message(STATUS "Using SFST ${USE_SFST}")

if (USE_SFST)
   find_path(SFST_LIB libsfst.so /usr/lib/sfst)
   find_path(SFST_LIB libsfst.so /usr/lib)
   find_path(SFST_INCLUDE fst.h /usr/include/sfst)

   if(EXISTS ${SFST_LIB} AND EXISTS ${SFST_INCLUDE})
      message(STATUS "Found SFST")
      include_directories(${SFST_INCLUDE})
      link_directories(${SFST_LIB})
      link_directories("${psi-toolkit_BINARY_DIR}/tools/lemmatizers/sfst/plugin")
      set(HAVE_SFST 1)
   else()
      message(WARNING "SFST not found")
      set(HAVE_SFST 0)
   endif(EXISTS ${SFST_LIB} AND EXISTS ${SFST_INCLUDE})
endif (USE_SFST)

# ----------------------------------------------------
# Java components
# ----------------------------------------------------
option(USE_JAVA "Using Java components" OFF)
message(STATUS "Using Java components ${USE_JAVA}")

if(USE_JAVA)
    enable_language(Java)

    find_package(Java REQUIRED)
    find_package(JNI REQUIRED)

    string(REGEX REPLACE "(.*)/libjvm.so" "\\1" LD_LIBPATH ${JAVA_JVM_LIBRARY})
    message(STATUS "You may need to set LD_LIBRARY_PATH=${LD_LIBPATH} to run installed software")

    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:${LD_LIBPATH}")

    include_directories(${JAVA_INCLUDE_PATH})
    include_directories(${JAVA_INCLUDE_PATH}/linux)
    include_directories("${psi-toolkit_SOURCE_DIR}/common/j")

    set(HAVE_JAVA 1)
endif(USE_JAVA)

# ----------------------------------------------------
# Finds PostgreSQL
# ----------------------------------------------------
option(USE_POSTGRESQL "Compiling PostgreSQL components" OFF)
message(STATUS "Compiling PostgreSQL components ${USE_POSTGRESQL}")

if(USE_POSTGRESQL)
    find_package(PostgreSQL REQUIRED)
    include_directories(${PostgreSQL_INCLUDE_DIRS})
    link_directories(${PostgreSQL_LIBRARY_DIRS})
    set(HAVE_POSTGRESQL 1)
endif(USE_POSTGRESQL)

# ----------------------------------------------------
# Regular expression libraries
# ----------------------------------------------------
option(WITH_RE2 "Using RE2 regular expression library" OFF)
message(STATUS "Using RE2 regular expression library ${WITH_RE2}")

if(WITH_RE2)
    set(HAVE_RE2 1)
endif(WITH_RE2)

option(WITH_PCRE "Using PCRE regular expression library" ON)
message(STATUS "Using PCRE regular expression library ${WITH_PCRE}")

if(WITH_PCRE)
    set(HAVE_PCRE 1)
endif(WITH_PCRE)

# ----------------------------------------------------
# Bonsai and puddle
# ----------------------------------------------------
option(USE_BONSAI_PARSEGRAPH "Using bonsai ParseGraph with puddle shallow parser" OFF)
message(STATUS "Using bonsai ParseGraph with puddle shallow parser ${USE_BONSAI_PARSEGRAPH}")

option(BONSAI_PATH "Bonsai common path file (required while using ParseGraph)" "/home/user/poleng4/src/bonsai/common")
message(STATUS "Bonsai common path file (required while using ParseGraph) ${BONSAI_PATH}")

option(USE_STRING_FRAG_VALIDATION "Validating string frags" OFF)
message(STATUS "Validating string frags ${USE_STRING_FRAG_VALIDATION}")

if(USE_STRING_FRAG_VALIDATION)
    set(STRING_FRAG_VALIDATION 1)
endif(USE_STRING_FRAG_VALIDATION)

# ----------------------------------------------------
# Finds SWIG
# ----------------------------------------------------
find_package(SWIG)
if(SWIG_FOUND)

  message(STATUS "Found SWIG version: ${SWIG_VERSION}")
  set(HAVE_SWIG_BINDINGS 1)

  find_package(PerlLibs)
  if(PERLLIBS_FOUND)
    set(HAVE_PERL_BINDINGS 1)
    include_directories(${PERL_INCLUDE_PATH})
    message(STATUS "Have Perl bindings ${HAVE_PERL_BINDINGS}")
  endif(PERLLIBS_FOUND)

  find_package(PythonLibs)
  if(PYTHONLIBS_FOUND)
    set(HAVE_PYTHON_BINDINGS 1)
    include_directories(${PYTHON_INCLUDE_PATH})
    message(STATUS "Have Python bindings ${HAVE_PYTHON_BINDINGS}")
  endif(PYTHONLIBS_FOUND)

  find_package(Ruby)
  if(RUBY_FOUND)
    set(HAVE_RUBY_BINDINGS 1)
    include_directories(${RUBY_INCLUDE_PATH})
    message(STATUS "Have Ruby bindings ${HAVE_RUBY_BINDINGS}")
  endif(RUBY_FOUND)

endif(SWIG_FOUND)

# ----------------------------------------------------
# Finds GraphViz
# ----------------------------------------------------
option(USE_GV_WRITER "Using gv-writer" ON)
message(STATUS "Using gv-writer ${USE_GV_WRITER}")

if (USE_GV_WRITER)
    find_package(Graphviz)
    if (GRAPHVIZ_FOUND)
        message(STATUS "Found GraphViz")
        include_directories(${GRAPHVIZ_INCLUDE_DIRS})
        link_directories(${GRAPHVIZ_LIBRARIES})
        link_directories("${psi-toolkit_BINARY_DIR}/formats/gv/plugin")
        set(HAVE_GRAPHVIZ 1)
    else (GRAPHVIZ_FOUND)
        message(WARNING "GraphViz not found: gv-writer disabled")
        set(HAVE_GRAPHVIZ 0)
    endif (GRAPHVIZ_FOUND)
else (USE_GV_WRITER)
    set(HAVE_GRAPHVIZ 0)
endif (USE_GV_WRITER)

# ----------------------------------------------------
# Finds LibMagic
# ----------------------------------------------------
find_package(LibMagic)
if(LIBMAGIC_FOUND)
    message(STATUS "Found LibMagic")
    include_directories(${LibMagic_INCLUDE_DIRS})
    link_directories(${LibMagic_LIBRARY})
    set(HAVE_LIBMAGIC 1)
else()
    message(WARNING "LibMagic not found")
    set(HAVE_LIBMAGIC 0)
endif(LIBMAGIC_FOUND)

# ----------------------------------------------------
# Finds Poppler
# ----------------------------------------------------
option(USE_PDF_READER "Using pdf-reader" ON)
message(STATUS "Using pdf-reader ${USE_PDF_READER}")

if (USE_PDF_READER)
    find_package(Poppler)
    if (POPPLER_FOUND)
        message(STATUS "Found Poppler")
        include_directories(${POPPLER_INCLUDE_DIRS})
        link_directories(${POPPLER_LIBRARIES})
        link_directories("${psi-toolkit_BINARY_DIR}/formats/pdf/plugin")
        set(HAVE_POPPLER 1)
    else (POPPLER_FOUND)
        message(WARNING "Poppler not found: pdf-reader disabled")
        set(HAVE_POPPLER 0)
    endif (POPPLER_FOUND)
else (USE_PDF_READER)
    set(HAVE_POPPLER 0)
endif (USE_PDF_READER)

# ----------------------------------------------------
# Finds DjVuLibre
# ----------------------------------------------------
option(USE_DJVU_READER "Using djvu-reader" OFF)
message(STATUS "Using djvu-reader ${USE_DJVU_READER}")

if (USE_DJVU_READER)
    find_package(DjVuLibre)
    if (DJVULIBRE_FOUND)
        message(STATUS "Found DjVuLibre")
        include_directories(${DJVULIBRE_INCLUDE_DIRS})
        link_directories(${DJVULIBRE_LIBRARIES})
        link_directories("${psi-toolkit_BINARY_DIR}/formats/djvu/plugin")
        set(HAVE_DJVULIBRE 1)
    else (DJVULIBRE_FOUND)
        message(WARNING "DjVuLibre not found: djvu-reader disabled")
        set(HAVE_DJVULIBRE 0)
    endif (DJVULIBRE_FOUND)
else (USE_DJVU_READER)
    set(HAVE_DJVULIBRE 0)
endif (USE_DJVU_READER)

# ----------------------------------------------------
# Finds Link Grammar
# ----------------------------------------------------
option(USE_LINK_GRAMMAR "Using link-parser" ON)
message(STATUS "Using link-parser ${USE_LINK_GRAMMAR}")

if (USE_LINK_GRAMMAR)
    find_package(LinkGrammar)
    if (LINK_GRAMMAR_FOUND)
        message(STATUS "Found LinkGrammar")
        include_directories(${LINK_GRAMMAR_INCLUDE_DIRS})
        link_directories(${LINK_GRAMMAR_LIBRARIES})
        link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/link/plugin")
        set(HAVE_LINK_GRAMMAR 1)
    else (LINK_GRAMMAR_FOUND)
        message(WARNING "LinkGrammar not found: link-parser disabled")
        set(HAVE_LINK_GRAMMAR 0)
    endif (LINK_GRAMMAR_FOUND)
else (USE_LINK_GRAMMAR)
    set(HAVE_LINK_GRAMMAR 0)
endif (USE_LINK_GRAMMAR)

# ----------------------------------------------------
# Links Antiword plugin
# ----------------------------------------------------
option(USE_DOC_READER "Using doc-reader" OFF)
message(STATUS "Using doc-reader ${USE_DOC_READER}")

if (USE_DOC_READER)
    link_directories("${psi-toolkit_BINARY_DIR}/formats/doc/plugin")
endif (USE_DOC_READER)


# ====================================================
# Sets compiler
# ====================================================

set(CMAKE_CXX_FLAGS "-g -Wall -Wundef -Wnon-virtual-dtor -Wextra")

if(WITH_LOW_MEMORY_COMPILATION)
  message("Setting low memory compilation flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --param ggc-min-expand=30 --param ggc-min-heapsize=8192")
endif(WITH_LOW_MEMORY_COMPILATION)

# ----------------------------------------------------
# BASE_TARGETS
# ----------------------------------------------------
set(BASE_TARGETS common common-plugins utf8case sundown framework
    formats-apertium formats-bracketing formats-dot formats-guessing formats-nkjp formats-psi
    formats-simple formats-json-simple formats-txt formats-utt
    maxent tools-tokenizers-tp tools-segmenters-srx tools-recognizers-langguesser
    tools-parsers-gobio tools-parsers-puddle tools-taggers-metagger)

# ----------------------------------------------------
# TESTS_TARGETS
# ----------------------------------------------------
set(TESTS_TARGETS common-tests framework-tests utf8case-tests
    formats-bracketing-tests formats-dot-tests formats-utt-tests formats-psi-tests
    formats-simple-tests formats-apertium-tests
    tools-tokenizers-tp-tests tools-segmenters-srx-tests tools-datastructures-psi_query-tests
    tools-recognizers-langguesser-tests tools-parsers-gobio-tests tools-parsers-puddle-tests
    tools-taggers-metagger-tests)

set(TARGETS ${BASE_TARGETS} psi-pipe)

if(HAVE_ASPELL)
    set(TESTS_TARGETS ${TESTS_TARGETS} tools-spellcheckers-aspell-tests)
endif(HAVE_ASPELL)

if(USE_JAVA)
    set(BASE_TARGETS ${BASE_TARGETS} tools-lemmatizers-morfologik)
    set(TESTS_TARGETS ${TESTS_TARGETS} tools-lemmatizers-morfologik-tests)
endif(USE_JAVA)

if(HAVE_SFST)
    #add_subdirectory(sfst)
    #set(BASE_TARGETS ${BASE_TARGETS} sfst)
    set(BASE_TARGETS ${BASE_TARGETS} tools-lemmatizers-sfst)
    set(TESTS_TARGETS ${TESTS_TARGETS} tools-lemmatizers-sfst-tests)
endif(HAVE_SFST)

if(HAVE_CMPH)
    set(BASE_TARGETS ${BASE_TARGETS} tools-datastructures-lexica)
    set(TESTS_TARGETS ${TESTS_TARGETS} tools-datastructures-lexica-tests)

    set(BASE_TARGETS ${BASE_TARGETS} tools-lexica-bilexicon)
    set(TESTS_TARGETS ${TESTS_TARGETS} tools-lexica-bilexicon-tests)
endif(HAVE_CMPH)

if(USE_POSTGRESQL)
    set(BASE_TARGETS ${BASE_TARGETS} lex-db)
endif(USE_POSTGRESQL)

if(USE_BONSAI_PARSEGRAPH)
    include_directories(${BONSAI_PATH})
endif(USE_BONSAI_PARSEGRAPH)

if(HAVE_GRAPHVIZ)
    set(BASE_TARGETS ${BASE_TARGETS} formats-gv)
    set(TESTS_TARGETS ${TESTS_TARGETS} formats-gv-tests)
endif(HAVE_GRAPHVIZ)

if(HAVE_POPPLER)
    set(BASE_TARGETS ${BASE_TARGETS} formats-pdf)
endif(HAVE_POPPLER)

if(HAVE_DJVULIBRE)
    set(BASE_TARGETS ${BASE_TARGETS} formats-djvu)
endif(HAVE_DJVULIBRE)

if(HAVE_LINK_GRAMMAR)
    set(BASE_TARGETS ${BASE_TARGETS} tools-parsers-link)
endif(HAVE_LINK_GRAMMAR)

if(USE_DOC_READER)
    set(BASE_TARGETS ${BASE_TARGETS} formats-doc)
endif(USE_DOC_READER)

# ----------------------------------------------------
# Include and link directories
# ----------------------------------------------------
set(DIRS common common/plugins maxent formats tools framework server bindings utf8 utf8case sundown)

foreach(dir ${DIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/${dir}")
    add_subdirectory(${dir})
endforeach(dir)

foreach(dir ${formats_SUBDIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/formats/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/formats/${dir}")
endforeach(dir)

set(ALL_TARGETS ${TARGETS} ${TESTS_TARGETS})

set_target_properties(${ALL_TARGETS} PROPERTIES
    COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8 -I${psi-toolkit_SOURCE_DIR}/utf8case -I${psi-toolkit_SOURCE_DIR}/tests")
if(USE_BONSAI_PARSEGRAPH)
    set_target_properties(tools-parsers-puddle PROPERTIES
    COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8 -I${psi-toolkit_SOURCE_DIR}/utf8case -I${psi-toolkit_SOURCE_DIR}/tests -I${BONSAI_PATH}")
    set_target_properties(tools-parsers-puddle-tests PROPERTIES
    COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8 -I${psi-toolkit_SOURCE_DIR}/utf8case -I${psi-toolkit_SOURCE_DIR}/tests -I${BONSAI_PATH}")
    set(_WITH_BONSAI_PARSEGRAPH 1)
endif(USE_BONSAI_PARSEGRAPH)

add_subdirectory(tests)

configure_file(${psi-toolkit_SOURCE_DIR}/common/config.hpp.in
    ${psi-toolkit_SOURCE_DIR}/common/config.hpp)

if (HAVE_PERL_BINDINGS)
   get_directory_property(PERL_LIBRARY_DIR DIRECTORY bindings/perl/PSIToolkit DEFINITION PERL_LIBRARY_DIR)
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:${PERL_LIBRARY_DIR}")
endif (HAVE_PERL_BINDINGS)

# ====================================================
# CMake functions
# ====================================================

function(download_artifact FILE)
  set(_url "http://mrt.wmi.amu.edu.pl:8080/job/psi-toolkit-data/lastSuccessfulBuild/artifact/psi-toolkit-data/${FILE}")

  message(STATUS "Checking ${_url}...")

  set(_downloaded_file "${psi-toolkit_SOURCE_DIR}/${FILE}")

  set(_skip_download 0)

  if (EXISTS "${_downloaded_file}")
     if ("${CMAKE_VERSION}" VERSION_LESS "2.8.7")
         execute_process(COMMAND "md5sum" "-b" ${_downloaded_file}
        OUTPUT_VARIABLE _file_md5_line
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
         string(REGEX MATCH "^[0-9a-f]+" _file_md5 "${_file_md5_line}")
     else()
        file(MD5 "${_downloaded_file}" _file_md5)
     endif ()

     message(STATUS "File ${_downloaded_file} already there with MD5 ${_file_md5}")
     message(STATUS "Downloading MD5 ${_url}...")

     set(_jenkins_fingerprint_file "${psi-toolkit_BINARY_DIR}/${FILE}.md5")

     file(DOWNLOAD
         "${_url}/*fingerprint*/"
         "${_jenkins_fingerprint_file}"
         STATUS status)

     # :( no '{..}' quantifiers in cmake regexps, should be ">([0-9a-f]{32})<"
     set(_stupid_regexp ">([0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f])<")

     file(STRINGS ${_jenkins_fingerprint_file} _md5_lines REGEX "${_stupid_regexp}")

     string(REGEX MATCH "${_stupid_regexp}" _match "${_md5_lines}")

     set(_remote_md5 "${CMAKE_MATCH_1}")

     message(STATUS "Remote file MD5 is: ${_remote_md5}")

     if ("${_remote_md5}" STREQUAL "${_file_md5}")
        message(STATUS "MD5 is the same, skipping download")
        set(_skip_download 1)
     endif ("${_remote_md5}" STREQUAL "${_file_md5}")
  endif ()

  if (NOT "${_skip_download}")
    message(STATUS "Downloading ${_url}...")
    file(DOWNLOAD
          "${_url}"
          "${psi-toolkit_SOURCE_DIR}/${FILE}"
          STATUS status)

    list(GET status 0 _status )
    list(GET status 1 _status_message )

    if (NOT ${_status} EQUAL 0)
       file(REMOVE "${psi-toolkit_SOURCE_DIR}/${FILE}")
       message(FATAL_ERROR
             "Cannot download ${_url}:${_status_message}. Try running cmake with -DDOWNLOAD_DATA=OFF")
    endif()
  endif()

endfunction(download_artifact)

function(download_and_unpack URL FILE CHECKSUM DESTINATION)
  message(STATUS "Checking ${URL}...")

  set(_downloaded_file "${psi-toolkit_SOURCE_DIR}/${DESTINATION}")

  set(_skip_download 0)

  if (EXISTS "${_downloaded_file}")
     if ("${CMAKE_VERSION}" VERSION_LESS "2.8.7")
         execute_process(COMMAND "md5sum" "-b" ${_downloaded_file}
        OUTPUT_VARIABLE _file_md5_line
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
         string(REGEX MATCH "^[0-9a-f]+" _file_md5 "${_file_md5_line}")
     else()
        file(MD5 "${_downloaded_file}" _file_md5)
     endif ()

     message(STATUS "File ${_downloaded_file} already there with MD5 ${_file_md5}")
     message(STATUS "Excpected MD5 is ${CHECKSUM}...")

     if ("${CHECKSUM}" STREQUAL "${_file_md5}")
        message(STATUS "MD5 is the same, skipping download")
        set(_skip_download 1)
     endif ()
  endif ()

  if (NOT "${_skip_download}")
    message(STATUS "Downloading ${URL}...")

    set(_tmp_archive_file "${psi-toolkit_BINARY_DIR}/pack.tmp.tar.gz")

    message(STATUS "${_tmp_archive_file}")

    file(DOWNLOAD
          "${URL}"
          "${_tmp_archive_file}"
          STATUS status)

    list(GET status 0 _status )
    list(GET status 1 _status_message )

    if (NOT ${_status} EQUAL 0)
       file(REMOVE "${_tmp_archive_file}")
       message(FATAL_ERROR
             "Cannot download ${_url}:${_status_message}. Try running cmake with -DDOWNLOAD_DATA=OFF")
    endif()

    message(STATUS "unpacking into ${_downloaded_file}")

    execute_process(COMMAND "tar" "zxfO" ${_tmp_archive_file} ${FILE}
                    OUTPUT_FILE "${_downloaded_file}")


    if ("${CMAKE_VERSION}" VERSION_LESS "2.8.7")
        execute_process(COMMAND "md5sum" "-b" ${_downloaded_file}
        OUTPUT_VARIABLE _file_md5_line
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
         string(REGEX MATCH "^[0-9a-f]+" _file_md5 "${_file_md5_line}")
    else()
       file(MD5 "${_downloaded_file}" _file_md5)
    endif ()

    if (NOT "${CHECKSUM}" STREQUAL "${_file_md5}")
       message(FATAL_ERROR
             "Wrong MD5 checksum for ${URL}")
    endif()

    file(REMOVE "${_tmp_archive_file}")

  endif()

endfunction(download_and_unpack)

if(DOWNLOAD_DATA)
    download_artifact("tools/lexica/bilexicon/data/plen.bin")

    if (HAVE_SFST)
       download_and_unpack(
           "http://www.let.rug.nl/coltekin/trmorph/trmorph-0.2.1.tar.gz"
           "trmorph-0.2.1/trmorph-0.2.1.a"
           "0d8a94791fce28f862e2890406e81727"
           "tools/lemmatizers/sfst/data/tr.a")
    endif()

endif(DOWNLOAD_DATA)

# ====================================================
# Sets prefix for all generated libraries
# ====================================================

# Helping macro
MACRO(LIST_CONTAINS var value)
    SET(${var})
    FOREACH (value2 ${ARGN})
        IF(${value} STREQUAL ${value2})
            SET(${var} TRUE)
        ENDIF(${value} STREQUAL ${value2})
    ENDFOREACH (value2)
ENDMACRO(LIST_CONTAINS)

GET_CMAKE_PROPERTY(all_variables VARIABLES)

FOREACH(variable_name ${all_variables})
    if(variable_name MATCHES "_LIB_DEPENDS$")
        STRING(REGEX REPLACE "_LIB_DEPENDS.*" "" library_name ${variable_name})

        GET_TARGET_PROPERTY(target_location ${library_name} LOCATION)

        if(NOT target_location MATCHES "NOTFOUND")
            # the prefix 'libpsitoolkit_' will not be added to any library enumerated
            # in regexp below
            if(NOT target_location MATCHES
                "psitoolkit_perl_simple|python.*_PSIToolkit|psitoolkit_ruby_simple")

                SET_TARGET_PROPERTIES(${library_name} PROPERTIES PREFIX ${MY_TARGETLIB_PREFIX})
            endif()
        endif()

    endif()
endforeach(variable_name)

message("RPATH is: ${CMAKE_INSTALL_RPATH}")

cmake_minimum_required(VERSION 2.8)

project(psi-toolkit CXX)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

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

# ----------------------------------------------------

# extracting information from git, see http://stackoverflow.com/questions/1435953/how-can-i-pass-git-sha1-to-compiler-as-definition-using-cmake

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/")
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
git_last_date(GIT_LAST_DATE)

# ----------------------------------------------------

include_directories(${psi-toolkit_SOURCE_DIR})

# Logging
find_library(LOG4CPP_LIB NAMES log4cpp)
find_path(LOG4CPP_INCLUDE log4cpp/Appender.hh)

if(EXISTS ${LOG4CPP_LIB} AND EXISTS ${LOG4CPP_INCLUDE})
  message("-- Using Log4cpp logging...")
  include_directories(${LOG4CPP_INCLUDE})
  link_directories(${LOG4CPP_LIB})
  set(HAVE_LOG4CPP 1)
else()
  message("-- Not using Log4cpp...")
  set(HAVE_LOG4CPP 0)
endif(EXISTS ${LOG4CPP_LIB} AND EXISTS ${LOG4CPP_INCLUDE})

# CMPH library (for perfect hashing)
find_library(CMPH_LIB NAMES cmph)
find_path(CMPH_INCLUDE cmph.h)

if(EXISTS ${CMPH_LIB} AND EXISTS ${CMPH_INCLUDE})
  message("-- Using CMPH...")
  include_directories(${CMPH_INCLUDE})
  link_directories(${CMPH_LIB})
  set(HAVE_CMPH 1)
else()
  message("-- Not using CMPH...")
  set(HAVE_CMPH 0)
endif(EXISTS ${CMPH_LIB} AND EXISTS ${CMPH_INCLUDE})

# Finds and includes external libraries
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS program_options graph serialization filesystem system thread unit_test_framework prg_exec_monitor REQUIRED)

set(PSITOOLKIT_INSTALL_DATA_DIR share/psi-toolkit)
set(INSTALL_DATA_DIR "${CMAKE_INSTALL_PREFIX}/${PSITOOLKIT_INSTALL_DATA_DIR}")

option(IS_INSTALLABLE "Is prepared for install" OFF)
message(STATUS "Is prepared for install ${IS_INSTALLABLE}")

option(USE_JAVA "Compiling Java components" OFF)
message(STATUS "Compiling Java components ${USE_JAVA}")

option(DOWNLOAD_DATA "Downloading data (artifacts)" ON)
message(STATUS "Downloading data (artifacts) ${DOWNLOAD_DATA}")

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

option(USE_POSTGRESQL "Compiling PostgreSQL components" OFF)
message(STATUS "Compiling PostgreSQL components ${USE_POSTGRESQL}")

if(USE_POSTGRESQL)
        find_package(PostgreSQL REQUIRED)
        include_directories(${PostgreSQL_INCLUDE_DIRS})
        link_directories(${PostgreSQL_LIBRARY_DIRS})
        set(HAVE_POSTGRESQL 1)
endif(USE_POSTGRESQL)

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

option(USE_PUDDLE "Compiling puddle shallow parser" ON)
message(STATUS "Compiling puddle shallow parser ${USE_PUDDLE}")

option(USE_BONSAI_PARSEGRAPH "Using bonsai ParseGraph with puddle shallow parser" OFF)
message(STATUS "Using bonsai ParseGraph with puddle shallow parser ${USE_BONSAI_PARSEGRAPH}")

option(BONSAI_PATH "Bonsai common path file (required while using ParseGraph)" "/home/user/poleng4/src/bonsai/common")
message(STATUS "Bonsai common path file (required while using ParseGraph) ${BONSAI_PATH}")

option(USE_STRING_FRAG_VALIDATION "Validating string frags" OFF)
message(STATUS "Validating string frags ${USE_STRING_FRAG_VALIDATION}")

if(USE_STRING_FRAG_VALIDATION)
    set(STRING_FRAG_VALIDATION 1)
endif(USE_STRING_FRAG_VALIDATION)


find_package(SWIG)
if(SWIG_FOUND)

  set(HAVE_SWIG_BINDINGS 1)

  find_package(PerlLibs)
  if (PERLLIBS_FOUND)
    set(HAVE_PERL_BINDINGS 1)
    include_directories(${PERL_INCLUDE_PATH})
  endif(PERLLIBS_FOUND)

  find_package(PythonLibs)
  if (PYTHONLIBS_FOUND)
    set(HAVE_PYTHON_BINDINGS 1)
  endif(PYTHONLIBS_FOUND)
endif(SWIG_FOUND)

# Sets compiler flags
set(CMAKE_CXX_FLAGS "-g -Wall -Wundef -Wnon-virtual-dtor -Wextra")

set(BASE_TARGETS common utf8case sundown framework formats-psi formats-simple formats-txt formats-utt formats-apertium maxent tools-tokenizers-tp tools-segmenters-srx tools-recognizers-langguesser tools-parsers-gobio)

set(TESTS_TARGETS common-tests tools-tokenizers-tp-tests tools-segmenters-srx-tests tools-datastructures-psi_query-tests tools-recognizers-langguesser-tests utf8case-tests formats-utt-tests formats-psi-tests formats-simple-tests tools-parsers-gobio-tests)

if(USE_JAVA)
    set(BASE_TARGETS ${BASE_TARGETS} tools-lemmatizers-morfologik)
    set(TESTS_TARGETS ${TESTS_TARGETS} tools-lemmatizers-morfologik-tests)
endif(USE_JAVA)

if(HAVE_CMPH)
    set(BASE_TARGETS ${BASE_TARGETS} tools-datastructures-lexica)
    set(TESTS_TARGETS ${TESTS_TARGETS} tools-datastructures-lexica-tests)

    set(BASE_TARGETS ${BASE_TARGETS} tools-lexica-bilexicon)
    set(TESTS_TARGETS ${TESTS_TARGETS} tools-lexica-bilexicon-tests)
endif(HAVE_CMPH)

if(USE_POSTGRESQL)
        set(BASE_TARGETS ${BASE_TARGETS} lex-db)
endif(USE_POSTGRESQL)

if(USE_PUDDLE)
        include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/puddle")
        set(BASE_TARGETS ${BASE_TARGETS} tools-parsers-puddle)
        set(TESTS_TARGETS ${TESTS_TARGETS} tools-parsers-puddle-tests)
        if(USE_BONSAI_PARSEGRAPH)
            include_directories(${BONSAI_PATH})
        endif(USE_BONSAI_PARSEGRAPH)
endif(USE_PUDDLE)

if(NOT DEFINED FLEX_PREFIX_OUTPUTS)
    set(FLEX_PREFIX_OUTPUTS FALSE)
endif(NOT DEFINED FLEX_PREFIX_OUTPUTS)

if(NOT DEFINED FLEX_GENERATE_DEFINES)
    set(FLEX_GENERATE_DEFINES FALSE)
endif(NOT DEFINED FLEX_GENERATE_DEFINES)

if(NOT FLEX_EXECUTABLE)
    message(STATUS "Looking for flex")
    find_program(FLEX_EXECUTABLE flex)
    if(FLEX_EXECUTABLE)
        message(STATUS "Looking for flex -- ${FLEX_EXECUTABLE}")
    endif(FLEX_EXECUTABLE)
endif(NOT FLEX_EXECUTABLE)

add_custom_target(
    flex-grlex ALL
    DEPENDS
        ${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/lex.grlex.cpp
)

add_custom_command(
    OUTPUT ${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/lex.grlex.cpp
    COMMAND ${FLEX_EXECUTABLE}
        -P grlex
        -o ${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/lex.grlex.cpp
        ${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/lex.grlex.lpp
    DEPENDS ${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/lex.grlex.lpp
    COMMENT "Generating lex.grlex.cpp"
)

if(NOT DEFINED BISON_PREFIX_OUTPUTS)
    set(BISON_PREFIX_OUTPUTS FALSE)
endif(NOT DEFINED BISON_PREFIX_OUTPUTS)

if(NOT DEFINED BISON_GENERATE_DEFINES)
    set(BISON_GENERATE_DEFINES FALSE)
endif(NOT DEFINED BISON_GENERATE_DEFINES)

if(NOT BISON_EXECUTABLE)
    message(STATUS "Looking for bison")
    find_program(BISON_EXECUTABLE bison)
    if(BISON_EXECUTABLE)
        message(STATUS "Looking for bison -- ${BISON_EXECUTABLE}")
    endif(BISON_EXECUTABLE)
endif(NOT BISON_EXECUTABLE)

add_custom_target(
    bison-grparser ALL
    DEPENDS ${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/grparser.bis.cpp
)

add_custom_command(
    OUTPUT ${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/grparser.bis.cpp
    COMMAND ${BISON_EXECUTABLE}
        -p grparser
        -o ${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/grparser.bis.cpp
        ${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/grparser.y
    DEPENDS ${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/grparser.y
    COMMENT "Generating grparser.bis.cpp"
)

include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/avmatrix")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/chart")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/parser")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/registrar")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/rules")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/transporter")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/trees")

link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/gobio")
link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/gobio/avmatrix")
link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/gobio/chart")
link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/gobio/parser")
link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/gobio/registrar")
link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/gobio/rules")
link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/gobio/translator")
link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/gobio/transporter")
link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/gobio/trees")

set(TARGETS ${BASE_TARGETS} psi-pipe)

set(DIRS common formats maxent tools framework utf8 utf8case server bindings)
foreach(dir ${DIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/${dir}")
    add_subdirectory(${dir})
endforeach(dir)

include_directories("${psi-toolkit_SOURCE_DIR}/sundown/src")
include_directories("${psi-toolkit_SOURCE_DIR}/sundown/html")
include_directories("${psi-toolkit_SOURCE_DIR}/sundown/cpp")
link_directories("${psi-toolkit_BINARY_DIR}/sundown")
add_subdirectory(sundown)

if(USE_PUDDLE)
    set_target_properties(tools-parsers-puddle PROPERTIES
    COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8")
    if(USE_BONSAI_PARSEGRAPH)
        set_target_properties(tools-parsers-puddle PROPERTIES
        COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8 -I${BONSAI_PATH}")
        set(_WITH_BONSAI_PARSEGRAPH 1)
    endif(USE_BONSAI_PARSEGRAPH)
    set(HAVE_PUDDLE 1)
endif(USE_PUDDLE)

foreach(dir ${formats_SUBDIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/formats/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/formats/${dir}")
endforeach(dir)


set(ALL_TARGETS ${TARGETS} ${TESTS_TARGETS})

set_target_properties(${ALL_TARGETS} PROPERTIES
    COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8 -I${psi-toolkit_SOURCE_DIR}/utf8case -I${psi-toolkit_SOURCE_DIR}/tests")
if(USE_PUDDLE)
    if(USE_BONSAI_PARSEGRAPH)
        set_target_properties(tools-parsers-puddle PROPERTIES
        COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8  -I${psi-toolkit_SOURCE_DIR}/tests -I${BONSAI_PATH}")
        set_target_properties(tools-parsers-puddle-tests PROPERTIES
        COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8  -I${psi-toolkit_SOURCE_DIR}/tests -I${BONSAI_PATH}")
    endif(USE_BONSAI_PARSEGRAPH)
endif(USE_PUDDLE)

add_subdirectory(tests)

configure_file(${psi-toolkit_SOURCE_DIR}/common/config.hpp.in ${psi-toolkit_SOURCE_DIR}/common/config.hpp)

function(download_artifact FILE)
  set(_url
      "http://mrt.wmi.amu.edu.pl:8080/job/psi-toolkit-data/lastSuccessfulBuild/artifact/psi-toolkit-data/${FILE}")

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

endfunction(download_artifact)

if(DOWNLOAD_DATA)
        download_artifact("tools/lexica/bilexicon/data/plen.bin")
endif(DOWNLOAD_DATA)

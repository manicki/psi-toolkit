cmake_minimum_required(VERSION 2.8)

project(psi-toolkit CXX)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

include_directories(${psi-toolkit_SOURCE_DIR})

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

set(BASE_TARGETS common utf8case framework formats-psi formats-simple formats-txt formats-utt tools-tokenizers-tp tools-segmenters-srx tools-recognizers-langguesser)

set(TESTS_TARGETS common-tests tools-tokenizers-tp-tests tools-segmenters-srx-tests tools-datastructures-psi_query-tests tools-recognizers-langguesser-tests utf8case-tests)

if(USE_JAVA)
    set(BASE_TARGETS ${BASE_TARGETS} tools-lemmatizers-morfologik)
    set(TESTS_TARGETS ${TESTS_TARGETS} tools-lemmatizers-morfologik-tests)
endif(USE_JAVA)

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

set(TARGETS ${BASE_TARGETS} psi-pipe)

set(DIRS common formats tools framework utf8 utf8case server bindings)
foreach(dir ${DIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/${dir}")
    add_subdirectory(${dir})
endforeach(dir)

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
    COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8 -I${psi-toolkit_SOURCE_DIR}/tests")
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


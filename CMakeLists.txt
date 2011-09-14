cmake_minimum_required(VERSION 2.8)

project(psi-toolkit CXX)

# Finds and includes external libraries
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS program_options graph serialization filesystem system thread unit_test_framework prg_exec_monitor REQUIRED)

option(USE_JAVA "Compiling Java components" OFF)
message(STATUS "Compiling Java components ${USE_JAVA}")

if(USE_JAVA)
	enable_language(Java)

	find_package(Java REQUIRED)
#	message(STATUS "Java runtime ${JAVA_RUNTIME}")
#	message(STATUS "Java compile ${JAVA_COMPILE}")

	find_package(JNI REQUIRED)
#	message(STATUS "JVM library ${JAVA_JVM_LIBRARY}")
#	message(STATUS "JNI path ${JAVA_INCLUDE_PATH}")

	string(REGEX REPLACE "(.*)/libjvm.so" "\\1" LD_LIBPATH ${JAVA_JVM_LIBRARY})
	message(STATUS "You may need to set LD_LIBRARY_PATH=${LD_LIBPATH} to run installed software")

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

# Sets compiler flags
set(CMAKE_CXX_FLAGS "-g -Wall -Wundef -Wnon-virtual-dtor -Wextra")

set(BASE_TARGETS common framework formats-psi formats-simple formats-txt formats-utt tools-tokenizers-tp tools-segmenters-srx)

set(DIRS common formats tools framework utf8 server)
foreach(dir ${DIRS})
	include_directories("${psi-toolkit_SOURCE_DIR}/${dir}")
	link_directories("${psi-toolkit_BINARY_DIR}/${dir}")
	add_subdirectory(${dir})
endforeach(dir)

set(TARGETS ${BASE_TARGETS} psi-pipe)

set(TESTS_TARGETS common-tests tools-tokenizers-tp-tests tools-segmenters-srx-tests)

if(USE_JAVA)
	set(TARGETS ${TARGETS} tools-lemmatizers-morfologik)
        set(TESTS_TARGETS ${TESTS_TARGETS} morfologik-tests)
endif(USE_JAVA)

if(USE_POSTGRESQL)
        set(TARGETS ${TARGETS} lex-db)
endif(USE_POSTGRESQL)


#if(USE_JAVA)
#	set_target_properties(psi-pipe PROPERTIES
#		COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8 -I${psi-toolkit_SOURCE_DIR}/common")
#endif(USE_JAVA)

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

if(USE_PUDDLE)
        include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/puddle")
        set(TARGETS ${TARGETS} tools-parsers-puddle)
        set(TESTS_TARGETS ${TESTS_TARGETS} tools-parsers-puddle-tests)
        if(USE_BONSAI_PARSEGRAPH)
            include_directories(${BONSAI_PATH})
        endif(USE_BONSAI_PARSEGRAPH)
endif(USE_PUDDLE)


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

configure_file(${psi-toolkit_SOURCE_DIR}/common/config.h.in ${psi-toolkit_SOURCE_DIR}/common/config.h)


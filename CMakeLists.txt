cmake_minimum_required(VERSION 2.8)

project(psi-toolkit CXX)

# Finds and includes external libraries
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS program_options graph serialization REQUIRED)

set(CXXTEST_INCLUDE_DIR ./prj/tests)
find_package(CxxTest)

option(USE_JAVA "Compiling Java components" OFF)
message(STATUS "Compiling Java components ${USE_JAVA}")

if(USE_JAVA)
	enable_language(Java)

	find_package(Java REQUIRED)
	message(STATUS "Java runtime ${JAVA_RUNTIME}")
	message(STATUS "Java compile ${JAVA_COMPILE}")

	find_package(JNI REQUIRED)
	message(STATUS "JVM library ${JAVA_JVM_LIBRARY}")
	message(STATUS "JNI path ${JAVA_INCLUDE_PATH}")

	include_directories(${JAVA_INCLUDE_PATH})
	include_directories("${psi-toolkit_SOURCE_DIR}/common/j")
endif(USE_JAVA)

option(USE_POSTGRESQL "Compiling PostgreSQL components" OFF)
message(STATUS "Compiling PostgreSQL components ${USE_POSTGRESQL}")

if(USE_POSTGRESQL)
        find_package(PostgreSQL REQUIRED)
        include_directories(${PostgreSQL_INCLUDE_DIRS})
        link_directories(${PostgreSQL_LIBRARY_DIRS})
        set(HAVE_POSTGRESQL 1)
endif(USE_POSTGRESQL)

option(USE_PUDDLE "Compiling puddle shallow parser" OFF)
message(STATUS "Compiling puddle shallow parser ${USE_PUDDLE}")

option(USE_BONSAI_PARSEGRAPH "Using bonsai ParseGraph with puddle shallow parser" OFF)
message(STATUS "Using bonsai ParseGraph with puddle shallow parser ${USE_BONSAI_PARSEGRAPH}")

option(BONSAI_PATH "Bonsai common path file (required while using ParseGraph)" "/home/user/poleng4/src/bonsai/common")
message(STATUS "Bonsai common path file (required while using ParseGraph) ${BONSAI_PATH}")

# Sets compiler flags
set(CMAKE_CXX_FLAGS "-g -Wall -Wundef -Wnon-virtual-dtor")

set(DIRS common formats framework tools utf8)
foreach(dir ${DIRS})
	include_directories("${psi-toolkit_SOURCE_DIR}/${dir}")
	link_directories("${psi-toolkit_BINARY_DIR}/${dir}")
	add_subdirectory(${dir})
endforeach(dir)

set(TARGETS common framework formats-psi formats-txt formats-utt tools-tokenizers-tp psi-pipe common-test)

if(USE_POSTGRESQL)
        set(TARGETS ${TARGETS} lex-db)
endif(USE_POSTGRESQL)

set_target_properties(${TARGETS} PROPERTIES
	COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8")

if(USE_PUDDLE)
set_target_properties(tools-parsers-puddle PROPERTIES
	COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8")
if(USE_BONSAI_PARSEGRAPH)
set_target_properties(tools-parsers-puddle PROPERTIES
    COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8 -I${BONSAI_PATH} -D_WITH_BONSAI_PARSEGRAPH")
#   message(STATUS "flagi ${CMAKE_CXX_FLAGS}")
endif(USE_BONSAI_PARSEGRAPH)
endif(USE_PUDDLE)

foreach(dir ${formats_SUBDIRS})
	include_directories("${psi-toolkit_SOURCE_DIR}/formats/${dir}")
	link_directories("${psi-toolkit_BINARY_DIR}/formats/${dir}")
endforeach(dir)

# Runs CxxTests
if(CXXTEST_FOUND)
	include_directories(${CXXTEST_INCLUDE_DIR})
	enable_testing()

	CXXTEST_ADD_TEST(runner runner.cpp ${morfologik-t} ${common-t} ${puddle-t})
	target_link_libraries(runner
		${Boost_LIBRARIES}
		common common-test
		formats-psi formats-txt formats-utt
	)

	if(USE_JAVA)
		target_link_libraries(runner tools-lemmatizers-morfologik)
	endif(USE_JAVA)

    if(USE_PUDDLE)
        include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/puddle")
        #   link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/puddle")
        target_link_libraries(runner tools-parsers-puddle re2)
    endif(USE_PUDDLE)
endif()

configure_file(${psi-toolkit_SOURCE_DIR}/common/config.h.in ${psi-toolkit_SOURCE_DIR}/common/config.h)


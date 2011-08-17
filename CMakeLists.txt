cmake_minimum_required(VERSION 2.8)

project(psi-toolkit CXX Java)

# Finds and includes external libraries
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS program_options graph serialization REQUIRED)

set(CXXTEST_INCLUDE_DIR ./prj/tests)
find_package(CxxTest)

option(USE_JAVA "Compiling Java components" OFF)
message(STATUS "Compiling Java components ${USE_JAVA}")

if(USE_JAVA)
	find_package(Java REQUIRED)
	message(STATUS "Java runtime ${JAVA_RUNTIME}")
	message(STATUS "Java compile ${JAVA_COMPILE}")

	find_package(JNI REQUIRED)
	message(STATUS "JVM library ${JAVA_JVM_LIBRARY}")
	message(STATUS "JNI path ${JAVA_INCLUDE_PATH}")
	
	include_directories(${JAVA_INCLUDE_PATH})
	include_directories("${psi-toolkit_SOURCE_DIR}/common/j")
endif(USE_JAVA)

option(USE_PUDDLE "Compiling puddle shallow parser" OFF)
message(STATUS "Compiling puddle shallow parser ${USE_PUDDLE}")

# Sets compiler flags
set(CMAKE_CXX_FLAGS "-g -Wall -Wno-deprecated")

set(DIRS common formats framework tools utf8)
foreach(dir ${DIRS})
	include_directories("${psi-toolkit_SOURCE_DIR}/${dir}")
	link_directories("${psi-toolkit_BINARY_DIR}/${dir}")
	add_subdirectory(${dir})
endforeach(dir)

set_target_properties(common framework formats-psi formats-txt formats-utt tools-tokenizers-tp psi-pipe PROPERTIES 
	COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/utf8")

foreach(dir ${formats_SUBDIRS})
	include_directories("${psi-toolkit_SOURCE_DIR}/formats/${dir}")
	link_directories("${psi-toolkit_BINARY_DIR}/formats/${dir}")
endforeach(dir)

# Runs CxxTests
if(CXXTEST_FOUND)
	include_directories(${CXXTEST_INCLUDE_DIR})
	enable_testing()

	CXXTEST_ADD_TEST(runner runner.cpp ${morfologik-t} ${common-t})
	target_link_libraries(runner 
		${Boost_LIBRARIES} 
		common common-test
		formats-psi formats-txt formats-utt 
	)

	if(USE_JAVA)
		target_link_libraries(runner tools-lemmatizers-morfologik)
	endif(USE_JAVA)
endif()

find_package(SWIG)

include(${SWIG_USE_FILE})

find_package(Ruby)

# getting installation path
execute_process(COMMAND
    ${RUBY_EXECUTABLE} -r rbconfig -e "print Config::CONFIG['vendorarchdir']"
    OUTPUT_VARIABLE RUBY_VENDOR_ARCH_DIR
)

message(STATUS "Ruby executable: ${RUBY_EXECUTABLE}")
message(STATUS "Ruby vendor arch dir: ${RUBY_VENDOR_ARCH_DIR}")
message(STATUS "Ruby version: ${RUBY_VERSION}")
message(STATUS "Ruby include path: ${RUBY_INCLUDE_PATH}")

set(INSTALL_RUBY_MODULES_PATH "lib/ruby/1.8/")
if("${CMAKE_INSTALL_RPATH}" MATCHES "/local(/|$)")
    STRING(REGEX REPLACE "((^|/)local/)" "" INSTALL_RUBY_MODULES_PATH ${INSTALL_RUBY_MODULES_PATH})
endif()

include_directories(${RUBY_INCLUDE_DIRS})

set(CMAKE_SWIG_FLAGS "")
set(CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY "${CMAKE_SWIG_BINDINGS_DIRECTORY}/ruby")
set(WRAPPER_INTERFACE_FILES PSIToolkitSimple.i)

foreach(filename ${WRAPPER_INTERFACE_FILES})
    set_source_files_properties(${filename} PROPERTIES CPLUSPLUS ON)
    set_source_files_properties(${filename} PROPERTIES SWIG_FLAGS "")
endforeach(filename)

set(CXX_FILES PSIToolkitSimpleRUBY_wrap.cxx)

# turning off warnings in generated files
foreach(filename ${CXX_FILES})
    set_source_files_properties(${filename} PROPERTIES COMPILE_FLAGS "-w")
endforeach(filename)

swig_add_module(psitoolkit_ruby_simple ruby PSIToolkitSimple.i)
swig_link_libraries(psitoolkit_ruby_simple ${RUBY_LIBRARY} ${BASE_TARGETS})


configure_file(${psi-toolkit_SOURCE_DIR}/bindings/ruby/psitoolkit.rb
    ${CMAKE_CURRENT_BINARY_DIR}/psitoolkit.rb COPYONLY)

INSTALL(FILES ${CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY}/psitoolkit.rb
    DESTINATION ${INSTALL_RUBY_MODULES_PATH})
INSTALL(FILES ${CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY}/libpsitoolkit_ruby_simple.so
    DESTINATION ${INSTALL_RUBY_MODULES_PATH})

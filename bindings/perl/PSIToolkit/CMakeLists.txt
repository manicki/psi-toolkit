include(${SWIG_USE_FILE})

find_package(PerlLibs)

set(${INSTALL_MODULES_PATH} ${PERL_VENDORLIB})
STRING(REGEX REPLACE "^(/usr/)" "" INSTALL_PERL_MODULES_PATH ${PERL_VENDORLIB})

if("${CMAKE_INSTALL_RPATH}" MATCHES "/local(/|$)")
    STRING(REGEX REPLACE "((^|/)local/)" "" INSTALL_PERL_MODULES_PATH ${INSTALL_PERL_MODULES_PATH})
endif()

set(CMAKE_SWIG_FLAGS "")
set(CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY "${CMAKE_SWIG_BINDINGS_DIRECTORY}/perl/PSIToolkit")
set(WRAPPER_INTERFACE_FILES PSIToolkitSimple.i)

foreach(filename ${WRAPPER_INTERFACE_FILES})
    set_source_files_properties(${filename} PROPERTIES CPLUSPLUS ON)
    set_source_files_properties(${filename} PROPERTIES SWIG_FLAGS "")
endforeach(filename)

set(CXX_FILES PSIToolkitSimplePERL_wrap.cxx)

# turning off warnings in generated files
foreach(filename ${CXX_FILES})
    set_source_files_properties(${filename} PROPERTIES COMPILE_FLAGS "-w")
endforeach(filename)

swig_add_module(psitoolkit_perl_simple perl PSIToolkitSimple.i)
swig_link_libraries(psitoolkit_perl_simple ${BASE_TARGETS})

link_directories("${psi-toolkit_BINARY_DIR}/bindings/perl/PSIToolkit")

install(TARGETS psitoolkit_perl_simple DESTINATION lib)
INSTALL(FILES ${CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY}/psitoolkit_perl_simple.pm
    DESTINATION ${INSTALL_PERL_MODULES_PATH}
)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/proxy/PSIToolkit/Simple.pm
    DESTINATION ${INSTALL_PERL_MODULES_PATH}/PSIToolkit/
)

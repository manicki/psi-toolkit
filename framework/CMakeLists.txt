# don't want to see warnings about deprecated hash_maps/sets used
# in SFST
if (HAVE_SFST)
   add_definitions(-Wno-deprecated -DSGI__gnu_cxx)
endif (HAVE_SFST)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/git_info.cpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/git_info.cpp" @ONLY)

# ----------------------------------------------------
# library framework
# ----------------------------------------------------
add_library(framework SHARED
    aliaser.cpp
    auto_completer.cpp
    factories_keeper.cpp
    main_factories_keeper.cpp
    help_formatter.cpp
    console_help_formatter.cpp
    html_help_formatter.cpp
    pipe_runner.cpp
    version_information.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/git_info.cpp"
)

target_link_libraries(framework
    ${Boost_LIBRARIES} pthread ${BASE_TARGETS} pcre pcrecpp test-extractor)

if (WITH_RE2)
    target_link_libraries(framework re2)
endif(WITH_RE2)

if (HAVE_SFST)
    target_link_libraries(framework sfst)
endif(HAVE_SFST)

foreach(dir ${formats_SUBDIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/formats/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/formats/${dir}")
endforeach(dir)

foreach(dir ${tools_DIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/tools/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/tools/${dir}")
endforeach(dir)

include_directories("${psi-toolkit_SOURCE_DIR}/tests")
link_directories("${psi-toolkit_BINARY_DIR}/tests")

# ----------------------------------------------------
# executable psi-pipe
# ----------------------------------------------------
add_executable(psi-pipe psi-pipe.cpp)

target_link_libraries(psi-pipe ${Boost_LIBRARIES} framework common common-plugins maxent)

include_directories("${psi-toolkit_SOURCE_DIR}/maxent")

if(USE_JAVA)
    target_link_libraries(psi-pipe tools-lemmatizers-morfologik)
endif(USE_JAVA)

if(HAVE_SFST)
    target_link_libraries(psi-pipe tools-lemmatizers-sfst)
endif(HAVE_SFST)

if(USE_POSTGRESQL)
    target_link_libraries(psi-pipe lex-db)
endif(USE_POSTGRESQL)

if (USE_BONSAI_PARSEGRAPH)
    include_directories("${BONSAI_PATH}")
endif (USE_BONSAI_PARSEGRAPH)
if (WITH_RE2)
    target_link_libraries(psi-pipe tools-parsers-puddle re2)
    if (WITH_PCRE)
        target_link_libraries(psi-pipe pcrecpp)
    endif(WITH_PCRE)
else(WITH_RE2)
    if (WITH_PCRE)
        target_link_libraries(psi-pipe tools-parsers-puddle pcrecpp)
    endif(WITH_PCRE)
endif(WITH_RE2)

if(HAVE_PERL_BINDINGS)
    include_directories("${psi-toolkit_SOURCE_DIR}/bindings/perl/writers")
    link_directories("${psi-toolkit_BINARY_DIR}/bindings/perl/writers")

    target_link_libraries(psi-pipe formats-perl-simple formats-simple formats-json-simple)
    target_link_libraries(framework formats-perl-simple)

    find_package(SWIG)
    include(${SWIG_USE_FILE})
    find_package(PerlLibs)
endif(HAVE_PERL_BINDINGS)

add_subdirectory(t)

if (HAVE_ASPELL)
  target_link_libraries(framework tools-spellcheckers-aspell)
  target_link_libraries(psi-pipe tools-spellcheckers-aspell)
endif (HAVE_ASPELL)

# ----------------------------------------------------
# install framework
# ----------------------------------------------------
install(TARGETS framework DESTINATION lib/)
install(TARGETS psi-pipe DESTINATION bin/)
install(FILES DESTINATION "${psi-toolkit_BINARY_DIR}")
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/framework)

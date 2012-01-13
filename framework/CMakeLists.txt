add_library(framework SHARED
    factories_keeper.cpp
    main_factories_keeper.cpp
    help_formatter.cpp
    console_help_formatter.cpp
    html_help_formatter.cpp
    pipe_runner.cpp
)

target_link_libraries(framework ${Boost_LIBRARIES} pthread mpfd-parser ${BASE_TARGETS} pcre pcrecpp tools-parsers-puddle)
if (WITH_RE2)
    target_link_libraries(framework re2)
endif(WITH_RE2)

foreach(dir ${formats_SUBDIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/formats/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/formats/${dir}")
endforeach(dir)

foreach(dir ${tools_DIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/tools/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/tools/${dir}")
endforeach(dir)

add_executable(psi-pipe psi-pipe.cpp)
target_link_libraries(psi-pipe ${Boost_LIBRARIES} framework common maxent)

include_directories("${psi-toolkit_SOURCE_DIR}/maxent")

if(USE_JAVA)
    include_directories("${psi-toolkit_SOURCE_DIR}/tools/lemmatizers/morfologik")
    link_directories("${psi-toolkit_BINARY_DIR}/tools/lemmatizers/morfologik")
    target_link_libraries(psi-pipe tools-lemmatizers-morfologik)
endif(USE_JAVA)

if(USE_POSTGRESQL)
        include_directories("${psi-toolkit_SOURCE_DIR}/tools/lemmatizers/lex_db")
    link_directories("${psi-toolkit_BINARY_DIR}/tools/lemmatizers/lex_db")
        target_link_libraries(psi-pipe lex-db)
endif(USE_POSTGRESQL)

if(USE_PUDDLE)
    include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/puddle")
    if (USE_BONSAI_PARSEGRAPH)
        include_directories("${BONSAI_PATH}")
    endif (USE_BONSAI_PARSEGRAPH)
    link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/puddle")
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

endif(USE_PUDDLE)

if(HAVE_PERL_BINDINGS)
    include_directories("${psi-toolkit_SOURCE_DIR}/bindings/perl/writers")
    link_directories("${psi-toolkit_BINARY_DIR}/bindings/perl/writers")
    target_link_libraries(psi-pipe formats-perl-simple)

    find_package(SWIG)
    include(${SWIG_USE_FILE})
    find_package(PerlLibs)
endif(HAVE_PERL_BINDINGS)

install(TARGETS framework DESTINATION lib/)
install(TARGETS psi-pipe DESTINATION bin/)
install(FILES DESTINATION "${psi-toolkit_BINARY_DIR}")

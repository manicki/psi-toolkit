add_library(tools-parsers-gobio SHARED
    gobio.cpp
)

target_link_libraries(tools-parsers-gobio ${Boost_LIBRARIES})

add_subdirectory(parser)

install(TARGETS tools-parsers-gobio DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/gobio)

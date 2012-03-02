add_library(tools-taggers-metagger SHARED
    me_tagger.cpp
)
target_link_libraries(tools-taggers-metagger common maxent)

add_subdirectory(t)

install(TARGETS tools-taggers-metagger DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/metagger)

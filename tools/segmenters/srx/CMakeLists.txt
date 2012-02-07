add_library(tools-segmenters-srx SHARED
        srx_rules.cpp
        srx_segmenter.cpp
)

add_subdirectory(t)

install(TARGETS tools-segmenters-srx DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/srx-segmenter)
install(DIRECTORY m/help/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/srx-segmenter/help-examples)

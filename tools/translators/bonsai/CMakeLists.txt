add_library(tools-translators-bonsai SHARED
    bonsai.cpp
)
target_link_libraries(tools-translators-bonsai common)

install(TARGETS tools-translators-bonsai DESTINATION lib/)

add_library(tools-taggers-metagger SHARED
    me_tagger.cpp
)
target_link_libraries(tools-taggers-metagger common)

install(TARGETS tools-taggers-metagger DESTINATION lib/)

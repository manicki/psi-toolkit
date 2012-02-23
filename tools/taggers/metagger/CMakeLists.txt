add_library(tools-taggers-metagger SHARED
    me_tagger.cpp
)
target_link_libraries(tools-taggers-metagger common maxent)

install(TARGETS tools-taggers-metagger DESTINATION lib/)

enable_language(C)

add_library(sundown SHARED
    src/autolink.c
    src/buffer.c
    src/markdown.c
    src/stack.c
)

set_target_properties(sundown PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS sundown DESTINATION lib/)

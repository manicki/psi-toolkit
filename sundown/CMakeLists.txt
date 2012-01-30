enable_language(C)

add_library(sundown SHARED
    src/autolink.c
    src/buffer.c
    src/markdown.c
    src/stack.c
    html/html.c
    html/html_smartypants.c
    html/houdini_href_e.c
    html/houdini_html_e.c
    cpp/stringwrapper.cpp
)

set_target_properties(sundown PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS sundown DESTINATION lib/)

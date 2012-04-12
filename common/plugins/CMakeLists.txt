add_library(common-plugins SHARED
    plugin_manager.cpp
)

target_link_libraries(
  common-plugins
  dl
)

install(TARGETS common-plugins DESTINATION lib/)

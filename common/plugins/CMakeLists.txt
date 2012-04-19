include_directories("${psi-toolkit_SOURCE_DIR}/tools/spellcheckers/aspell")
include_directories("${psi-toolkit_SOURCE_DIR}/formats/doc")

foreach(dir ${tools_DIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/tools/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/tools/${dir}")
endforeach(dir)

add_library(common-plugins SHARED
    plugin_manager.cpp
    abstract_plugin.cpp
    plugin_requirements_checker.cpp
    ubuntu_plugin_requirements_checker.cpp
    archlinux_plugin_requirements_checker.cpp
)

target_link_libraries(
  common-plugins
  dl
)

install(TARGETS common-plugins DESTINATION lib/)

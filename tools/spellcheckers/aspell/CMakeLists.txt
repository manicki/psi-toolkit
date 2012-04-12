add_library(tools-spellcheckers-aspell SHARED
	psi_aspell.cpp
)

target_link_libraries(
  tools-spellcheckers-aspell
  common
  common-plugins
)

add_subdirectory(t)
add_subdirectory(plugin)

install(TARGETS tools-spellcheckers-aspell DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/aspell-spellchecker)


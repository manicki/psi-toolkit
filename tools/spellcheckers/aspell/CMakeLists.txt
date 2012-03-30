add_library(tools-spellcheckers-aspell SHARED
	psi_aspell.cpp
)

target_link_libraries(
  tools-spellcheckers-aspell
  common aspell
)

add_subdirectory(t)

install(TARGETS tools-spellcheckers-aspell DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/aspell-spellchecker)


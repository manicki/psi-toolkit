add_library(tools-spellcheckers-aspell-adapter SHARED
	aspell_adapter.cpp
)

target_link_libraries(
  tools-spellcheckers-aspell-adapter
  aspell
)

install(TARGETS tools-spellcheckers-aspell-adapter DESTINATION lib/)



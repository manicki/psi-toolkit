add_library(tools-spellcheckers-aspell-plugin SHARED
	aspell_adapter_impl.cpp
)

target_link_libraries(
  tools-spellcheckers-aspell-plugin
  aspell
)

install(TARGETS tools-spellcheckers-aspell-plugin DESTINATION lib/)


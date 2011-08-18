if(USE_POSTGRESQL)
  add_subdirectory(lex_db)
endif(USE_POSTGRESQL)

if(USE_JAVA)
  add_subdirectory(morfologik)
else()
  unset(morfologik-t CACHE)
endif(USE_JAVA)


if(USE_POSTGRESQL)
  add_subdirectory(lex_db)
endif(USE_POSTGRESQL)

if(USE_JAVA)
  add_subdirectory(morfologik)
else()
  unset(morfologik-t CACHE)
endif(USE_JAVA)

if(HAVE_SFST)
  add_subdirectory(sfst)
endif(HAVE_SFST)

if(HAVE_HFST)
  add_subdirectory(hfst)
endif(HAVE_HFST)

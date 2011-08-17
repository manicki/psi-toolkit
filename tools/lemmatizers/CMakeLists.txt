if(USE_JAVA)
  add_subdirectory(morfologik)
else()
  unset(morfologik-t CACHE)
endif(USE_JAVA)


if(USE_PUDDLE)
  add_subdirectory(puddle)
else()
  unset(puddle-t CACHE)
endif(USE_PUDDLE)

add_subdirectory(gobio)

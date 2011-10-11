add_library(lex-db SHARED
        lex_db_lemmatizer.cpp
)
target_link_libraries(lex-db common pqxx)

install(TARGETS lex-db DESTINATION "${psi-toolkit_BINARY_DIR}")

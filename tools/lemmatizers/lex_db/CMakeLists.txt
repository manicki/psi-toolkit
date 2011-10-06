add_library(lex-db SHARED
        lex_db_lemmatizer.cpp
)
target_link_libraries(lex-db common pqxx)

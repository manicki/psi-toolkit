add_library(lex-db
        lex_db_lemmatizer.cpp
)
target_link_libraries(lex-db common pqxx)

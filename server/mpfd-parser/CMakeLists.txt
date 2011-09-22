add_library(mpfd-parser
    Parser.cpp
    Field.cpp
    Exception.cpp
)

set_target_properties(mpfd-parser PROPERTIES
    COMPILE_FLAGS "-I${psi-toolkit_SOURCE_DIR}/server/mpdf-parser")

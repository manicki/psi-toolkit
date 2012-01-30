add_library(formats-utt SHARED
    utt_lattice_reader.cpp
    utt_quoter.cpp
)

add_subdirectory(t)

install(TARGETS formats-utt DESTINATION lib/)

add_library(maxent SHARED
    display.cpp
    gistrainer.cpp
    maxent.cpp
#    maxent_cmdline.c
    maxent_cmdline.cpp
    maxentmodel.cpp
#    mmapfile.c
    mmapfile.cpp
    modelfile.cpp
    trainer.cpp
)

target_link_libraries(maxent ${Boost_LIBRARIES})

install(TARGETS maxent DESTINATION lib/)

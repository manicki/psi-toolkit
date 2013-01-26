add_definitions(-Wno-sign-compare -Wno-reorder -Wno-unused-parameter -Wno-write-strings)

add_library(tools-translators-bonsai SHARED
    bonsai.cpp
    irstlm/mfstream.cpp
    irstlm/lmmacro.cpp
    irstlm/htable.cpp
    irstlm/mempool.cpp
    irstlm/util.cpp
    irstlm/ngramtable.cpp
    irstlm/n_gram.cpp
    irstlm/lmtable.cpp
    irstlm/dictionary.cpp
    irstlm/ngramcache.cpp
    rules/Huffman.cpp
    rules/CompressedDAG.cpp
    rules/HuffedWords.cpp
    rules/RuleSymbolNumberMap.cpp
    rules/RuleSet.cpp
    rules/RuleLoader.cpp
    rules/SimpleDAG.cpp
    translator/Translation.cpp
    translator/LmContainer.cpp
    translator/Translator.cpp
    common/ParseGraph.cpp
    common/TransitionInfo.cpp
    common/Transformation.cpp
    common/TransferTypes.cpp
)

set(bonsai_SUBDIRS irstlm rules translator common CACHE PATH "Bonsai subdirectories" FORCE)

foreach(subdir ${bonsai_SUBDIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/tools/translators/bonsai/${subdir}")
    link_directories("${psi-toolkit_BINARY_DIR}/tools/translators/bonsai/${subdir}")
    add_subdirectory(${subdir})
endforeach(subdir)

target_link_libraries(tools-translators-bonsai ${Boost_LIBRARIES} z common)

install(TARGETS tools-translators-bonsai DESTINATION lib/)

#add_executable(bonsaibin bonsaibin.cpp)
#target_link_libraries(bonsaibin ${Boost_LIBRARIES} tools-translators-bonsai)
#install(TARGETS bonsaibin DESTINATION bin/)

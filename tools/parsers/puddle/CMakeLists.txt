if(USE_BONSAI_PARSEGRAPH)
	set(common-bonsai-files
        ${BONSAI_PATH}/ParseGraph.cpp
        ${BONSAI_PATH}/TransitionInfo.cpp
        ${BONSAI_PATH}/TransferTypes.cpp
	)
endif(USE_BONSAI_PARSEGRAPH)

add_library(tools-parsers-puddle
	puddle.cpp
	tagset_loader.cpp
	rule_loader.cpp
	tagger.cpp
	tagset.cpp
	action.cpp
	add_action.cpp
	attach_action.cpp
	delete_action.cpp
	group_action.cpp
	join_action.cpp
	syntok_action.cpp
	transform_action.cpp
	unify_action.cpp
	puddle_util.cpp
	rule.cpp
	rule_matcher.cpp
	rule_modifier.cpp
	lattice_wrapper.cpp
    ${common-bonsai-files}
)
target_link_libraries(tools-parsers-puddle ${Boost_LIBRARIES} common)

add_subdirectory(t)


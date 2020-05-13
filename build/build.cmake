set(root "${CMAKE_CURRENT_LIST_DIR}/..")

set(SRCS
    ${root}/src/main.cpp
)

set(BUILD
    ${root}/build/build.cmake
)

set(TEST_FILES
    ${root}/test_assets/shader.crsm
)

add_executable(embed  
	${SRCS} 
	${BUILD}
	${TEST_FILES}
)

settingsCR(embed)	
createPCH(embed)
			
target_link_libraries(embed 
	cli11
	fmt
    glm
	spdlog
	core
	platform
)

source_group("Test Files" FILES ${TEST_FILES})
	
add_custom_command(TARGET embed POST_BUILD       
  COMMAND ${CMAKE_COMMAND} -E copy_if_different  
      ${TEST_FILES}
      $<TARGET_FILE_DIR:embed>) 
		
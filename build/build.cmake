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
			
add_compile_definitions(DOCTEST_CONFIG_DISABLE)

target_precompile_headers(embed PRIVATE 
	<3rdParty/cli11.h>
	<3rdParty/doctest.h>
	<3rdParty/fmt.h>
	<3rdParty/function2.h>
	<3rdParty/spdlog.h>
)

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
		
configure_file(CMakeLists.txt.rapidcheck rapidcheck-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/rapidcheck-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/rapidcheck-download"
)
add_subdirectory("${CMAKE_CURRENT_BINARY_DIR}/rapidcheck-download/rapidcheck"
    "${CMAKE_CURRENT_BINARY_DIR}/rapidcheck-download/rapidcheck"
    EXCLUDE_FROM_ALL)

add_executable( bignumtest
  main.cpp )

target_link_libraries(bignumtest
	bignumlib
	rapidcheck)

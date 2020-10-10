# include rapidcheck
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

# include gtest
configure_file(CMakeLists.txt.gtest googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
	${CMAKE_CURRENT_BINARY_DIR}/googletest-build
	EXCLUDE_FROM_ALL)
	
# include rapidcheck_gtest
add_subdirectory("${CMAKE_CURRENT_BINARY_DIR}/rapidcheck-download/rapidcheck/extras/gtest"
    "${CMAKE_CURRENT_BINARY_DIR}/rapidcheck-download/rapidcheck/extras/gtest"
    EXCLUDE_FROM_ALL)

add_executable(bignumtest
  AdditionTests.cpp
  SubstractionTests.cpp
  MultiplicationTests.cpp
  main.cpp)

target_link_libraries(bignumtest
	bignumlib
	rapidcheck
	rapidcheck_gtest
	gtest)
	
target_include_directories(bignumtest PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/rapidcheck-download/rapidcheck/ext/gmock/gtest/include)
	


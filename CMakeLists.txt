cmake_minimum_required(VERSION 2.8)

configure_file(CMakeLists.txt.rapidcheck rapidcheck-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/rapidcheck-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/rapidcheck-download"
)
add_subdirectory("${CMAKE_BINARY_DIR}/rapidcheck-download/rapidcheck")

project( bignum )
add_executable( bignum
  BigNum.cpp
  main.cpp )

target_link_libraries(bignum rapidcheck)

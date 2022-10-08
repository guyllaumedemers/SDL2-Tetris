cmake_minimum_required(
    VERSION
    3.20.0
)

# add subdirectories

add_subdirectory("${CMAKE_SOURCE_DIR}/doc" "${CMAKE_BINARY_DIR}/doc")
add_subdirectory("${CMAKE_SOURCE_DIR}/extern" "${CMAKE_BINARY_DIR}/extern")
add_subdirectory("${CMAKE_SOURCE_DIR}/src" "${CMAKE_BINARY_DIR}/bin")
add_subdirectory("${CMAKE_SOURCE_DIR}/test" "${CMAKE_BINARY_DIR}/test")

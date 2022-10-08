cmake_minimum_required(
    VERSION
    3.20.0
)

project(
	Root
	VERSION 0.0.0.0
	DESCRIPTION "Root"
)

# add subdirectories

add_subdirectory("doc")
add_subdirectory("extern")
add_subdirectory("src" "${CMAKE_BINARY_DIR}/bin")
add_subdirectory("test")

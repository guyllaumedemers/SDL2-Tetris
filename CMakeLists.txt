cmake_minimum_required(
    VERSION
    3.21
)

project(
	Root
	VERSION 0.0.0.0
	DESCRIPTION "Root"
)

# Update Git submodule, updated version to point to top level cmakelists
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# add subdirectories

add_subdirectory("doc")
add_subdirectory("extern")
add_subdirectory("src" "${CMAKE_BINARY_DIR}/bin")
add_subdirectory("test")

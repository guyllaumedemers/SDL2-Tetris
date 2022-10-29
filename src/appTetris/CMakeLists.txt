project(
	Demo
    VERSION 0.0.0.0
    DESCRIPTION "Demo"
	LANGUAGES C CXX
)

# project includes, sources

file(
	GLOB_RECURSE
	MYPROJECT_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	*.cpp
)

# project executable

add_executable(
	${PROJECT_NAME}
	Main.cpp
	${MYPROJECT_SOURCES}
)

add_dependencies(
	${PROJECT_NAME}
	lib::imgui
	lib::sdl2
	lib::sdlimage
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE lib::imgui
	PRIVATE lib::sdl2
	PRIVATE lib::sdlimage
)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	CMAKE_CXX_COMPILE_FEATURES cxx_std_20
	CMAKE_CXX_STANDARD 20
	CMAKE_CXX_STANDARD_REQUIRED ON
	CMAKE_CXX_EXTENSIONS OFF
)

target_compile_options(
	${PROJECT_NAME}
	PUBLIC CMAKE_CXX_STANDARD
	PUBLIC CMAKE_CXX_STANDARD_REQUIRED
	PUBLIC CMAKE_CXX_EXTENSIONS
)

# check support for LINK Time Optimization between translation units
# include built-in MODULE

include(
	CheckIPOSupported
)

# make call to MODULE function

check_ipo_supported(
	RESULT IS_IPO_SUPPORTED
	OUTPUT IPO_SUPPORT_LOG_OUTPUT
	LANGUAGES C CXX
)

if(NOT ${IS_IPO_SUPPORTED})
	message(WARNING "IPO ADD ON SUPPORTED : ${IS_IPO_SUPPORTED}. ${IPO_SUPPORT_LOG_OUTPUT}")
else()
	set(
		CMAKE_INTERPROCEDURAL_OPTIMIZATION
		TRUE
	)
endif()

# check compiler features supported

set(
	PRINT_UNSUPPORTED_COMPILE_FEATURES
	FALSE
)

if(${PRINT_UNSUPPORTED_COMPILE_FEATURES})
	foreach(VAR IN LISTS CMAKE_CXX_COMPILE_FEATURES)
		list(FIND CMAKE_CXX_COMPILE_FEATURES ${VAR} RESULT COMPILE_FEATURE_SUPPORT_RESULT)
		if(NOT ${COMPILE_FEATURE_SUPPORT_RESULT})
			message(WARNING "WARNING! FEATURE NOT SUPPORTED : ${VAR}")
		endif()
	endforeach()
endif()

# copy-move dll at executable level

add_custom_command ( TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
	COMMAND_EXPAND_LISTS
)

# copy res in out-of-source build

add_custom_command ( TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/res" "${PROJECT_BINARY_DIR}/res"
)
﻿project(
	libBoost
)

# add directory for lib

add_library(
	${PROJECT_NAME}
	INTERFACE
)

add_library(
	lib::boost
	ALIAS
	${PROJECT_NAME}
)

add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/pkg"
	EXCLUDE_FROM_ALL
)

# currently has issues in boost CMakeLists and cannot link properly

target_include_directories(
	${PROJECT_NAME}
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
	${PROJECT_NAME}
	INTERFACE
	Boost::filesystem
)
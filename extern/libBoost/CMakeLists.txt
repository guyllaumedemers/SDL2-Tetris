project(
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

#include(
#	BoostLibraryIncludes.cmake
#	BOOST_MODULE
#)
#
#if(BOOST_MODULE EQUAL NOTFOUND)
#	message("${PROJECT_NAME}.cmake MODULE NOT FOUND")
#endif()

## require to access the .cmake from pkg/cmake

#generate_boost_include_paths(BOOST_INCLUDES)

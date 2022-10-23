project(
	libSDLImage	
)

# add directory for lib

add_library(
	${PROJECT_NAME}
	INTERFACE
)

add_library(
	lib::sdlimage
	ALIAS
	${PROJECT_NAME}
)

# avoid pkg/CMakeLists from running

add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/pkg"
	EXCLUDE_FROM_ALL
)

# include headers

target_include_directories(
	${PROJECT_NAME}
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/pkg"
)

# link src

target_link_libraries(
	${PROJECT_NAME}
	INTERFACE
	SDL2_image::SDL2_image
)
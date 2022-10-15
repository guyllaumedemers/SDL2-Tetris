project(
	libSDL2	
)

# add directory for lib

add_library(
	${PROJECT_NAME}
	INTERFACE
)

add_library(
	lib::sdl2
	ALIAS
	${PROJECT_NAME}
)

# avoid pkg/CMakeLists from running/generating a bunch of duplicated files for SHARED and STATIC

add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/pkg"
	EXCLUDE_FROM_ALL
)

# include headers

target_include_directories(
	${PROJECT_NAME}
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/pkg/include"
)

# link src

target_link_libraries(
	${PROJECT_NAME}
	INTERFACE
	SDL2::SDL2
	SDL2::SDL2main
)
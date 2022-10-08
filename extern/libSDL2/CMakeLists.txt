project(
	libSDL2	
)

# add directory for lib

add_library(
	${PROJECT_NAME}
	OBJECT
	libSDL2.cpp	
)

add_library(
	lib::sdl2
	ALIAS
	${PROJECT_NAME}
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	"${PROJECT_SOURCE_DIR}/pkg/include"
)

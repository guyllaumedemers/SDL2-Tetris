project(
	libSDL2	
)

# add directory for lib

add_library(
	${PROJECT_NAME}
	STATIC
	"${PROJECT_SOURCE_DIR}/libSDL2.cpp"	
)

add_library(
	lib::sdl2
	ALIAS
	${PROJECT_NAME}
)

#target_include_directories(
#	${PROJECT_NAME}
#	INTERFACE
#	"${PROJECT_SOURCE_DIR}/pkg/include"
#)

# temp solution for try_run in appTetris CMakeLists.txt

#add_library(
#	tryRunlibSDL2
#	STATIC
#	$<TARGET_OBJECTS:${PROJECT_NAME}>
#)

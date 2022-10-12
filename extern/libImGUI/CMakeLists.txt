project(
	libImGUI
)

# add directory for lib

add_library(
	${PROJECT_NAME}
	STATIC
	"${PROJECT_SOURCE_DIR}/libImGUI.cpp"
)

add_library(
	lib::imgui
	ALIAS
	${PROJECT_NAME}
)

#target_include_directories(
#	${PROJECT_NAME}
#	INTERFACE
#	"${PROJECT_SOURCE_DIR}/pkg/backends"
#)

# temp solution for try_run in appTetris CMakeLists.txt

#add_library(
#	tryRunlibImGUI
#	STATIC
#	$<TARGET_OBJECTS:${PROJECT_NAME}>
#)
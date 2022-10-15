project(
	libImGUI
)

# add directory for lib

add_library(
	${PROJECT_NAME}
	STATIC
	"${CMAKE_CURRENT_SOURCE_DIR}/libImGUI.cpp"
)

add_library(
	lib::imgui
	ALIAS
	${PROJECT_NAME}
)
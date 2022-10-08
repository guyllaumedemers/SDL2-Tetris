project(
	libImGUI
)

# add directory for lib

add_library(
	${PROJECT_NAME}
	""
)

add_library(
	lib::imgui
	ALIAS
	${PROJECT_NAME}
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	"${PROJECT_SOURCE_DIR}/include" #might change depending on git module architecture fetch
)


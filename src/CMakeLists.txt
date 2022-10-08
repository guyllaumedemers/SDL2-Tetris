project(
	SDL2-TetrisApp
)

# add subdirectories

add_subdirectory(
	"appTetris"
	"${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}"
)

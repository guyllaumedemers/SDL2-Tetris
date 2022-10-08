project(
	SDL2-App
)

# add subdirectories

add_subdirectory(
	"appTetris"
	"${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}"
)

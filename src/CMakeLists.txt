project(
	SDL2-TetrisApp
)

# set GLOBAL compiler requirements - not ideal

set(CMAKE_CXX_COMPILE_FEATURES cxx_std_20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# add subdirectories

add_subdirectory(
	"appTetris"
	"${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}"
)

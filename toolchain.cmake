# toolchain specifics for windows
	
# set system info

if(WIN32)
	set(CMAKE_SYSTEM_NAME Windows)
	if(NOT $<IF:$<CMAKE_SYSTEM_NAME:"">:TRUE>)
		message(STATUS "system: ${CMAKE_SYSTEM_NAME}")
	endif()
	
	set(CMAKE_SYSTEM_PROCESSOR ARM64)
	if(NOT $<IF:$<CMAKE_SYSTEM_PROCESSOR:"">:TRUE>)
		message(STATUS "processor: ${CMAKE_SYSTEM_PROCESSOR}")
	endif()
endif()

# set generator options, should done in the cmd toolchain

set(CMAKE_GENERATOR "Visual Studio 17 2022")
set(CMAKE_GENERATOR_PLATEFORM "x64")
set(CMAKE_GENERATOR_TOOLSET "host=x64")

# check generator using regex

string(REGEX MATCH "^Visual Studio" REGEX_MSVC ${CMAKE_GENERATOR})
string(REGEX MATCH "^Ninja" REGEX_NINJA ${CMAKE_GENERATOR})

# check generator selected

if(NOT $<STREQUAL:${REGEX_MSVC},"">)
	message("generator: ${CMAKE_GENERATOR}")
	set(MSVC_GENERATOR TRUE)
elseif(NOT $<STREQUAL:${REGEX_NINJA},"">)
	message("generator: ${CMAKE_GENERATOR}")
	set(NINJA_GENERATOR TRUE)
endif()
	
# set compiler options

if(MSVC_GENERATOR)
	message(STATUS "Setting MSVC compiler")
	find_path(MSVC_C_COMPILER_PATH "cl.exe" PATHS ENV var)
	if(NOT $<IF:$<MSVC_C_COMPILER_PATH:"">:NOTFOUND>)
		message(STATUS "msvc c compiler path: ${MSVC_C_COMPILER_PATH}>")
		set(MSVC_C_COMPILER_PATH "$<MSVC_C_COMPILER_PATH>/gcc")
	endif()
	
	find_path(MSVC_CXX_COMPILER_PATH "cl.exe" PATHS ENV var)
	if(NOT $<IF:$<MSVC_CXX_COMPILER_PATH:"">:NOTFOUND>)
		message(STATUS "msvc cxx compiler path: ${MSVC_CXX_COMPILER_PATH}>")
		set(MSVC_CXX_COMPILER_PATH "$<MSVC_CXX_COMPILER_PATH>/g++")
	endif()
	set(CMAKE_C_COMPILER_ID MSVC)
	set(CMAKE_CXX_COMPILER_ID MSVC)
elseif(NINJA_GENERATOR)
	message(STATUS "Setting Ninja compiler")
	find_path(C_COMPILER_PATH "gcc.exe" PATHS ENV var)
	if(NOT $<IF:$<C_COMPILER_PATH:"">:NOTFOUND>)
		message(STATUS "c compiler path: ${C_COMPILER_PATH}>")
		set(CMAKE_C_COMPILER "$<C_COMPILER_PATH>/gcc")
	endif()
	
	find_path(CXX_COMPILER_PATH "g++.exe" PATHS ENV var)
	if(NOT $<IF:$<CXX_COMPILER_PATH:"">:NOTFOUND>)
		message(STATUS "cxx compiler path: ${CXX_COMPILER_PATH}>")
		set(CMAKE_CXX_COMPILER "$<CXX_COMPILER_PATH>/g++")
	endif()
	set(CMAKE_C_COMPILER_ID GNU)
	set(CMAKE_CXX_COMPILER_ID GNU)
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
# toolchain specifics for windows
	
# set system info

if(WIN32)
	set(CMAKE_SYSTEM_NAME Windows)
	if(NOT $<IF:$<CMAKE_SYSTEM_NAME:"">:TRUE>)
		message(STATUS "system: ${CMAKE_SYSTEM_NAME}")
	endif()
	
	set(CMAKE_SYSTEM_PROCESSOR x64)
	if(NOT $<IF:$<CMAKE_SYSTEM_PROCESSOR:"">:TRUE>)
		message(STATUS "processor: ${CMAKE_SYSTEM_PROCESSOR}")
	endif()
endif()

# set generator options, should done in the cmd toolchain

#set(CMAKE_GENERATOR "Visual Studio 17 2022")
#set(CMAKE_GENERATOR_PLATEFORM "x64")
#set(CMAKE_GENERATOR_TOOLSET "host=x64")

message(STATUS "generator plateform: ${CMAKE_GENERATOR_PLATEFORM}")

# check generator using regex

string(REGEX MATCH "^Visual Studio" REGEX_MSVC ${CMAKE_GENERATOR})
string(REGEX MATCH "^Ninja" REGEX_NINJA ${CMAKE_GENERATOR})

# check generator selected

if(NOT ${REGEX_MSVC} STREQUAL "")
	message("select msvc generator: ${CMAKE_GENERATOR}, ${REGEX_MSVC}")
	set(MSVC_GENERATOR TRUE)
elseif(NOT ${REGEX_NINJA} STREQUAL "")
	message("select ninja generator: ${CMAKE_GENERATOR}, ${REGEX_NINJA}")
	set(NINJA_GENERATOR TRUE)
endif()

# set environment variable

set(MSVC_ENV_VAR "$ENV{Path}")

# run debug test for ENV Path entry targeting x86 when x64 is target OS specified

# copy path
set(SPLIT_MSVC_ENV_VAR ${MSVC_ENV_VAR})
# init pos 
set(MSVC_ENV_VAR_BEGIN 0)
# init delim
set(MSVC_ENV_VAR_DELIM ";")
# init replace
set(MSVC_ENV_VAR_REPLACE "")
# split until
while(NOT MSVC_ENV_VAR_BEGIN EQUAL -1)
	# reset pos
	set(MSVC_ENV_VAR_BEGIN 0)
	# find delim
	string(FIND "${SPLIT_MSVC_ENV_VAR}" "${MSVC_ENV_VAR_DELIM}" MSVC_ENV_VAR_END)
	# substring, 0 + length 
	string(SUBSTRING "${SPLIT_MSVC_ENV_VAR}" "${MSVC_ENV_VAR_BEGIN}" "${MSVC_ENV_VAR_END}" OUT_MSVC_SUBSTRING)
	# print ENV var PATH
	message(STATUS "ENV var PATH: ${OUT_MSVC_SUBSTRING}")
	# remove substring from string
	string(REPLACE "${OUT_MSVC_SUBSTRING}${MSVC_ENV_VAR_DELIM}" "${MSVC_ENV_VAR_REPLACE}" OUT_MSVC_REDUCE_STRING "${SPLIT_MSVC_ENV_VAR}")
	# update target string
	set(SPLIT_MSVC_ENV_VAR "${OUT_MSVC_REDUCE_STRING}")
	# update pos
	set(MSVC_ENV_VAR_BEGIN "${MSVC_ENV_VAR_END}")
endwhile()


set(MINGW64_ENV_VAR "$ENV{Path}")
	
# set compiler options,  ERROR: somehow retrieve x86 when Env PATH set x64

if(MSVC_GENERATOR)
	message(STATUS "Setting MSVC compiler")
	find_path(MSVC_C_COMPILER_PATH "cl.exe" PATHS ${MSVC_ENV_VAR})
	if(NOT $<IF:$<MSVC_C_COMPILER_PATH:"">:NOTFOUND>)
		message(STATUS "msvc c compiler dir: ${MSVC_C_COMPILER_PATH}>")
		set(CMAKE_C_COMPILER "${MSVC_C_COMPILER_PATH}/cl.exe")
		message(STATUS "c compiler executable path: ${CMAKE_C_COMPILER}")
	endif()

	message(STATUS "--------------------------------------------------------------------------------------------")
	
	find_path(MSVC_CXX_COMPILER_PATH "cl.exe" PATHS ${MSVC_ENV_VAR})
	if(NOT $<IF:$<MSVC_CXX_COMPILER_PATH:"">:NOTFOUND>)
		message(STATUS "msvc cxx compiler dir: ${MSVC_CXX_COMPILER_PATH}>")
		set(CMAKE_CXX_COMPILER "${MSVC_CXX_COMPILER_PATH}/cl.exe")
		message(STATUS "cxx compiler executable path: ${CMAKE_CXX_COMPILER}")
	endif()
	set(CMAKE_C_COMPILER_ID MSVC)
	set(CMAKE_CXX_COMPILER_ID MSVC)
elseif(NINJA_GENERATOR)
	message(STATUS "Setting Ninja compiler")
	find_path(C_COMPILER_PATH "gcc.exe" PATHS ${MINGW64_ENV_VAR})
	if(NOT $<IF:$<C_COMPILER_PATH:"">:NOTFOUND>)
		message(STATUS "c compiler dir: ${C_COMPILER_PATH}>")
		set(CMAKE_C_COMPILER "${C_COMPILER_PATH}/gcc.exe")
		message(STATUS "c compiler executable path: ${CMAKE_C_COMPILER}")
	endif()

	message(STATUS "--------------------------------------------------------------------------------------------")
	
	find_path(CXX_COMPILER_PATH "g++.exe" PATHS ${MINGW64_ENV_VAR})
	if(NOT $<IF:$<CXX_COMPILER_PATH:"">:NOTFOUND>)
		message(STATUS "cxx compiler dir: ${CXX_COMPILER_PATH}>")
		set(CMAKE_CXX_COMPILER "${CXX_COMPILER_PATH}/g++.exe")
		message(STATUS "cxx compiler executable path: ${CMAKE_CXX_COMPILER}")
	endif()
	set(CMAKE_C_COMPILER_ID GNU)
	set(CMAKE_CXX_COMPILER_ID GNU)
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
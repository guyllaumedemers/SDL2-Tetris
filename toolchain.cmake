# toolchain specifics for windows

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

string(REGEX MATCH "^Visual" REGEX_MSVC ${CMAKE_GENERATOR})
string(REGEX MATCH "^Ninja" REGEX_NINJA ${CMAKE_GENERATOR})

# check generator selected

if(NOT ${REGEX_MSVC} STREQUAL "")
	message("select msvc generator: ${CMAKE_GENERATOR}, ${REGEX_MSVC}")
	set(MSVC_GENERATOR TRUE)
elseif(NOT ${REGEX_NINJA} STREQUAL "")
	message("select ninja generator: ${CMAKE_GENERATOR}, ${REGEX_NINJA}")
	set(NINJA_GENERATOR TRUE)
endif()

# split string using regex. looking for ways to out variable and print outside

function(get_split_list_regex IN_VAR OUT_VAR)
	set(BLOCK_STATEMENT_MINIMUM_VERSION_REQUIRED "3.25")
	if(CMAKE_VERSION GREATER_EQUAL "${BLOCK_STATEMENT_MINIMUM_VERSION_REQUIRED}")
		block(SCOPE_FOR VARIABLES)
			message(STATUS "regex parsing...")
			string(REGEX MATCHALL "C\:[a-zA-Z\\\ 0-9\.\(\)]+[^\;]" REGEX_OUT_VAR "${IN_VAR}")
			message(STATUS "parse complete!")
			return(PROPAGATE "${REGEX_OUT_VAR}")
		endblock()
	else()
			message(STATUS "regex parsing...")
			string(REGEX MATCHALL "C\:[a-zA-Z\\\ 0-9\.\(\)]+[^\;]" REGEX_OUT_VAR "${IN_VAR}")
			message(STATUS "parse complete!")
			set(OUT_VAR "${REGEX_OUT_VAR}" PARENT_SCOPE)
	endif()
endfunction()

# set environment variable

if(MSVC_GENERATOR)
	set(MSVC_ENV_VAR "$ENV{Path}")
	# run ENV var PATH _debug output
	get_split_list_regex("${MSVC_ENV_VAR}" OUT_VAR)
	foreach(ITEM IN LISTS OUT_VAR)
		message(STATUS "${ITEM}")
	endforeach()
elseif(NINJA_GENERATOR)
	set(MINGW64_ENV_VAR "$ENV{Path}")
	# run ENV var PATH _debug output
	get_split_list_regex("${MINGW64_ENV_VAR}" OUT_VAR)
	foreach(ITEM IN LISTS OUT_VAR)
		message(STATUS "${ITEM}")
	endforeach()
endif()
	
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
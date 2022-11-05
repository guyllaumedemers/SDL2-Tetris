# toolchain specifics for windows, generator expression drawback: https://stackoverflow.com/questions/28692896/cmake-generator-expression-is-not-evaluated

if(WIN32)
	set(CMAKE_SYSTEM_NAME Windows)
	if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL NOTFOUND)
		message(STATUS "system: ${CMAKE_SYSTEM_NAME}")
	endif()
	
	set(CMAKE_SYSTEM_PROCESSOR x64)
	if(NOT "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL NOTFOUND)
		message(STATUS "processor: ${CMAKE_SYSTEM_PROCESSOR}")
	endif()
endif()

# set generator options, should done in the cmd toolchain

#set(CMAKE_GENERATOR "Visual Studio 17 2022")
#set(CMAKE_GENERATOR_PLATEFORM "x64")
#set(CMAKE_GENERATOR_TOOLSET "host=x64")

# check generator using regex

string(REGEX MATCH "^Visual" REGEX_MSVC ${CMAKE_GENERATOR})
string(REGEX MATCH "^Ninja" REGEX_NINJA ${CMAKE_GENERATOR})
string(REGEX MATCH "^MinGW" REGEX_MINGW ${CMAKE_GENERATOR})
string(REGEX MATCH "^NMake" REGEX_NMAKE ${CMAKE_GENERATOR})

# check generator selected

if(NOT ${REGEX_MSVC} STREQUAL NOTFOUND)
	message("select msvc generator: ${CMAKE_GENERATOR}, ${REGEX_MSVC}")
	set(MSVC_GENERATOR TRUE)
elseif(NOT ${REGEX_NINJA} STREQUAL NOTFOUND)
	message("select ninja generator: ${CMAKE_GENERATOR}, ${REGEX_NINJA}")
	set(NINJA_GENERATOR TRUE)
elseif(NOT ${REGEX_MINGW} STREQUAL NOTFOUND)
	message("select mingw generator: ${CMAKE_GENERATOR}, ${REGEX_MINGW}")
	set(MINGW_GENERATOR TRUE)
elseif(NOT ${REGEX_NMAKE} STREQUAL NOTFOUND)
	message("select nmake generator: ${CMAKE_GENERATOR}, ${REGEX_NMAKE}")
	set(NMAKE_GENERATOR TRUE)
endif()

# split ENV var list

function(get_env_path_list_regex IN_VAR OUT_UNIQUE_REGEX_SPLIT_ENV_LIST_VAR)
	# out variable feature - version specific
	set(BLOCK_STATEMENT_MINIMUM_VERSION_REQUIRED "3.25")
	# regex parsing
	message(STATUS "env path directories...")
	string(REGEX MATCHALL "C\:[a-zA-Z\\\ 0-9\.\(\)\+\-]+[^\;]" REGEX_OUT_VAR "${IN_VAR}")
	# print splits
	foreach(ITEM IN LISTS REGEX_OUT_VAR)
		message(STATUS "${ITEM}")
	endforeach()
	message(STATUS "complete!")
	# conditional out
	if("${CMAKE_VERSION}" GREATER_EQUAL "${BLOCK_STATEMENT_MINIMUM_VERSION_REQUIRED}")
		block(SCOPE_FOR VARIABLES)
			return(PROPAGATE "${REGEX_OUT_VAR}")
		endblock()
	else()
		set(OUT_UNIQUE_REGEX_SPLIT_ENV_LIST_VAR "${REGEX_OUT_VAR}" PARENT_SCOPE)
	endif()
endfunction()

# filter ENV var list by Plateform.

function(get_plateform_specific_list_regex IN_VAR IN_FILTER OUT_UNIQUE_REGEX_FETCH_PLATEFORM_LIST_VAR)
	# out variable feature - version specific
	set(BLOCK_STATEMENT_MINIMUM_VERSION_REQUIRED "3.25")
	# regex parsing
	message(STATUS "target directories by plateform: ${IN_FILTER}")
	string(REGEX MATCHALL "C\:[a-zA-Z\\\ 0-9\.\(\)\+\-]+(${IN_FILTER})[a-zA-Z\\\ 0-9\.\(\)\+\-]+[^\;]" REGEX_OUT_VAR "${IN_VAR}")
	# print splits
	foreach(ITEM IN LISTS REGEX_OUT_VAR)
		message(STATUS "${ITEM}")
	endforeach()
	message(STATUS "complete!")
	# conditional out
	if("${CMAKE_VERSION}" GREATER_EQUAL "${BLOCK_STATEMENT_MINIMUM_VERSION_REQUIRED}")
		block(SCOPE_FOR VARIABLES)
			return(PROPAGATE "${REGEX_OUT_VAR}")
		endblock()
	else()
		set(OUT_UNIQUE_REGEX_FETCH_PLATEFORM_LIST_VAR "${REGEX_OUT_VAR}" PARENT_SCOPE)
	endif()
endfunction()

# check plateform

set(PLATEFORM_x64 "x64|64")
set(PLATEFORM_x86 "x86|86")

if(WIN32)
	# there is no way of checking windows plateform here
	set(PLATEFORM_WINDOW "${PLATEFORM_x64}")
	message(STATUS "target plateform: ${PLATEFORM_WINDOW}")
endif()

# set environment variable

# cached env path
set(HOST_ENV_VAR "$ENV{Path}")
# run ENV var PATH _debug output
get_env_path_list_regex("${HOST_ENV_VAR}" OUT_UNIQUE_REGEX_SPLIT_ENV_LIST_VAR)
message(STATUS "--------------------------------------------------------------------------------------------")
# run Plateform ENV var PATH _debug output
get_plateform_specific_list_regex("${OUT_UNIQUE_REGEX_SPLIT_ENV_LIST_VAR}" "${PLATEFORM_WINDOW}" OUT_UNIQUE_REGEX_FETCH_PLATEFORM_LIST_VAR)
# update ENV var entries
set(HOST_ENV_VAR "${OUT_UNIQUE_REGEX_FETCH_PLATEFORM_LIST_VAR}")

message(STATUS "--------------------------------------------------------------------------------------------")
	
# set compiler options,  ERROR: somehow retrieve x86 when Env PATH set x64

if(MSVC_GENERATOR)
	# find compiler at path
	foreach(ITEM IN LISTS HOST_ENV_VAR)
		find_path(MSVC_C_COMPILER_PATH "cl.exe" PATHS "${ITEM}" NO_CACHE NO_DEFAULT_PATH)
	endforeach()
	if(NOT "${MSVC_C_COMPILER_PATH}" STREQUAL NOTFOUND)
		# print compiler path
		message(STATUS "msvc c compiler dir: ${MSVC_C_COMPILER_PATH}")
		# set compiler
		set(CMAKE_C_COMPILER "${MSVC_C_COMPILER_PATH}/cl.exe")
		message(STATUS "c compiler executable path: ${CMAKE_C_COMPILER}")
	endif()

	message(STATUS "--------------------------------------------------------------------------------------------")
	
	# find compiler at path
	foreach(ITEM IN LISTS HOST_ENV_VAR)
		find_path(MSVC_CXX_COMPILER_PATH "cl.exe" PATHS "${ITEM}" NO_CACHE NO_DEFAULT_PATH)
	endforeach()
	if(NOT "${MSVC_CXX_COMPILER_PATH}" STREQUAL NOTFOUND)
		# print compiler path
		message(STATUS "msvc cxx compiler dir: ${MSVC_CXX_COMPILER_PATH}")
		# set compiler
		set(CMAKE_CXX_COMPILER "${MSVC_CXX_COMPILER_PATH}/cl.exe")
		message(STATUS "cxx compiler executable path: ${CMAKE_CXX_COMPILER}")
	endif()
	# c compiler id
	set(CMAKE_C_COMPILER_ID MSVC)
	# cxx compiler id
	set(CMAKE_CXX_COMPILER_ID MSVC)
elseif(NINJA_GENERATOR OR MINGW_GENERATOR OR NMAKE_GENERATOR)
	# find compiler at path
	foreach(ITEM IN LISTS HOST_ENV_VAR)
		find_path(C_COMPILER_PATH "gcc.exe" PATHS "${HOST_ENV_VAR}" NO_CACHE NO_DEFAULT_PATH)
	endforeach()
	if(NOT "${C_COMPILER_PATH}" STREQUAL NOTFOUND)
		# print compiler path
		message(STATUS "c compiler dir: ${C_COMPILER_PATH}")
		# set compiler
		set(CMAKE_C_COMPILER "${C_COMPILER_PATH}/gcc.exe")
		message(STATUS "c compiler executable path: ${CMAKE_C_COMPILER}")
	endif()

	message(STATUS "--------------------------------------------------------------------------------------------")
	
	# find compiler at path
	foreach(ITEM IN LISTS HOST_ENV_VAR)
		find_path(CXX_COMPILER_PATH "g++.exe" PATHS "${HOST_ENV_VAR}" NO_CACHE NO_DEFAULT_PATH)
	endforeach()
	if(NOT "${CXX_COMPILER_PATH}" STREQUAL NOTFOUND)
		# print compiler path
		message(STATUS "cxx compiler dir: ${CXX_COMPILER_PATH}")
		# set compiler
		set(CMAKE_CXX_COMPILER "${CXX_COMPILER_PATH}/g++.exe")
		message(STATUS "cxx compiler executable path: ${CMAKE_CXX_COMPILER}")
	endif()
	# c compiler id
	set(CMAKE_C_COMPILER_ID GNU)
	# cxx compiler id
	set(CMAKE_CXX_COMPILER_ID GNU)
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
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

# split string using c-style algorithm *not really - unreliable - produce bad parsing. looking for ways to out variable and get list at least

function(get_split_list_safe InSTRING SPLIT_STRING)
	# init begin index
	set(CURRENT_INDEX 0)
	# cache string
	set(STRING_COPY "${InSTRING}")
	# delim
	set(InDELIM ";")
	# replace
	set(InREPLACE "")
	# start
	message(STATUS "parsing...")
	while(NOT CURRENT_INDEX EQUAL -1)
		# reset head position
		set(CURRENT_INDEX 0)
		# find delim pos
		string(FIND "${STRING_COPY}" "${InDELIM}" DELIM_INDEX)
		# extract substring
		string(SUBSTRING "${STRING_COPY}" "${CURRENT_INDEX}" "${DELIM_INDEX}" OUT_SUBSTRING)
		# remove value from entry
		string(REPLACE "${OUT_SUBSTRING}${InDELIM}" "${InREPLACE}" UPDATE_STRING "${STRING_COPY}")
		# update string copy
		set(STRING_COPY "${UPDATE_STRING}")
		# append list
		list(APPEND SPLIT_STRING "${OUT_SUBSTRING}")
		# update pos
		set(CURRENT_INDEX "${DELIM_INDEX}")
		#message(STATUS "${OUT_SUBSTRING}")
	endwhile()
	message(STATUS "complete!")
endfunction()

# set environment variable

if(MSVC_GENERATOR)
	set(MSVC_ENV_VAR "$ENV{Path}")
	# run ENV var PATH _debug output

	## unreliable - produce bad parsing
	#foreach(ITEM IN LISTS MSVC_ENV_VAR)
	#	message(STATUS "${ITEM}")
	#endforeach()
	message(STATUS "MSVC ENV var PATH")
	get_split_list_safe("${MSVC_ENV_VAR}" OUT_MSVC_SPLIT_STRING CACHE STRING)
	foreach(ITEM IN LISTS OUT_MSVC_SPLIT_STRING)
		message(STATUS "${ITEM}")
	endforeach()
elseif(NINJA_GENERATOR)
	set(MINGW64_ENV_VAR "$ENV{Path}")
	# run ENV var PATH _debug output

	## unreliable - produce bad parsing
	#foreach(ITEM IN LISTS MINGW64_ENV_VAR)
	#	message(STATUS "${ITEM}")
	#endforeach()
	message(STATUS "Ninja ENV var PATH")
	get_split_list_safe("${MINGW64_ENV_VAR}" OUT_MINGW64_SPLIT_STRING)
	foreach(ITEM IN LISTS OUT_MINGW64_SPLIT_STRING)
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
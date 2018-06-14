# This file is part of TUNNEX.

# TUNNEX is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# laster any later version.

# TUNNEX is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with TUNNEX.  If not, see <http://www.gnu.org/licenses/>.

# TUNNEX 1.0.0 
# Copyright (c) Henrik Quanz

###
### CMake settings
###
## Due to Mac OSX we need to keep compatibility with CMake 2.6
# see http://www.cmake.org/Wiki/CMake_Policies
cmake_minimum_required(VERSION 3.3)


include(CheckCXXCompilerFlag)


###
### Project settings
###
project(alglib_CPP)

set(ALGLIB_CPP_VERSION_MAJOR "3")
set(ALGLIB_CPP_VERSION_MINOR "3")
set(ALGLIB_CPP_VERSION_PATCH "0")
set(ALGLIB_CPP_VERSION "${ALGLIB_CPP_VERSION_MAJOR}.${ALGLIB_CPP_VERSION_MINOR}.${ALGLIB_CPP_VERSION_PATCH}")

enable_testing()

include(CheckCCompilerFlag)

###
### Project options
###
## Project stuff


## Build options
# --> General
# see http://www.cmake.org/cmake/help/cmake2.6docs.html#variable:BUILD_SHARED_LIBS
#     http://www.cmake.org/cmake/help/cmake2.6docs.html#command:add_library
option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)

# --> Apple
option(APPLE_UNIVERSAL_BIN "Apple: Build universal binary" OFF)

# --> Microsoft Visual C++
# see http://msdn.microsoft.com/en-us/library/aa278396(v=VS.60).aspx
#     http://msdn.microsoft.com/en-us/library/2kzt1wy3(v=VS.71).aspx
option(MSVC_SHARED_RT "MSVC: Build with shared runtime libs (/MD)" ON)
option(MSVC_STHREADED_RT "MSVC: Build with single-threaded static runtime libs (/ML until VS .NET 2003)" OFF)

###
### Sources, headers, directories and libs
###

set(header_directory ".")

file(GLOB sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE public_headers "[a-zA-Z]*.h")


set(library_sources
  ${sources}
  ${public_headers}
)

add_sources(${library_sources})

if(VERBOSE)
	message(STATUS "sources: ${sources}")
	message(STATUS "public_headers: ${public_headers}")
endif()

include_directories(${ALGLIB_CPP_SOURCE_DIR})


###
### General compilation settings
###
if(UNIX AND NOT APPLE)
	set(BUILD_SHARED_LIBS OFF)
endif(UNIX AND NOT APPLE)

set(alglib_c_flags ${CMAKE_C_FLAGS})
set(alglib_cxx_flags ${CMAKE_CXX_FLAGS})

if(BUILD_SHARED_LIBS)
	set(LABEL_SUFFIX "shared")
else()
	set(LABEL_SUFFIX "static")
endif()

if(APPLE)
	if(APPLE_UNIVERSAL_BIN)
		set(CMAKE_OSX_ARCHITECTURES ppc;i386)
	endif()
endif()

#if(IPHONE)
#	set(CMAKE_OSX_SYSROOT "iphoneos4.2")
#	set(CMAKE_OSX_ARCHITECTURES "armv6;armv7")
#endif()

if(WIN32)
	if(BUILD_SHARED_LIBS)
		add_definitions(-D${PROJECT_NAME}_DLL)	# use or build Windows DLL
	endif()
	if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
		set(CMAKE_INSTALL_PREFIX "C:/")
	endif()
endif()

# GCC or Clang specialities
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR
   "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	### General stuff
	if(WIN32)
		set(CMAKE_SHARED_LIBRARY_PREFIX "")	# DLLs do not have a "lib" prefix
		set(CMAKE_IMPORT_LIBRARY_PREFIX "")	# same for DLL import libs
		set(CMAKE_LINK_DEF_FILE_FLAG "")	# CMake workaround (2.8.3)
	endif()

	### Project stuff
	if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE Release)
	endif()
	#
	set(CMAKE_CXX_FLAGS_RELEASE "-O2")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
	#
	set(GCC_EXTRA_OPTIONS "")
	#
	set(FLAG_TESTED "-Wextra")
	check_cxx_compiler_flag(${FLAG_TESTED} FLAG_WEXTRA)
	if(FLAG_WEXTRA)
		set(GCC_EXTRA_OPTIONS "${GCC_EXTRA_OPTIONS} ${FLAG_TESTED}")
	endif()

	# Disable warnings about unused variables
	set(FLAG_TESTED "-Wno-unused-variable")
	check_cxx_compiler_flag(${FLAG_TESTED} FLAG_UNUSED_VARIABLE)
	if(FLAG_UNUSED_VARIABLE)
		set(GCC_EXTRA_OPTIONS "${GCC_EXTRA_OPTIONS} ${FLAG_TESTED}")
	endif()
	#

	# Disable warnings about unused parameters
	set(FLAG_TESTED "-Wno-unused-parameter")
	check_cxx_compiler_flag(${FLAG_TESTED} FLAG_UNUSED_PARAMETER)
	if(FLAG_UNUSED_PARAMETER)
		set(GCC_EXTRA_OPTIONS "${GCC_EXTRA_OPTIONS} ${FLAG_TESTED}")
	endif()

	set(alglib_cxx_flags "-Wall ${GCC_EXTRA_OPTIONS} -pedantic -Wno-long-long ${alglib_cxx_flags}")
	#

	### Make specific
	if(${CMAKE_BUILD_TOOL} MATCHES make OR ${CMAKE_BUILD_TOOL} MATCHES gmake)
		add_custom_target(debuggable $(MAKE) clean
			COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
			COMMENT "Adjusting settings for debug compilation"
			VERBATIM)
		add_custom_target(releasable $(MAKE) clean
			COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
			COMMENT "Adjusting settings for release compilation"
			VERBATIM)
	endif()
endif()

# Microsoft VisualC++ specialities
if(MSVC)
	### General stuff
	# a) Change MSVC runtime library settings (/MD[d], /MT[d], /ML[d] (single-threaded until VS 2003))
	#    plus set lib suffix for later use and project label accordingly
	# see http://msdn.microsoft.com/en-us/library/aa278396(v=VS.60).aspx
	#     http://msdn.microsoft.com/en-us/library/2kzt1wy3(v=VS.71).aspx
	set(LIB_RT_SUFFIX "md")	# CMake defaults to /MD for MSVC
	set(LIB_RT_OPTION "/MD")
	#
	if(NOT MSVC_SHARED_RT)	# User wants to have static runtime libraries (/MT, /ML)
		if(MSVC_STHREADED_RT)	# User wants to have old single-threaded static runtime libraries
			set(LIB_RT_SUFFIX "ml")
			set(LIB_RT_OPTION "/ML")
			if(NOT ${MSVC_VERSION} LESS 1400)
				message(FATAL_ERROR "Single-threaded static runtime libraries (/ML) only available until VS .NET 2003 (7.1).")
			endif()
		else()
			set(LIB_RT_SUFFIX "mt")
			set(LIB_RT_OPTION "/MT")
		endif()

		# correct linker options
		foreach(flag_var  alglib_c_flags  alglib_cxx_flags)
			foreach(config_name  ""  DEBUG  RELEASE  MINSIZEREL  RELWITHDEBINFO)
				set(var_name "${flag_var}")
				if(NOT "${config_name}" STREQUAL "")
					set(var_name "${var_name}_${config_name}")
				endif()
				string(REPLACE "/MD" "${LIB_RT_OPTION}" ${var_name} "${${var_name}}")
			endforeach()
		endforeach()
	endif()
	#
	set(LABEL_SUFFIX "${LABEL_SUFFIX} ${LIB_RT_SUFFIX}")

	# b) Change prefix for static libraries
	set(CMAKE_STATIC_LIBRARY_PREFIX "lib")	# to distinguish static libraries from DLL import libs

	# c) Correct suffixes for static libraries
	if(NOT BUILD_SHARED_LIBS)
		### General stuff
		set(LIB_TARGET_SUFFIX "${LIB_SUFFIX}${LIB_RT_SUFFIX}")
	endif()

	### Project stuff
	# /W3 = set warning level; see http://msdn.microsoft.com/en-us/library/thxezb7y.aspx
	# /wd4127 = disable warning C4127 "conditional expression is constant"; see http://msdn.microsoft.com/en-us/library/6t66728h.aspx
	# /wd4355 = disable warning C4355 "'this' : used in base member initializer list"; http://msdn.microsoft.com/en-us/library/3c594ae3.aspx
	set(alglib_cxx_flags "/W3 /wd4127 /wd4355 ${alglib_cxx_flags}")
endif()


###
### General install settings
###
if(WIN32)
	set(_library_dir bin) # .dll are in PATH, like executables
else()
	set(_library_dir lib)
endif()





set(LIB_INSTALL_DIR "${_library_dir}${LIB_SUFFIX}")

set(_INSTALL_DESTINATIONS
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
	ARCHIVE DESTINATION "lib${LIB_SUFFIX}"
)


###
### Library
###
add_definitions(-DCOMPILE_ALG_LIB)
add_library(alglib ${library_sources})
set_target_properties(alglib PROPERTIES
  COMPILE_FLAGS "${alglib_c_flags} ${alglib_cxx_flags}"
)


set_target_properties(alglib PROPERTIES
	VERSION "${alglib_CPP_VERSION}"
	SOVERSION "${ALGLIB_CPP_VERSION_MAJOR}.${ALGLIB_CPP_VERSION_MINOR}"
	PROJECT_LABEL "alglib ${LABEL_SUFFIX}"
)


if(MSVC)
	if(NOT BUILD_SHARED_LIBS)
		# correct library names
		set_target_properties(alglib PROPERTIES
			DEBUG_POSTFIX "${LIB_TARGET_SUFFIX}d"
			RELEASE_POSTFIX "${LIB_TARGET_SUFFIX}"
			MINSIZEREL_POSTFIX "${LIB_TARGET_SUFFIX}"
			RELWITHDEBINFO_POSTFIX "${LIB_TARGET_SUFFIX}"
		)
	endif()
endif()



export(
    TARGETS alglib
    FILE "${PROJECT_BINARY_DIR}/alglib-targets.cmake")
export(PACKAGE alglib)
set(EXPORT_TARGETS alglib CACHE INTERNAL "export targets")



if(UNIX)
	set(PC_FILE ${CMAKE_BINARY_DIR}/alglib.pc)
	#install(FILES ${PC_FILE} DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
endif()

IF(WIN32)
    INSTALL(TARGETS alglib
        RUNTIME DESTINATION ".")
ELSE()
    INSTALL(TARGETS alglib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
ENDIF(WIN32)

set_property(TARGET alglib PROPERTY POSITION_INDEPENDENT_CODE ON)



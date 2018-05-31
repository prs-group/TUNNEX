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
project(math_CPP)

set(math_CPP_VERSION_MAJOR "1")
set(math_CPP_VERSION_MINOR "0")
set(math_CPP_VERSION_PATCH "0")
set(math_CPP_VERSION "${math_CPP_VERSION_MAJOR}.${math_CPP_VERSION_MINOR}.${math_CPP_VERSION_PATCH}")

enable_testing()


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

include_directories(${math_CPP_SOURCE_DIR})


###
### General compilation settings
###
set(math_c_flags ${CMAKE_C_FLAGS})
set(math_cxx_flags ${CMAKE_CXX_FLAGS})

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

if(IPHONE)
	set(CMAKE_OSX_SYSROOT "iphoneos4.2")
	set(CMAKE_OSX_ARCHITECTURES "armv6;armv7")
endif()

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
	#
	set(math_cxx_flags "-Wall ${GCC_EXTRA_OPTIONS} -pedantic -Wno-long-long ${math_cxx_flags}")


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
		foreach(flag_var  math_c_flags  math_cxx_flags)
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
	set(math_cxx_flags "/W3 /wd4127 /wd4355 ${math_cxx_flags}")
endif()


###
### General install settings
###
if(WIN32)
	set(_library_dir bin)	# .dll are in PATH, like executables
else()
	set(_library_dir lib)
endif()

set(INCLUDE_INSTALL_ROOT_DIR include)

set(INCLUDE_INSTALL_DIR ${INCLUDE_INSTALL_ROOT_DIR}/math)
set(LIB_INSTALL_DIR "${_library_dir}${LIB_SUFFIX}")

set(_INSTALL_DESTINATIONS
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
	ARCHIVE DESTINATION "lib${LIB_SUFFIX}"
)


###
### Library
###
add_definitions(-DCOMPILE_MATH_LIB)

add_library(math ${library_sources})
set_target_properties(math PROPERTIES
  COMPILE_FLAGS "${math_c_flags} ${math_cxx_flags}"
)

set_property(TARGET math PROPERTY CXX_STANDARD 11)



target_link_libraries(math "${Qt5Core_LIBRARIES}")

set_target_properties(math PROPERTIES
	VERSION "${math_CPP_VERSION}"
	SOVERSION "${math_CPP_VERSION_MAJOR}.${math_CPP_VERSION_MINOR}"
	PROJECT_LABEL "math ${LABEL_SUFFIX}"
)

if(IPHONE)
	set_target_properties(math PROPERTIES
		XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "3.0"
	)
endif()

if(MSVC)
	if(NOT BUILD_SHARED_LIBS)
		# correct library names
		set_target_properties(math PROPERTIES
			DEBUG_POSTFIX "${LIB_TARGET_SUFFIX}d"
			RELEASE_POSTFIX "${LIB_TARGET_SUFFIX}"
			MINSIZEREL_POSTFIX "${LIB_TARGET_SUFFIX}"
			RELWITHDEBINFO_POSTFIX "${LIB_TARGET_SUFFIX}"
		)
	endif()
endif()

#install(TARGETS math ${_INSTALL_DESTINATIONS})
#install(
#	DIRECTORY ${header_directory}
#	DESTINATION ${INCLUDE_INSTALL_DIR}
#	FILES_MATCHING PATTERN "*.h"
#)

export(
    TARGETS math
    FILE "${PROJECT_BINARY_DIR}/math-targets.cmake")
export(PACKAGE math)
set(EXPORT_TARGETS math CACHE INTERNAL "export targets")



if(UNIX)
	set(PC_FILE ${CMAKE_BINARY_DIR}/math.pc)
	#install(FILES ${PC_FILE} DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
endif()

IF(WIN32)
    INSTALL(TARGETS math
        RUNTIME DESTINATION ".")
ELSE()
    INSTALL(TARGETS math
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
ENDIF(WIN32)

set_property(TARGET math PROPERTY POSITION_INDEPENDENT_CODE ON)

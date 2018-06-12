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

#------------------------------------------------------------------------------
#
#				Options
#
#------------------------------------------------------------------------------
option(DEPLOY "If set the app will be deployed using qt tools" OFF)
#------------------------------------------------------------------------------
include(CheckCXXCompilerFlag)

cmake_minimum_required(VERSION 3.9)
project(tunnex)

IF( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Release)
ENDIF()


# A bug when linking against the qtmaind library on windows prevents
#the usage of debug build type

IF(deploy)
    set(should_deploy YES)
ELSE(deploy)
    SET(should_deploy NO)
ENDIF(deploy)

IF(WITH_TESTING)
    enable_testing()
ENDIF(WITH_TESTING)

IF(APPLE)
    if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9 CACHE STRING "Minimum Version of MacOS supported" FORCE)
    endif(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
ENDIF(APPLE)

IF(DEPLOY)
	# On apple systems we need to know where the macdeployqt is located
	if(APPLE)
        find_program(DEPLOYQT macdeployqt)
        IF(NOT DEPLOYQT)
            message(FATAL "Could not find macdeployqt for deploying")
        ENDIF(NOT DEPLOYQT)
        message(STATUS "Found macdeployqt: ${DEPLOYQT}")
	ENDIF(APPLE)

	# On Windows Systems we need to know wehre windeployqt is located
	if(WIN32)
			find_program(DEPLOYQT windeployqt)
        IF(NOT DEPLOYQT)
            message(FATAL "Could not find windeployqt for deploying")
        ENDIF(NOT DEPLOYQT)
        message(STATUS "Found windeployqt: ${DEPLOYQT}")
	ENDIF(WIN32)

ENDIF(DEPLOY)




# app executable name
set(APP_NAME "tunnex")
        
# Initilize CXX FLAGS for debug and release
IF("${CMAKE_BUILD_TYPE}" EQUAL "Release" OR "{CMAKE_BUILD_TYPE}" EQUAL "MinSizeRel")
    add_definitions("-DNDEBUG")
ENDIF("${CMAKE_BUILD_TYPE}" EQUAL "Release" OR "{CMAKE_BUILD_TYPE}" EQUAL "MinSizeRel")

IF(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG          "/Od /DEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "/O1")
    set(CMAKE_CXX_FLAGS_RELEASE        "/O2")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /DEBUG")
ELSE()
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O4")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
ENDIF(MSVC)

# Nice grabber function for source files and headers

# From http://www.cmake.org/pipermail/cmake/2010-March/035992.html:
# function to collect all the sources from sub-directories
# into a single list
function(add_sources)
  get_property(is_defined GLOBAL PROPERTY SRCS_LIST DEFINED)
  if(NOT is_defined)
    define_property(GLOBAL PROPERTY SRCS_LIST
      BRIEF_DOCS "List of source files"
      FULL_DOCS "List of all source files in the entire project")
  endif()
  # make absolute paths
  set(SRCS)
  foreach(s IN LISTS ARGN)
    if(NOT IS_ABSOLUTE "${s}")
      get_filename_component(s "${s}" ABSOLUTE)
    endif()
    list(APPEND SRCS "${s}")
  endforeach()
  # append to global list
  set_property(GLOBAL APPEND PROPERTY SRCS_LIST "${SRCS}")
endfunction(add_sources)


# Qt 5 
set(QT_MIN_VERSION "5.6.0")
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
include_directories("${Qt5Core_INCLUDE_DIRS}" "${Qt5Widgets_INCLUDE_DIRS}" "${Qt5PrintSupport_INCLUDE_DIRS}")

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Tunnex")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Tunnex")
endif(WIN32)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("${PROJECT_SOURCE_DIR}/ALGLIB")
add_subdirectory("${PROJECT_SOURCE_DIR}/Math")
add_subdirectory("${PROJECT_SOURCE_DIR}/QCustomPlot")
add_subdirectory("${PROJECT_SOURCE_DIR}/gui")
add_subdirectory("${PROJECT_SOURCE_DIR}/Tunnex")

set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/license.txt)

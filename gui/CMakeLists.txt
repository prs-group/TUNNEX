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


project(gui)

#--------------------------------------------------------------------------------
# we need this to be able to include headers produced by uic in our code
#--------------------------------------------------------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#-------------------------------------------------------------------------------
# The source files for the gui are structed according to MVC Principle
# Sadly, sometimes the controller also creates GUIs which is hard
# to seperate
#-------------------------------------------------------------------------------

add_subdirectory(model)
add_subdirectory(controller)
add_subdirectory(view)

#-------------------------------------------------------------------------------
# Set the search path to the current directory
# This ui file can be either included by 
# <AUTOUIC_SEARCH_PATH>/<path><ui_basename>.h or
# <AUTOUIC_SEARCH_PATH>/<ui_basename>.h or
# where path is in this directory
#-------------------------------------------------------------------------------

if(CMAKE_AUTOUIC_SEARCH_PATHS)
	list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})
else()
	set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})
endif(CMAKE_AUTOUIC_SEARCH_PATHS)


#-------------------------------------------------------------------------------
# The definitions for using the libraries
#-------------------------------------------------------------------------------

add_definitions(-DUSE_MATH_LIB)
add_definitions(-DQCUSTOMPLOT_USE_LIBRARY)
add_definitions(-DUSE_ALG_LIB)

#--------------------------------------------------------------------------------
# This is the list of source files that need to be compiled
#--------------------------------------------------------------------------------
file(GLOB misc_SRC "[a-zA-Z]*.cpp")

#--------------------------------------------------------------------------------
# another list, this time it includes all header files that should be treated with moc
#--------------------------------------------------------------------------------
file(GLOB misc_HEADER "[a-zA-Z]*.h")

#--------------------------------------------------------------------------------
# and finally any resource files. I do not have any in this app
#--------------------------------------------------------------------------------
SET( tunnex_RCS

)

#--------------------------------------------------------------------------------
# Configure file
#--------------------------------------------------------------------------------
# Get github tag
INCLUDE(GetGitRevisionDescription)
git_describe(GIT_TAG --tags --match v*.*.*)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/gui_config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/gui_config.h @ONLY)

if(VERBOSE)
	message(STATUS "GUI Sources: ${misc_SRC} ${model_SRC} ${controller_SRC} \
	${view_SRC}")
	message(STATUS "GUI Headers: ${misc_HEADER} ${model_HEADER} ${controller_HEADER}
		${view_HEADER}")
	message(STATUS "GUI UIC search path: ${CMAKE_AUTOUIC_SEARCH_PATHS}")
endif()

#--------------------------------------------------------------------------------
# Build the gui as a static library
# This enables the test programs to build without rebuilding the entire app
#--------------------------------------------------------------------------------

ADD_LIBRARY( gui OBJECT
  ${misc_SRC}
  ${misc_HEADER}
  ${model_SRC}
  ${model_HEADER}
  ${controller_SRC}
  ${controller_HEADER}
  ${view_SRC}
  ${view_HEADER}
  ${gui_GEN_MOC_HDRS} 
  ${gui_GEN_UIS}
  ${gui_GEN_RCS}
  ${tunnex_RCS}
)

set_target_properties(gui PROPERTIES AUTOUIC ON)
set_property(TARGET gui PROPERTY POSITION_INDEPENDENT_CODE ON)

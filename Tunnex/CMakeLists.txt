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

#--------------------------------------------------------------------------------
# This is an example project that uses CMake as a build system to compile a Qt4.x
# based project. 
# To have CMake pick a Qt installation of your choice that won't be found
# automatically, set the CMAKE_PREFIX_PATH environment variable.
# For example: "export CMAKE_PREFIX_PATH=/usr/local/trolltech/qt4.3.5"
#  

PROJECT (tunnex)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (tunnex_VERSION_MAJOR 1)
set (tunnex_VERSION_MINOR 0)
set (tunnex_VERSION_PATCH 0)

INCLUDE_DIRECTORIES (
  # Look in the cmake build directory (some generated headers could be there)
  ${CMAKE_CURRENT_BINARY_DIR} 
  # Subdirectories
  ${CMAKE_SOURCE_DIR}/gui
)

#--------------------------------------------------------------------------------
# For Apple set the icns file containing icons
IF(APPLE)
  # set how it shows up in the Info.plist file
  SET(ICON tunnex.icns) 
  # set where in the bundle to put the icns file
  SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/Resources/tunnex.icns PROPERTIES  MACOSX_PACKAGE_LOCATION Resources)
  SET(tunnex_SRCS ${tunnex_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/Resources/tunnex.icns)
ENDIF(APPLE)

IF(WIN32 OR MSVC)
  set(tunnex_SRCS ${tunnex_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/Resources/tunnex.rc)
  INCLUDE_DIRECTORIES(Before ${CMAKE_CURRENT_SOURCE_DIR}/Resources)
endif(WIN32 OR MSVC)

#--------------------------------------------------------------------------------
#  CMake's way of creating an executable
ADD_EXECUTABLE( ${APP_NAME} MACOSX_BUNDLE WIN32
	main.cpp $<TARGET_OBJECTS:gui> ${tunnex_SRCS}
)


#--------------------------------------------------------------------------------
# Tell CMake which libraries we need to link our executable against.

# A bug occurs during the compilation using MSVC 2013 when linking against qtmaind 
# This bug can be fixed by linking against the qtmain library
# Therefore the behaviour to link against the qtmain library on windows is disabled

if(MSVC)
    set_property(TARGET ${APP_NAME} PROPERTY Qt5_NO_LINK_QTMAIN ON)
    find_library(qtmain_lib qtmain)
    if(NOT qtmain_lib)
        message(FATAL "Could not find the qtmain library")
    endif(NOT qtmain_lib)
    target_link_libraries( ${APP_NAME} ${qtmain_lib})
endif(MSVC)

target_link_libraries( ${APP_NAME} 
    Qt5::Core
    Qt5::Widgets
    Qt5::PrintSupport
    math alglib qcustomplot)

#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------
SET(plugin_dest_dir bin)
if(MSVC)
    SET(qtconf_dest_dir ".")
else()
    SET(qtconf_dest_dir bin)
ENDIF(MSVC)

SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${APP_NAME}")
IF(APPLE)
  SET(plugin_dest_dir ${APP_NAME}.app/Contents/Plugins)
  SET(qtconf_dest_dir ${APP_NAME}.app/Contents/Resources)
  SET(library_dest_dir ${APP_NAME}.app/Contens/libs)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/${APP_NAME}.app")
ENDIF(APPLE)
IF(WIN32)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/${APP_NAME}.exe")
ENDIF(WIN32)

#--------------------------------------------------------------------------------
# Install the application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.

IF(WIN32)
    INSTALL(TARGETS ${APP_NAME}
        RUNTIME DESTINATION ".")
# Copy Qt Libraries to executable path 
    message(STATUS "Configuration ${CONFIGURATION}")
    foreach(qtlib "Qt5::Core" "Qt5::Widgets" "Qt5::Gui" "Qt5::PrintSupport")
        get_target_property(locationd ${qtlib} LOCATION_DEBUG)
        get_target_property(location ${qtlib} LOCATION_RELEASE)
        install(FILES "${locationd}" DESTINATION "." CONFIGURATIONS DEBUG)
        install(FILES "${location}" DESTINATION "." CONFIGURATIONS RELEASE)
	endforeach(qtlib)
    
# Program also depends on windows plugin
    foreach(plugin ${Qt5Widgets_PLUGINS})
         get_target_property(_loc ${plugin} LOCATION)
         message("Plugin ${plugin} is at location ${_loc}")
    endforeach(plugin)
    find_path(plugin_dir NAMES "plugins")
    if(plugin_dir)
        install(FILES "${plugin_dir}/plugins/platforms/qwindowsd.dll"
        DESTINATION "plugins/platforms" CONFIGURATIONS DEBUG)
        install(FILES "${plugin_dir}/plugins/platforms/qwindows.dll"
        DESTINATION "plugins/platforms" CONFIGURATIONS RELEASE)
    endif(plugin_dir)
    
ELSE()
    INSTALL(TARGETS ${APP_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        BUNDLE DESTINATION . COMPONENT Runtime)
ENDIF(WIN32)

#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE'
#IF(NOT QT_PLUGINS_DIR)
#        find_path(plugin_dir NAMES "plugins")
#        if(plugin_dir)
#            set(QT_PLUGINS_DIR "${plugin_dir}/plugins")
#        endif(plugin_dir)

#ENDIF(NOT QT_PLUGINS_DIR)

#IF(QT_PLUGINS_DIR)
#INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${plugin_dest_dir}/plugins #COMPONENT Runtime)
#ENDIF(QT_PLUGINS_DIR)

#IF(NOT QT_PLUGINS_DIR)
#    MESSAGE( WARNING "Skipping installation of Qt Plugins. You can change that by #specifiying the QT_PLUGINS_DIR variable")
#ENDIF(NOT QT_PLUGINS_DIR)

#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
INSTALL(CODE "
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
    " COMPONENT Runtime)


#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.

# directories to look for dependencies
SET(DIRS ${QT_LIBRARY_DIRS})

# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well

IF(DEPLOY)
	include(CPack)
	if(APPLE)
		set(CPACK_BINARY_DRAGNDROP ON)
		set(BINARY_PATH $<TARGET_BUNDLE_DIR:${APP_NAME}>)
		add_custom_target(deploy ALL 
		"${DEPLOYQT}" "${BINARY_PATH}"
		DEPENDS ${APP_NAME}
		USES_TERMINAL)
	elseif(WIN32)
		set(BINARY_PATH $<TARGET_FILE:${APP_NAME}>)
		add_custom_target(deploy ALL 
		"${DEPLOYQT}" "${BINARY_PATH}" "-printsupport"
		DEPENDS ${APP_NAME})

	endif(APPLE)
	#add_custom_command(OUTPUT deployed
			#COMMAND "${DEPLOYQT}" "${APP_NAME}"
			#VERBATIM
			#COMMENT "Deploying ${APP_NAME} using ${DEPLOYQT}"
			#DEPENDS ${APP_NAME})

ENDIF(DEPLOY)

if(APPLE)
  set(MACOSX_BUNDLE_INFO_FILE_IN Resources/Info.plist.in)
  SET(MACOSX_BUNDLE_INFO_FILE Resources/Info.plist)
  SET(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${MACOSX_BUNDLE_INFO_FILE})
  CONFIGURE_FILE(${MACOSX_BUNDLE_INFO_FILE_IN} ${OUTPUT_PATH} @ONLY)
  set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${OUTPUT_PATH})
ENDIF(APPLE)

IF(WITH_TESTING)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example_data)
ENDIF(WITH_TESTING)

set_property(TARGET ${APP_NAME} PROPERTY CXX_STANDARD 11)

# To Debug the application a custom user file needs to be generated 
# To include the PATH variable for Qt Libraries

# First get the path of the Library Location. Configuration does not matter
# because the debug and the release library should be in the same directory
get_target_property(QtCoreLocation Qt5::Core IMPORTED_LOCATION_DEBUG)
get_filename_component(QT_LIB_LOCATION ${QtCoreLocation} DIRECTORY CACHE)
if(MSVC_IDE)
	# Minimum version that is supported (not tested)
	set(TOOLS_VERSION "12.0")
	# Configure the template file
	SET(USER_FILE ${PROJECT_NAME}.vcxproj.user)
	SET(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
	CONFIGURE_FILE(UserTemplate.vcxproj.user ${USER_FILE} @ONLY)
elseif(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MD")
endif(MSVC_IDE)


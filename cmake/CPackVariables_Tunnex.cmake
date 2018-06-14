# vim:tabstop=4:shiftwidth=4
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
INCLUDE(InstallRequiredSystemLibraries)
INCLUDE(GetGitRevisionDescription)

git_describe(GIT_TAG --tags --match v*.*.*)


STRING(REGEX MATCHALL "^v([0-9]+)\\.([0-9+])\\.([0-9]+)" 
	GIT_VERSION_STRING ${GIT_TAG})

if(CMAKE_MATCH_0)
	set(CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1})
	set(CPACK_PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2})
	set(CPACK_PACKAGE_VERSION_PATCH ${CMAKE_MATCH_3})
else()
	message(FATAL_ERROR "Deployment: Could not get version from git describe")
ENDIF(CMAKE_MATCH_0)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tunneling in experiments (TUNNEX) is a free open-source program with an easy-to use graphical user interface (GUI) to simplify the process of WKB computations.")
SET(CPACK_PACKAGE_VENDOR "Henrik Quanz")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "TUNNEX")
SET(CPACK_PACKAGE_CONTACT "henrik.quanz@org.chemie.uni-giessen.de")
IF(WIN32 AND NOT UNIX)
	set(CPACK_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
	# There is a bug in NSI that does not handle full unix paths properly. Make
	# sure there is at least one set of four (4) backlasshes.
	string(REPLACE "/" "\\\\"  CMAKE_SOURCE_DIR_WIN "${CMAKE_SOURCE_DIR}")
	SET(CPACK_NSIS_PACKAGE_NAME "TUNNEX")
	SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR_WIN}\\\\Tunnex\\\\Resources\\\\installer_icon.bmp")
	SET(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR_WIN}\\\\Tunnex\\\\Resources\\\\tunnex.ico")
	#SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
	SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} TUNNEX")
	SET(CPACK_NSIS_HELP_LINK "https://github.com/prs-group/TUNNEX")
	SET(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/prs-group/TUNNEX")
	SET(CPACK_NSIS_CONTACT "henrik.quanz@org.chemie.uni-giessen.de")
	SET(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
	SET(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
	SET(CPACK_NSIS_MODIFY_PATH OFF)
	SET(CPACK_NSIS_PACKAGE_NAME "TUNNEX")
	SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "tunnex.exe")
ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES "tunnex.exe" "${CMAKE_SOURCE_DIR}/Tunnex/Resources/tunnex.ico" )
IF(APPLE)
	SET(CPACK_DMG_VOLUME_NAME "Install TUNNEX")
ENDIF(APPLE)

if(UNIX AND NOT APPLE)
	# Debian packet manger
	SET(CPACK_DEBIAN_PACKAGE_NAME "tunnex")
	SET(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
	# Version of the package not the program
	SET(CPACK_DEBIAN_PACKAGE_RELEASE 1)
	# EPOCH see https://www.debian.org/doc/debian-policy/
	STRING(TIMESTAMP EPOCH "%Y%m%d")
	SET(CPACK_DEBIAN_PACKAGE_EPOCH ${EPOCH})
	SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
	SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "TUNNEX is a graphical user inter to simplify the process of WKB computations.")
	SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/prs-group/TUNNEX")
	SET(CPACK_DEBIAN_PACKAGE_SOURCE "https://github.com/prs-group/TUNNEX/archive/v1.0.0.tar.gz")
	SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Henrik Quanz")
	# Redhead package manager
	SET(CPACK_RPM_PACKAGE_SUMMARY "${CPACK_DEBIAN_PACKAGE_DESCRIPTION}")
	SET(CPACK_RPM_PACKAGE_RELEASE "${CPACK_DEBIAN_PACKAGE_RELEASE}")
	SET(CPACK_RPM_PACKAGE_LICENSE "GPLv3")
	SET(CPACK_RPM_PACKAGE_GROUP "Science")
	SET(CPACK_RPM_PACKAGE_VENDOR "${CPACK_DEBIAN_PACKAGE_MAINTAINER}")
	SET(CPACK_RPM_PACKAGE_URL "${CPACK_DEBIAN_PACKAGE_HOMEPAGE}")
	SET(CPACK_RPM_PACKAGE_DESCRIPTION "${CPACK_DEBIAN_PACKAGE_DESCRIPTION}")
	SET(CPACK_RPM_PACKAGE_AUTOREQ yes)
	SET(CPACK_RPM_PACKAGE_AUTOPROV yes)
ENDIF(UNIX AND NOT APPLE)
INCLUDE(CPack)


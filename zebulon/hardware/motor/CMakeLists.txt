# We must specify a minimum version, current min is 2.8
IF(WIN32)
	# Require 2.8.2 for Debug/Release configurations in VS
	cmake_minimum_required (VERSION 2.8.2) 
ELSE()
	cmake_minimum_required (VERSION 2.8)
ENDIF(WIN32)

# Use path relative to cmake build dir
if(COMMAND cmake_policy)
	if(POLICY CMP0015)
		cmake_policy(SET CMP0015 NEW)
	endif(POLICY CMP0015)
endif(COMMAND cmake_policy)

# Set output folders for binaries and libraries.
include(../../SetOutputDirectories.cmake)
include(../../FindACTIVE.cmake)

# Name the project
project (motors)

# Adds the CxUtils CMakeLists.txt file if available.
ACTIVE_ADD_SUBDIRECTORY(CxUtils ./libcxutils)
#ACTIVE_ADD_SUBDIRECTORY(TinyXML ./libtinyxml)
#ACTIVE_ADD_SUBDIRECTORY(JAUS ./libjaus++)

# Add external dependencies we have projects for.

add_subdirectory(minissc/build/cmake ./cmake.minissc)
add_subdirectory(roboteq/build/cmake ./cmake.roboteq)
add_subdirectory(sabertooth/build/cmake ./cmake.sabertooth)


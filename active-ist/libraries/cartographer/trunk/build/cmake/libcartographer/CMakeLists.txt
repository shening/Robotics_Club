# We must specify a minimum version, current min is 2.8
IF(WIN32)
	# Require 2.8.2 for Debug/Release configurations in VS
	cmake_minimum_required (VERSION 2.8.2) 
ELSE()
	cmake_minimum_required (VERSION 2.8)
ENDIF(WIN32)

# This policy makes sure that all relative 
# paths are relative to this CMakeLists.txt file.
if(COMMAND cmake_policy)
	if(POLICY CMP0015)
		cmake_policy(SET CMP0015 NEW)
	endif(POLICY CMP0015)
endif(COMMAND cmake_policy)

# Script to setup output directories.
include(../../../../../SetOutputDirectories.cmake)
include(../../../../../FindOpenCV.cmake)

if(OpenCV_FOUND)
	#MESSAGE(STATUS "OpenCV Found")
	OPTION(CARTOGRAPHER_USE_OPENCV "Add OPENCV visualization" TRUE)
	if(CARTOGRAPHER_USE_OPENCV)
		add_definitions(-DUSE_OPENCV)
	endif(CARTOGRAPHER_USE_OPENCV)
endif(OpenCV_FOUND)

# Set the name to use for the DLL Export Preprocessor
# definition for WIN32
set(DLL_EXPORT_NAME CARTOGRAPHER_DLL)
# Set name of this project, and dependencies needed.
# Names are different between MSVC and other build systems, so
# in MSVC add prefix to name(s) as needed.
set(LIB_PROJECT_NAME cartographer)
# Set build dependencies.  These projects will get built
# before this file.
set(DEPENDENCIES 
		${CxUtils_DEPENDENCY}
		${TinyXML_DEPENDENCY})
# Source/Directories to sort.
set(SOURCE_GROUPS maps objects planning)
# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(LIB_NAME ${LIB_PREFIX}${LIB_PROJECT_NAME})

if(OpenCV_FOUND)
	MESSAGE(STATUS "OpenCV Found")
	OPTION(CARTOGRAPHER_USE_OPENCV "Add OPENCV visualization" ${CARTOGRAPHER_USE_OPENCV})
	if(CARTOGRAPHER_USE_OPENCV)
		add_definitions(-DUSE_OPENCV)
	endif(CARTOGRAPHER_USE_OPENCV)
endif(OpenCV_FOUND)

# Add include directories for header files.
include_directories(../../../include/)
include_directories(${TinyXML_INCLUDE_DIRS})
include_directories(${CxUtils_INCLUDE_DIRS})
include_directories(${CxUtils_INCLUDE_DIRS}/../ext/zlib-1.2.3)

# Set variable name of input directories for code.
set(LIB_INCLUDE_DIR ../../../include/${LIB_PROJECT_NAME})
set(LIB_SRC_DIR ../../../src/${LIB_PROJECT_NAME})

# Setup any additional compiler flags.
if(MSVC)
	# Setup to use UNICODE for MSVC, and disable warnings for
	# use of s_* standard C functions.
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE)
	# Add support for parallel compilation
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif(MSVC)

if(UNIX AND NOT MINGW)
	# Ensure using gcc for our code
	set(CMAKE_C_COMPILER gcc)
	# Add fPIC to support 64 bit building issues
	add_definitions(-fPIC)
endif()

if(OpenCV_FOUND)
	if(CARTOGRAPHER_USE_OPENCV)
		add_definitions(-DUSE_OPENCV)
	endif(CARTOGRAPHER_USE_OPENCV)
endif(OpenCV_FOUND)

# Add paths to directories containing external dependencies we need.
link_directories(../../../lib)

# Get header files.
file(GLOB_RECURSE LIB_HEADERS ${LIB_INCLUDE_DIR}/*.h)
# Get src files.
file(GLOB_RECURSE LIB_SOURCES ${LIB_SRC_DIR}/*.cpp)

# Build a library
add_library(${LIB_NAME} SHARED ${LIB_SOURCES} ${LIB_HEADERS})

if(WIN32)
	# Add a define for building a DLL in windows.
	set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "-D${DLL_EXPORT_NAME}=__declspec(dllexport)")
else()
	# Link symbols
	set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS -rdynamic)
endif(WIN32)

# Add an _d when building debug version
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")

if(DEPENDENCIES)
	add_dependencies(${LIB_NAME} ${DEPENDENCIES})
	target_link_libraries(${LIB_NAME} ${DEPENDENCIES})
endif(DEPENDENCIES)

# Set any external libraries to link against.
set(EXT_LIBS
		${CxUtils_DEPENDENCY}
		${TinyXML_DEPENDENCY})
		
if(OpenCV_FOUND AND CARTOGRAPHER_USE_OPENCV)
	set(EXT_LIBS ${EXT_LIBS} ${OpenCV_LIBRARIES})
endif(OpenCV_FOUND AND CARTOGRAPHER_USE_OPENCV)
		
# Link against dependencies.
target_link_libraries(${LIB_NAME} 
						${CMAKE_REQUIRED_LIBRARIES}
						${EXT_LIBS})


# CPack Installer Generation
if(WIN32)

	install(TARGETS ${LIB_NAME}
			DESTINATION bin
			ARCHIVE DESTINATION lib
			COMPONENT SDK)
	install(TARGETS ${LIB_NAME}
			DESTINATION bin
			RUNTIME DESTINATION bin
			COMPONENT SDK)
	install(TARGETS ${LIB_NAME}
			DESTINATION bin
			LIBRARY DESTINATION bin
			COMPONENT SDK)
	install(DIRECTORY 
			${LIB_INCLUDE_DIR}
			DESTINATION include/ 
			COMPONENT SDK
			FILES_MATCHING PATTERN "*.h" 
			PATTERN ".svn" EXCLUDE)
	install(DIRECTORY 
			../../../docs/
			DESTINATION docs/../docs/Cartographer
			COMPONENT SDK
			PATTERN ".svn" EXCLUDE
			)
	install(DIRECTORY 
			../../../include
			../../../src
			../../../ext
			../../../docs
			DESTINATION src/active-ist/libraries/cartographer/trunk/
			COMPONENT SourceCode
			PATTERN ".svn" EXCLUDE
			)
	install(DIRECTORY 
			../../../build/cmake/lib${LIB_PROJECT_NAME}
			DESTINATION src/active-ist/libraries/cartographer/trunk/build/cmake
			COMPONENT SourceCode
			PATTERN ".svn" EXCLUDE)
elseif(UNIX)
	# Where to install headers
	install(DIRECTORY 
			${LIB_INCLUDE_DIR}
			DESTINATION include/active/
			COMPONENT SDK
			FILES_MATCHING PATTERN "*.h" 
			PATTERN ".svn" EXCLUDE)
	# Where to install targets
	install(TARGETS 
			${LIB_NAME} 
			LIBRARY DESTINATION 
			lib/active
			COMPONENT SDK)
endif(WIN32)

# Make Source Groups, this makes sub folders in Visual Studio

foreach(GRP ${SOURCE_GROUPS})
	file(GLOB_RECURSE HEADER_GROUP ${LIB_INCLUDE_DIR}/${GRP}/*.h)
	file(GLOB_RECURSE SRC_GROUP ${LIB_SRC_DIR}/${GRP}/*.cpp)
	SOURCE_GROUP("Header Files\\${GRP}" FILES ${HEADER_GROUP})
	SOURCE_GROUP("Source Files\\${GRP}" FILES ${SRC_GROUP})
endforeach(GRP)

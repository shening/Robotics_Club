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

# Set output folders for binaries and libraries.
include(../../../../../../SetOutputDirectories.cmake)

#ZEBULON_ADD_SUBDIRECTORY(cvcapture ./sensors/video)

# Set build dependencies.  These projects will get built
# before this file.
set(DEPENDENCIES 
            ${LIB_PREFIX}baseline
#            ${LIB_PREFIX}cvcapture
#            ${LIB_PREFIX}compassfactory
#            ${LIB_PREFIX}laserfactory
#            ${LIB_PREFIX}gpsfactory
#            ${LIB_PREFIX}xmlconfig
#            ${LIB_PREFIX}mcu
#            ${LIB_PREFIX}mixer
#            ${LIB_PREFIX}filter
#            ${LIB_PREFIX}grapher
#			${LIB_PREFIX}dynamixel
            )

IF(CxUtils_SET_DEPENDENCY)
    set(DEPENDENCIES
            ${DEPENDENCIES}
            ${CxUtils_DEPENDENCY})
ENDIF()

IF(TinyXML_SET_DEPENDENCY)
    set(DEPENDENCIES
            ${DEPENDENCIES}
            ${TinyXML_DEPENDENCY})
ENDIF()

IF(JAUS_SET_DEPENDENCY)
    set(DEPENDENCIES
            ${DEPENDENCIES}
            ${JAUS_DEPENDENCY})
ENDIF(JAUS_SET_DEPENDENCY)

IF(Zebulon_SET_DEPENDENCY)
    set(DEPENDENCIES
            ${DEPENDENCIES}
            ${Zebulon_DEPENDENCIES})
ENDIF(Zebulon_SET_DEPENDENCY)

# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(EXT_LIBS 
        ${DEPENDENCIES}
        ${JAUS_LIBRARIES}
        ${Zebulon_LIBRARIES})

# Set variable name of input directories for code.
set(INCLUDE_DIR ../../../include/asvc/)
set(SRC_DIR ../../../src/asvc/)

# Add include paths required for this project
include_directories(../../../include/
                    ../../../include/asvc
                    ${Zebulon_INCLUDE_DIRS})

# Add library paths required for this project.
link_directories(../../../lib/ 
                    ${Zebulon_LIBRARY_DIRS})

foreach(V ${Zebulon_LIBRARY_DIRS})
#MESSAGE(STATUS ${V})
endforeach()

# Applications

# Get header files.
file(GLOB_RECURSE PROG_HEADERS ${INCLUDE_DIR}/baseline/*.h)
# Get src files.
file(GLOB_RECURSE PROG_SOURCES ${SRC_DIR}/baseline/*.cpp)

add_executable(baseline_program ${PROG_SOURCES} ${PROG_HEADERS})

target_link_libraries(baseline_program ${CMAKE_REQUIRED_LIBRARIES})
# Additional dependecies.
target_link_libraries(baseline_program 
            ${EXT_LIBS})

# Setup dependencies and build order.
add_dependencies(baseline_program ${DEPENDENCIES})
# Add Debug postfix
set_target_properties(baseline_program PROPERTIES DEBUG_POSTFIX "_d")

# Setup any additional compiler flags.
#if(WIN32)
#endif(WIN32)
#if(MSVC)
    # Setup to use UNICODE for MSVC, and disable warnings for
    # use of s_* standard C functions.
#    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE)
    # Add support for parallel compilation
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
#endif(MSVC)
#if(UNIX AND NOT MINGW)
    # Ensure using gcc for our code
#    set(CMAKE_C_COMPILER gcc)
    # Add fPIC to support 64 bit building issues
#    add_definitions(-fPIC)
#endif(UNIX AND NOT MINGW)

# render3d
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----
project(
	render3d
	VERSION 0.1.0
	LANGUAGES CXX)

# ---- Fetch CPM ----
set(CPM_DOWNLOAD_VERSION 0.40.2) 
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
  message(STATUS "Downloading CPM.cmake v${CPM_DOWNLOAD_VERSION}")
  file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()
include(${CPM_DOWNLOAD_LOCATION})

# ---- Add dependencies via CPM ----
# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")

# ---- Find Ogre-Next SDK ----
set(OGRE_NEXT_SDK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/source/ogre-next")

# Include and Library directories
set(OGRE_INCLUDE_DIRS ${OGRE_NEXT_SDK_ROOT}/include)
set(OGRE_LIBRARY_DIRS ${OGRE_NEXT_SDK_ROOT}/lib/Release/opt)

# Debugging paths
message(STATUS "Ogre-Next Include Dir: ${OGRE_INCLUDE_DIRS}")
message(STATUS "Ogre-Next Library Dir: ${OGRE_LIBRARY_DIRS}")

# Required libraries
find_library(OGRE_MAIN_LIB OgreNextMain PATHS ${OGRE_LIBRARY_DIRS} NO_DEFAULT_PATH)
find_library(OGRE_GL_RENDER_LIB RenderSystem_GL3Plus PATHS ${OGRE_LIBRARY_DIRS} NO_DEFAULT_PATH)

# Validate library locations
if(OGRE_MAIN_LIB STREQUAL "OGRE_MAIN_LIB-NOTFOUND")
    message(FATAL_ERROR "OgreNextMain library not found in ${OGRE_LIBRARY_DIRS}")
endif()

if(OGRE_GL_RENDER_LIB STREQUAL "OGRE_GL_RENDER_LIB-NOTFOUND")
    message(FATAL_ERROR "RenderSystem_GL3Plus library not found in ${OGRE_LIBRARY_DIRS}")
endif()

message(STATUS "Found OgreNextMain Library: ${OGRE_MAIN_LIB}")
message(STATUS "Found RenderSystem_GL3Plus Library: ${OGRE_GL_RENDER_LIB}")

# ---- Add source files ----
file(GLOB_RECURSE sources CONFIGURE_DEPENDS 
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cxx"
)

# ---- Create library ----
add_library(${PROJECT_NAME} ${sources})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/MTd>
        $<$<CONFIG:Release>:/MT>
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

# ---- Target Includes ----
target_include_directories(
  ${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
    ${OGRE_INCLUDE_DIRS}
)

# ---- Link Libraries ----
target_link_directories(${PROJECT_NAME} PRIVATE ${OGRE_LIBRARY_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${OGRE_MAIN_LIB}    # Core Ogre-Next library
    ${OGRE_GL_RENDER_LIB}  # OpenGL RenderSystem_GL3Plus library
)

# ---- Source Group for IDE Organization ----
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" FILES ${sources})

# ---- Package Project ----
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/source
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
)

# ---- Copy Binaries (DLLs) for Runtime ----
file(GLOB OGRE_DLLS "${OGRE_NEXT_SDK_ROOT}/Release/bin/*.dll")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${OGRE_NEXT_SDK_ROOT}/bin/Release $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying Ogre-Next DLLs to output directory."
)

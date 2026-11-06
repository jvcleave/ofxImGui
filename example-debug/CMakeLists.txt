# openFrameworks app (CMake)
#
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build
#
# The app is built into bin/. The openFrameworks core is linked from
# libs/openFrameworksCompiled/lib, or built here first if it isn't there.

cmake_minimum_required(VERSION 3.22)

# Use a different OF path
# set(OF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../of_0.12.1_git_cmake")

# Bring osx 10.15 support (not oficially supported by OF)
if(APPLE AND CMAKE_OSX_DEPLOYMENT_TARGET VERSION_EQUAL "10.15")
    message(STATUS "Targeting macOS 10.15 - Applying fixes !")
	add_compile_definitions(JSON_HAS_CPP_17 JSON_HAS_CPP_14 JSON_HAS_CPP_11) # fix json compilation with incomplete c++20 (c++2a)
	set(MAC_OS_MIN_VERSION 10.15)
	set(MAC_OS_CPP_VER -std=c++2a)
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)
	set(CMAKE_OBJCXX_STANDARD 20)
	set(MAC_OS_SDK 10.15)
	set(CMAKE_CXX_STANDARD 20)
endif()

# Enable debug functions
add_compile_definitions(OFXIMGUI_DEBUG)


# path to openFrameworks, relative to this project (apps/myApps/<app> by default)
if(NOT OF_ROOT)
	set(OF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
endif()
include("${OF_ROOT}/libs/openFrameworksCompiled/project/CMake/ccache.cmake")

# project folder name is the app name by default
get_filename_component(OF_APP_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${OF_APP_NAME} LANGUAGES C CXX)

include("${OF_ROOT}/libs/openFrameworksCompiled/project/CMake/openFrameworks.cmake")

# app version and build number (macOS / iOS / tvOS Info.plist), like APP_VERSION /
# APP_BUILD in a config.make; the version defaults to project(... VERSION), then 1.0
# set(APP_VERSION 1.0)
# set(APP_BUILD 1)

# sources come from src/, addons from addons.make; or list them:
#   of_add_app(${OF_APP_NAME} ADDONS ofxGui ofxOsc)
of_add_app(${OF_APP_NAME})

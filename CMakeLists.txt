# ofxImGui (CMake)
#
# Standalone target for projects that pull the addon in with add_subdirectory():
#
#   add_subdirectory(${OF_ROOT}/addons/ofxImGui ofxImGui)
#   target_link_libraries(myApp PRIVATE ofxImGui::ofxImGui)
#
# Inside an openFrameworks CMake project the usual way still works and does not
# use this file: of_add_app(myApp ADDONS ofxImGui) or addons.make read
# addon_config.mk, which carries the same platform excludes.
#
# Options:
#   OFXIMGUI_DEBUG   (OFF)  compile the ofxImGui debug window helpers
#
# The platform excludes below mirror addon_config.mk and the guards in the
# vendored ImGui backends. They are needed because a CMake glob picks up every
# backend, but only some of them exist or compile on a given platform:
#   all                  EngineVk.cpp (Vulkan engine is disabled upstream)
#   ios / tvos           GLFW backend (no GLFW) + OpenGL 2 backend (desktop-only GL)

cmake_minimum_required(VERSION 3.22)

if(NOT DEFINED OFXIMGUI_ROOT)
	set(OFXIMGUI_ROOT "${CMAKE_CURRENT_LIST_DIR}")
endif()

project(ofxImGui LANGUAGES C CXX)

# ---- openFrameworks -----------------------------------------------------
if(NOT TARGET openFrameworks::openFrameworks)
	if(NOT OF_ROOT)
		get_filename_component(OF_ROOT "${OFXIMGUI_ROOT}/../.." ABSOLUTE)
	endif()
	if(NOT EXISTS "${OF_ROOT}/libs/openFrameworksCompiled/project/CMake/openFrameworks.cmake")
		message(FATAL_ERROR "ofxImGui: openFrameworks not found. Set OF_ROOT, or include openFrameworks.cmake before adding this addon.")
	endif()
	include("${OF_ROOT}/libs/openFrameworksCompiled/project/CMake/openFrameworks.cmake")
	of_use_core() # prebuilt core if there is one, else builds it here
endif()

option(OFXIMGUI_DEBUG "ofxImGui: enable the debug window helpers" OFF)

# ---- platform ------------------------------------------------------------
if(NOT OF_PLATFORM)
	if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
		set(OF_PLATFORM ios)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "tvOS")
		set(OF_PLATFORM tvos)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
		set(OF_PLATFORM macos)
	elseif(EMSCRIPTEN)
		set(OF_PLATFORM emscripten)
	elseif(ANDROID)
		set(OF_PLATFORM android)
	endif()
endif()

# ---- sources -------------------------------------------------------------
set(_exts cpp cc cxx c)
if(APPLE)
	list(APPEND _exts mm m)
endif()
set(_patterns)
foreach(_ext IN LISTS _exts)
	list(APPEND _patterns "${OFXIMGUI_ROOT}/src/*.${_ext}" "${OFXIMGUI_ROOT}/libs/*.${_ext}")
endforeach()
file(GLOB_RECURSE OFXIMGUI_SOURCES CONFIGURE_DEPENDS ${_patterns})
list(FILTER OFXIMGUI_SOURCES EXCLUDE REGEX "/\\.[^/]*")

# ---- platform excludes ---------------------------------------------------
set(OFXIMGUI_EXCLUDES
	"/src/EngineVk\\.cpp$")

if(OF_PLATFORM STREQUAL "ios" OR OF_PLATFORM STREQUAL "tvos")
	list(APPEND OFXIMGUI_EXCLUDES
		"/libs/imgui/backends/imgui_impl_glfw\\.cpp$"
		"/libs/imgui/backends/imgui_impl_glfw_context_support\\.cpp$"
		"/libs/imgui/backends/imgui_impl_opengl2\\.cpp$"
		"/src/EngineGLFW\\.cpp$")
endif()

foreach(_pattern IN LISTS OFXIMGUI_EXCLUDES)
	list(FILTER OFXIMGUI_SOURCES EXCLUDE REGEX "${_pattern}")
endforeach()

# ---- target --------------------------------------------------------------
add_library(ofxImGui STATIC ${OFXIMGUI_SOURCES})
add_library(ofxImGui::ofxImGui ALIAS ofxImGui)

target_include_directories(ofxImGui PUBLIC
	"${OFXIMGUI_ROOT}/src"
	"${OFXIMGUI_ROOT}/libs/imgui"
	"${OFXIMGUI_ROOT}/libs/imgui/src"
	"${OFXIMGUI_ROOT}/libs/imgui/backends"
	"${OFXIMGUI_ROOT}/libs/imgui/extras"
	"${OFXIMGUI_ROOT}/libs/imgui/misc/freetype")

# same as ADDON_CFLAGS in addon_config.mk: lets other addons detect ofxImGui
target_compile_definitions(ofxImGui PUBLIC ofxAddons_ENABLE_IMGUI)
if(OFXIMGUI_DEBUG)
	target_compile_definitions(ofxImGui PUBLIC OFXIMGUI_DEBUG)
endif()

# iOS builds the GLES 2 backend by default, set OFXIMGUI_IOS_GLES3=ON for an ES3 app (OF_GLES_VERSION=3)
option(OFXIMGUI_IOS_GLES3 "Build the GLES 3 backend on iOS/tvOS" OFF)
if((OF_PLATFORM STREQUAL "ios" OR OF_PLATFORM STREQUAL "tvos") AND OFXIMGUI_IOS_GLES3)
	target_compile_definitions(ofxImGui PUBLIC OF_GLES_VERSION=3)
endif()

target_link_libraries(ofxImGui PUBLIC openFrameworks::openFrameworks)

list(LENGTH OFXIMGUI_SOURCES _n)
message(STATUS "ofxImGui: ${_n} sources (platform: ${OF_PLATFORM})")

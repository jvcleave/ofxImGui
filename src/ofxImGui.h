#pragma once

// Todo: try to detect the best settings from the OF compiler flags

#include "ofxImGuiConstants.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui.h"
#include "Gui.h"
#include "ImHelpers.h" // Note: not included anymore by default, include it manually if you need it !
#include "ImCanvas.h"
#include "ImViewport.h"

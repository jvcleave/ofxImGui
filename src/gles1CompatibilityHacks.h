
#pragma once

#include "ofxImGuiConstants.h"
#if defined (OFXIMGUI_RENDERER_GLES)// && defined (OFXIMGUI_RENDERER_GLES_1)

// Todo: maybe not all RPIs need this... Anyways, it's only used when GL ES 1 is required, compilation needs to be fixed anyways.
#pragma message "[Notice] Using some GL ES 1.1 compatibility hack for the RPI ! (prefer using at least GL ES 2)"

// Instead of loading GL, we load GL ES. This is normally not supported by the native DearImGui backend, but it seems to work.
//#ifndef GL_VERSION_1_0
#pragma message "Trying to include <GLES/gl.h> on rpi, instead of <GL/gl.h>. If you get errors, please comment the inclusion below."
//#undef __gl_h_ // this forces re-loading GL/gl.h
//#include <GL/gl.h> // --> Original inclusion, on RPIs in /usr/include/GL/gl.h
#ifdef TARGET_TARGET_LINUX_ARM
#include <GLES/gl.h>
#include <GLES/glext.h>
#elif defined(TARGET_OF_IOS)
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
//#import <OpenGLES/ES2/gl.h>
//#import <OpenGLES/ES2/glext.h>
#else
#warning "Couldn't locate GLES for your platform !"
#endif

// Rpis might want to apt install libmesa-dev or mesa-common-dev... they look already installed here.

#ifndef GL_VERSION_1_1
#pragma message "<GL/gl.h> was included, but not loaded correctly."
#endif

#ifdef GL_VERSION_ES_CM_1_0
#pragma message "<GLES/gl.h> was loaded somewhere, thus using GL ES 1, which needs some the tweaks below to become GL 1 compilable."
#endif

#if !defined(GL_VERSION_1_1) && defined(GL_VERSION_ES_CM_1_0)
#pragma message "Hacking GL ES to support GL 1.0, specially for RPIs. ofxImGui might behave weirdly as some GL functions are simply ignored."
// These defines are needed for fixed pipeline (GL ES 1) to compile with the RPI.
// Some have fallbacks, others simply disable the problematic calls.
#define glOrtho glOrthof // Different function name
#define glPolygonMode( X, Y ) // No alternative, but seems not to affect rendering
#define glPushAttrib( X ) // No alternative, but seems not to affect rendering
#define glPopAttrib( X )
#define GL_POLYGON_MODE GL_LINES
#define GL_FILL 0 // Enabled by default
#endif // !GL_VERSION_1_0 && GL_VERSION_ES_CM_1_0

// Some defines needed ?
#ifndef GL_TEXTURE_RECTANGLE
#define GL_TEXTURE_RECTANGLE 0 // TODO: find correct value
#endif

#ifndef GL_VERTEX_ARRAY_BINDING
#define GL_VERTEX_ARRAY_BINDING 777
#endif

#define GL_ENABLE_BIT 0
#define GL_TRANSFORM_BIT 0

// iOS GLES 2.0 hasn't got GL_UNPACK_ROW_LENGTH
#ifndef GL_UNPACK_ROW_LENGTH
OFXIMGUI_COMPILER_MESSAGE("GLES 2.0 doesn't have GL_UNPACK_ROW_LENGTH, providing a dummy placeholder !")
#define GL_UNPACK_ROW_LENGTH 0
#endif

#endif // rpi & gles1

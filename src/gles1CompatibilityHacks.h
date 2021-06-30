
#pragma once

#if defined(TARGET_RASPBERRY_PI) && defined(TARGET_OPENGLES)

// Todo: maybe not all RPIs need this... Anyways, it's only used when GL ES 1 is required, compilation needs to be fixed anyways.
#pragma message "[Notice] Using some GL ES 1.1 compatibility hack for the RPI ! (prefer using at least GL ES 2)"

// Instead of loading GL, we load GL ES. This is normally not supported by the native DearImGui backend, but it seems to work.
//#ifndef GL_VERSION_1_0
#pragma message "Trying to include <GLES/gl.h> on rpi, instead of <GL/gl.h>. If you get errors, please comment the inclusion below."
//#undef __gl_h_ // this forces re-loading GL/gl.h
//#include <GL/gl.h> // --> Original inclusion, on RPIs in /usr/include/GL/gl.h
#include <GLES/gl.h>
//#endif

// Rpis might want to apt install libmesa-dev or mesa-common-dev... they look already installed here.

//#ifndef GL_VERSION_1_1
//#pragma message "<GL/gl.h> was included, but not loaded correctly."
//#endif

//#ifdef GL_VERSION_ES_CM_1_0
//#pragma message "<GLES/gl.h> was loaded somewhere, thus using GL ES 1, which needs some the tweaks below to become GL 1 compilable."
//#endif

#if !defined(GL_VERSION_1_1) && defined(GL_VERSION_ES_CM_1_0)
#pragma message "Hacking GL ES to support GL 1.0, specially for RPIs. ofxImGui might behave weirdly."
// These defines are needed for fixed pipeline (GL ES 1) to compile with the RPI.
// Some have fallbacks, others simply disable the problematic calls.
#define glOrtho glOrthof // Different function name
#define glPolygonMode // No alternative, but seems not to affect rendering
#define glPushAttrib( X ) // No alternative, but seems not to affect rendering
#define glPopAttrib( X )
#define GL_POLYGON_MODE GL_LINES
#define GL_FILL 0 // Enabled by default
#endif // !GL_VERSION_1_0 && GL_VERSION_ES_CM_1_0

#endif // rpi & gles

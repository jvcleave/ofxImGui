#pragma once

#include "imgui.h"
#include "ofColor.h"
#include "ofRectangle.h"
#include <glm/vec2.hpp>
#include <vector>

// Forward declaration — avoids pulling in all of ofPath.h from this header.
class ofPath;

// ---------------------------------------------------------------------------
// ofxImGui::DrawList  — OF-style stateful wrapper around ImDrawList*
// ofxImGui::Canvas    — ImGui child-region with local coordinate system
//
// Quick usage:
//   ofxImGui::Canvas canvas;
//   if (canvas.begin("##myCanvas", {600, 400})) {
//       auto& dl = canvas.getDrawList();
//       dl.setColor(ofColor::yellow);
//       dl.setFill(false);
//       dl.setLineWidth(2.f);
//       dl.drawBezier(50,200, 150,50, 350,350, 550,200);
//       canvas.end();
//   }
//
// DrawList local coordinates: (0,0) = top-left of the canvas region.
// All primitives respect the current fill/color/lineWidth state, mirroring
// the openFrameworks stateful drawing API.
//
// ofPath rendering:
//   dl.drawPath(myPath);                    // path already in local coords
//   dl.drawPath(myPath, translate, zoom);   // path in doc/world space:
//                                           //   local = translate + doc * zoom
//   fill=true  → PathFillConvex (convex shapes only; use two calls for
//                filled+stroked paths)
//   fill=false → PathStroke (correct for any shape)
// ---------------------------------------------------------------------------

namespace ofxImGui {

// ---------------------------------------------------------------------------
// DrawList
// ---------------------------------------------------------------------------
class DrawList {
public:
    DrawList() = default;
    DrawList(ImDrawList* dl, ImVec2 origin);

    // --- State (mirrors ofSetColor / ofFill / ofNoFill / ofSetLineWidth) ---
    void setColor(const ofColor& c);
    void setColor(const ofFloatColor& c);
    void setColor(int r, int g, int b, int a = 255);
    void setFill(bool fill);
    void setLineWidth(float w);

    bool  getFill()      const { return fill_; }
    float getLineWidth() const { return lineWidth_; }

    // --- Primitives (local-space coordinates) ---
    void drawRect(float x, float y, float w, float h);
    void drawRect(const ofRectangle& r);
    void drawRectRounded(float x, float y, float w, float h, float radius);
    void drawCircle(float x, float y, float radius);
    void drawEllipse(float x, float y, float rx, float ry);
    void drawLine(float x1, float y1, float x2, float y2);
    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);

    // Cubic bezier — same argument order as ofDrawBezier / ofBezier:
    //   (x0,y0) = start,  (x1,y1) = cp1,  (x2,y2) = cp2,  (x3,y3) = end
    void drawBezier(float x0, float y0, float x1, float y1,
                    float x2, float y2, float x3, float y3);

    // --- Polyline / filled polygon path (mirrors ofBeginShape / ofVertex / ofEndShape) ---
    void beginShape();
    void vertex(float x, float y);
    void vertex(const glm::vec2& p);
    void endShape(bool close = false);

    // --- ofPath rendering ---
    // Render an ofPath using the current fill/color/lineWidth state.
    // Coordinates must be in DrawList local space (0,0 = canvas top-left).
    void drawPath(const ofPath& p);

    // Render an ofPath applying a uniform scale-and-translate first.
    //   canvas_local = translate + path_coord * scale
    // Typical vector-editor use: translate = pan - center*zoom, scale = zoom.
    void drawPath(const ofPath& p, glm::vec2 translate, float scale = 1.0f);

    // --- Escape hatch: direct ImDrawList access ---
    ImDrawList* raw()    const { return dl_; }
    ImVec2      origin() const { return origin_; }

    // Coordinate helper: local → screen
    ImVec2 toScreen(float x, float y) const;
    ImVec2 toScreen(const glm::vec2& p) const;

private:
    ImU32 col() const { return color_; }

    ImDrawList*         dl_        = nullptr;
    ImVec2              origin_    = {0.f, 0.f};
    ImU32               color_     = IM_COL32_WHITE;
    bool                fill_      = true;
    float               lineWidth_ = 1.f;
    std::vector<ImVec2> path_;
};

// ---------------------------------------------------------------------------
// Canvas
// ---------------------------------------------------------------------------
class Canvas {
public:
    // Reserves an ImGui region and sets up a clipped draw surface.
    // Always returns true (mirrors ImGui::Begin pattern — always call end()).
    bool begin(const char* id, ImVec2 size, bool drawBorder = true);
    void end();

    DrawList&  getDrawList() { return dl_; }

    bool       isHovered() const { return hovered_; }
    bool       isActive()  const { return active_;  }  // left or right mouse held

    // Mouse position in local (canvas) coordinates.
    glm::vec2  getMousePos() const;
    glm::vec2  getSize()     const { return {size_.x, size_.y}; }

private:
    DrawList dl_;
    ImVec2   pos_     = {0.f, 0.f};
    ImVec2   size_    = {0.f, 0.f};
    bool     hovered_ = false;
    bool     active_  = false;
    bool     begun_   = false;
};

} // namespace ofxImGui

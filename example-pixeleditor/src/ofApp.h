#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

// ---------------------------------------------------------------------------
// example-pixeleditor
//
// Shows both integration patterns side by side:
//
//  VIEWPORT pattern (main window)
//   - Pixel grid tiles seamlessly across the full window (no letterbox)
//   - Painting wraps around — Asteroids-style toroidal topology
//   - ofxImGui::Viewport captures mouse; painting logic uses world coords
//   - Left-click to paint, right-click to erase, scroll to zoom
//
//  CANVAS pattern (ImGui panel → "Magnifier" section)
//   - ofxImGui::Canvas widget showing a 7×7 magnified view around the cursor
//   - Neighbours wrap around the grid edges (same toroidal logic)
// ---------------------------------------------------------------------------

class ofApp : public ofBaseApp {
public:
    void setup()                           override;
    void draw()                            override;
    void mouseScrolled(ofMouseEventArgs& e) override;

private:
    void drawGrid();
    void drawControls();
    void drawMagnifier();
    void paintAt(glm::vec2 worldMouse, bool erase);

    // Grid resolves wrapped coords: ((v % dim) + dim) % dim
    static constexpr int kW = 32;
    static constexpr int kH = 32;

    ofxImGui::Gui      gui_;
    ofxImGui::Viewport vp_;
    ofxImGui::Canvas   canvas_;   // magnifier inside the ImGui panel

    ofColor grid_[kH][kW];

    // Brush
    float brushCol_[4]   = { 1.f, 0.4f, 0.1f, 1.f };
    int   brushSize_     = 1;

    // View
    float cellSz_        = 16.f;  // pixels per grid cell (scroll to zoom)
    bool  showGrid_      = true;
    bool  showTileBounds_ = true;
};

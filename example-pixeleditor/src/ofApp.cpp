#include "ofApp.h"

// Wrap a cell coordinate into [0, dim)
static inline int wrap(int v, int dim) { return ((v % dim) + dim) % dim; }

void ofApp::setup() {
    ofSetWindowTitle("ofxImGui — example-pixeleditor");
    gui_.setup(nullptr, true);

    for (int y = 0; y < kH; ++y)
        for (int x = 0; x < kW; ++x)
            grid_[y][x] = ofColor(0, 0, 0, 0);
}

// ---------------------------------------------------------------------------
void ofApp::draw() {
    ofBackground(28, 28, 36);

    // ---- 1. Draw tiled grid with plain OF ----
    drawGrid();

    // ---- 2. ImGui frame: capture input + controls ----
    gui_.begin();

    // Viewport covers the full window — no camera, screen == world coords
    vp_.update(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));

    if (vp_.isActive()) {
        bool erase = ImGui::GetIO().MouseDown[1];
        bool paint = ImGui::GetIO().MouseDown[0];
        if (paint || erase)
            paintAt(vp_.getWorldMouse(), erase);
    }

    drawControls();
    gui_.end();

    // ---- 3. Hover highlight drawn AFTER gui_.end() (above ImGui panels) ----
    // Shown on every visible tile so the wrap-around is immediately obvious.
    if (!vp_.isHovered() || cellSz_ < 4.f) return;

    glm::vec2 mp  = vp_.getWorldMouse();
    int hx = wrap((int)std::floor(mp.x / cellSz_), kW);
    int hy = wrap((int)std::floor(mp.y / cellSz_), kH);

    float tileW   = kW * cellSz_;
    float tileH   = kH * cellSz_;
    int   tilesX  = (int)std::ceil(ofGetWidth()  / tileW) + 1;
    int   tilesY  = (int)std::ceil(ofGetHeight() / tileH) + 1;
    int   half    = brushSize_ / 2;

    ofSetLineWidth(1.5f);
    for (int ty = 0; ty < tilesY; ++ty) {
        for (int tx = 0; tx < tilesX; ++tx) {
            float ox = tx * tileW;
            float oy = ty * tileH;

            for (int dy = -half; dy < brushSize_ - half; ++dy) {
                for (int dx = -half; dx < brushSize_ - half; ++dx) {
                    int nx = wrap(hx + dx, kW);
                    int ny = wrap(hy + dy, kH);
                    float px = ox + nx * cellSz_;
                    float py = oy + ny * cellSz_;

                    ofSetColor(255, 255, 255, 50);
                    ofFill();
                    ofDrawRectangle(px, py, cellSz_, cellSz_);
                    ofSetColor(255, 255, 255, 200);
                    ofNoFill();
                    ofDrawRectangle(px, py, cellSz_, cellSz_);
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
void ofApp::drawGrid() {
    float tileW  = kW * cellSz_;
    float tileH  = kH * cellSz_;
    int   tilesX = (int)std::ceil(ofGetWidth()  / tileW) + 1;
    int   tilesY = (int)std::ceil(ofGetHeight() / tileH) + 1;

    ofColor ck[2] = { ofColor(70, 70, 70), ofColor(100, 100, 100) };

    for (int ty = 0; ty < tilesY; ++ty) {
        for (int tx = 0; tx < tilesX; ++tx) {
            float ox = tx * tileW;
            float oy = ty * tileH;

            // Checkerboard background (transparency indicator)
            ofFill();
            for (int y = 0; y < kH; ++y) {
                for (int x = 0; x < kW; ++x) {
                    ofSetColor(ck[(x + y) & 1]);
                    ofDrawRectangle(ox + x * cellSz_, oy + y * cellSz_, cellSz_, cellSz_);
                }
            }

            // Pixel data
            for (int y = 0; y < kH; ++y) {
                for (int x = 0; x < kW; ++x) {
                    const ofColor& c = grid_[y][x];
                    if (c.a == 0) continue;
                    ofSetColor(c);
                    ofDrawRectangle(ox + x * cellSz_, oy + y * cellSz_, cellSz_, cellSz_);
                }
            }

            // Per-pixel grid lines
            if (showGrid_ && cellSz_ >= 6.f) {
                ofSetColor(0, 0, 0, 55);
                ofNoFill();
                ofSetLineWidth(1.f);
                for (int x = 0; x <= kW; ++x)
                    ofDrawLine(ox + x * cellSz_, oy, ox + x * cellSz_, oy + tileH);
                for (int y = 0; y <= kH; ++y)
                    ofDrawLine(ox, oy + y * cellSz_, ox + tileW, oy + y * cellSz_);
            }
        }
    }

    // Tile boundary lines — slightly more prominent so you can see the repeat
    if (showTileBounds_) {
        ofSetColor(160, 160, 220, 90);
        ofSetLineWidth(1.5f);
        for (int tx = 1; tx < tilesX; ++tx)
            ofDrawLine(tx * tileW, 0, tx * tileW, ofGetHeight());
        for (int ty = 1; ty < tilesY; ++ty)
            ofDrawLine(0, ty * tileH, ofGetWidth(), ty * tileH);
    }
}

// ---------------------------------------------------------------------------
void ofApp::drawControls() {
    ImGui::SetNextWindowPos({ 10, 10 }, ImGuiCond_Always);
    ImGui::SetNextWindowSize({ 185, 0 }, ImGuiCond_Always);
    ImGui::Begin("Pixel Editor", nullptr,
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(140.f);

    ImGui::SeparatorText("Brush");
    ImGui::ColorEdit4("Color##b", brushCol_);
    ImGui::Text("Size");
    ImGui::SameLine();
    ImGui::RadioButton("1x1", &brushSize_, 1); ImGui::SameLine();
    ImGui::RadioButton("2x2", &brushSize_, 2); ImGui::SameLine();
    ImGui::RadioButton("3x3", &brushSize_, 3);

    ImGui::SeparatorText("View");
    ImGui::SliderFloat("Zoom", &cellSz_, 6.f, 48.f, "%.0f px");
    ImGui::Checkbox("Pixel grid",    &showGrid_);
    ImGui::Checkbox("Tile bounds",   &showTileBounds_);

    ImGui::SeparatorText("Canvas");
    if (ImGui::Button("Clear", {80, 0}))
        for (int y = 0; y < kH; ++y)
            for (int x = 0; x < kW; ++x)
                grid_[y][x] = ofColor(0, 0, 0, 0);

    ImGui::Spacing();
    ImGui::SeparatorText("Help");
    ImGui::TextDisabled("LMB       paint");
    ImGui::TextDisabled("RMB       erase");
    ImGui::TextDisabled("Scroll    zoom");
    ImGui::TextDisabled("Edges wrap around");

    // ---- Canvas magnifier ----
    // Renders a 7×7 zoomed view around the cursor using a Canvas widget.
    // Neighbours wrap — same toroidal logic as the main viewport.
    ImGui::SeparatorText("Magnifier");
    ImGui::PopItemWidth();
    drawMagnifier();
    ImGui::End();
}

// ---------------------------------------------------------------------------
// Canvas magnifier — 7×7 zoomed view, wrapping at grid edges.
void ofApp::drawMagnifier() {
    constexpr int kView = 7;

    // Cursor cell (wrapped)
    glm::vec2 mp = vp_.getWorldMouse();
    int hx = wrap((int)std::floor(mp.x / cellSz_), kW);
    int hy = wrap((int)std::floor(mp.y / cellSz_), kH);

    float magSz = ImGui::GetContentRegionAvail().x;
    if (!canvas_.begin("##mag", { magSz, magSz })) return;

    auto& dl = canvas_.getDrawList();
    float ms  = magSz / static_cast<float>(kView);

    for (int dy = 0; dy < kView; ++dy) {
        for (int dx = 0; dx < kView; ++dx) {
            // Wrapped neighbour — never out of bounds
            int gx = wrap(hx + dx - kView / 2, kW);
            int gy = wrap(hy + dy - kView / 2, kH);

            // Checkerboard
            ImU32 ck = ((dx + dy) & 1)
                ? IM_COL32(100, 100, 100, 255)
                : IM_COL32(70,  70,  70,  255);
            dl.raw()->AddRectFilled(
                dl.toScreen( dx      * ms,  dy      * ms),
                dl.toScreen((dx + 1) * ms, (dy + 1) * ms), ck);

            // Pixel colour
            const ofColor& c = grid_[gy][gx];
            if (c.a > 0) {
                dl.setColor(c);
                dl.setFill(true);
                dl.drawRect(dx * ms, dy * ms, ms, ms);
            }
        }
    }

    // Highlight the centre cell (cursor position)
    int cx = kView / 2;
    dl.setColor(ofColor(255, 255, 255, 230));
    dl.setFill(false);
    dl.setLineWidth(2.f);
    dl.drawRect(cx * ms, cx * ms, ms, ms);

    dl.raw()->AddText(dl.toScreen(2.f, magSz - 13.f),
                      IM_COL32(100, 100, 120, 180), "canvas · 7×7 zoom");

    canvas_.end();
}

// ---------------------------------------------------------------------------
void ofApp::paintAt(glm::vec2 worldMouse, bool erase) {
    // Wrap cell coordinates — negative mouse positions handled correctly
    int cx = wrap((int)std::floor(worldMouse.x / cellSz_), kW);
    int cy = wrap((int)std::floor(worldMouse.y / cellSz_), kH);

    ofColor col(
        static_cast<unsigned char>(brushCol_[0] * 255),
        static_cast<unsigned char>(brushCol_[1] * 255),
        static_cast<unsigned char>(brushCol_[2] * 255),
        static_cast<unsigned char>(brushCol_[3] * 255));
    if (erase) col = ofColor(0, 0, 0, 0);

    int half = brushSize_ / 2;
    for (int dy = -half; dy < brushSize_ - half; ++dy)
        for (int dx = -half; dx < brushSize_ - half; ++dx)
            grid_[wrap(cy + dy, kH)][wrap(cx + dx, kW)] = col;
}

// ---------------------------------------------------------------------------
void ofApp::mouseScrolled(ofMouseEventArgs& e) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    cellSz_ = std::max(6.f, std::min(48.f, cellSz_ + e.scrollY * 1.5f));
}

#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include <vector>

// ---------------------------------------------------------------------------
// example-Conway
//
// Conway's Game of Life on a toroidal (wrapping) grid, demonstrating both
// ofxImGui integration patterns:
//
//  VIEWPORT pattern (main window)
//   - Grid tiles seamlessly across the full window — Asteroids-style edges
//   - Draw mode: LMB paint alive, RMB erase
//   - Stamp mode: place classic Life patterns centred on the cursor
//   - Scroll to zoom
//
//  CANVAS pattern (ImGui panels)
//   - Population history sparkline (Controls panel)
//   - Static pattern preview (Notes panel)
//
// Three ImGui windows:
//   • Controls  — simulation speed, draw/stamp mode, grid settings
//   • Library   — scrollable pattern list with category filter
//   • Notes     — rich info + mini preview for the selected pattern
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Pattern metadata
// ---------------------------------------------------------------------------
struct Pattern {
    const char* name;
    const char* category;   // "Spaceship" | "Gun" | "Oscillator" | "Still Life" | "Methuselah"
    const char* creator;
    int         year;
    const char* description;
    const char* funFact;
    std::vector<glm::ivec2> cells;  // relative offsets (centred on bbox centre)
};

// Declared in ofApp.cpp — accessed globally for the filter combo
extern const std::vector<Pattern> kPatterns;

// ---------------------------------------------------------------------------
class ofApp : public ofBaseApp {
public:
    void setup()                            override;
    void update()                           override;
    void draw()                             override;
    void mouseScrolled(ofMouseEventArgs& e) override;
    void keyPressed(ofKeyEventArgs& e)      override;

private:
    // Simulation
    void stepConway();
    void randomize(float density = 0.3f);
    void clearGrid();
    void resizeGrid(int w, int h);

    // Drawing / stamping
    void paintAt(glm::vec2 worldMouse, bool erase);
    void stamp(int cx, int cy, int patternIdx);

    // Rendering
    void drawCells();
    void drawStampGhost();
    void drawHoverHighlight();

    // ImGui panels
    void drawControlsPanel();
    void drawLibraryPanel();
    void drawNotesPanel();

    // Helpers
    int  cellIdx(int x, int y)  const { return y * kW_ + x; }
    int  wrapX(int x)           const { return ((x % kW_) + kW_) % kW_; }
    int  wrapY(int y)           const { return ((y % kH_) + kH_) % kH_; }
    glm::ivec2 patternCenter(int pi) const;  // bbox centre of pattern i

    // ---- Grid (dynamic size) ----
    int kW_  = 64;
    int kH_  = 64;
    std::vector<uint8_t> cells_;   // live=1, dead=0
    std::vector<uint8_t> next_;    // double-buffer

    // ---- Simulation state ----
    bool  running_      = false;
    float speed_        = 10.f;    // generations per second
    float lastStepTime_ = 0.f;
    int   generation_   = 0;
    int   population_   = 0;

    // Population history for sparkline (ring buffer)
    static constexpr int kHistLen = 200;
    std::vector<int> popHistory_;

    // ---- ImGui integration ----
    ofxImGui::Gui      gui_;
    ofxImGui::Viewport vp_;
    ofxImGui::Canvas   canvas_;   // reused for sparkline and pattern preview

    // ---- View ----
    float cellSz_         = 8.f;
    bool  showGrid_       = false;
    bool  showTileBounds_ = true;

    // ---- Draw / Stamp mode ----
    enum class Mode { Draw, Stamp } mode_ = Mode::Draw;
    int  patternIdx_    = 0;    // index into kPatterns
    int  filterCat_     = 0;   // 0 = All, 1..N = category

    // ---- Appearance ----
    float liveCol_[3]   = { 0.18f, 0.95f, 0.45f };  // bright green
    float deadColB_     = 0.06f;                      // background brightness
    float randomDens_   = 0.3f;

    // ---- Grid size presets ----
    static constexpr int kSizeCount    = 5;
    static constexpr int kSizes[kSizeCount][2] = {
        {32,32}, {48,48}, {64,64}, {96,96}, {128,128}
    };
    int sizePreset_ = 2;  // default 64×64
};

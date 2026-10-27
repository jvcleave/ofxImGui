#include "ofApp.h"
#include <algorithm>
#include <numeric>

// ===========================================================================
// Pattern library
// ===========================================================================
// Cells are relative offsets; stamp() centres them on the click point.
const std::vector<Pattern> kPatterns = {
// ---------------------------------------------------------------------------
// SPACESHIPS
// ---------------------------------------------------------------------------
{
    "Glider", "Spaceship", "Richard K. Guy / Conway's group", 1970,
    "The iconic 5-cell diagonal traveller, moving one step every 4 generations "
    "(c/4 diagonal speed). Conway called it 'the most common object in the Life "
    "universe'. Its discovery in 1970 sparked the entire field of Life research.",
    "Gosper used the Glider to prove Life is Turing-complete — an infinite stream "
    "of Gliders can encode any computation.",
    {{1,0},{2,1},{0,2},{1,2},{2,2}}
},
{
    "LWSS", "Spaceship", "Conway's group", 1970,
    "Lightweight Spaceship — the smallest natural orthogonal spaceship. "
    "Moves at c/2 (one row every two generations). "
    "Often emerges spontaneously from random configurations.",
    "The 'LW' stands for Lightweight. The 'standard' spaceships (LW/MW/HW) "
    "are among the first non-glider movers ever documented.",
    {{1,0},{4,0},{0,1},{0,2},{4,2},{0,3},{1,3},{2,3},{3,3}}
},
{
    "MWSS", "Spaceship", "Conway's group", 1970,
    "Middleweight Spaceship — one step up from the LWSS, also moving at c/2. "
    "Slightly wider and more asymmetric. Requires careful stabilisation at its tail.",
    "MWSS cannot be produced by two-Glider collisions, unlike the LWSS — "
    "it needs at least three.",
    {{2,0},{0,1},{4,1},{5,2},{0,3},{5,3},{1,4},{2,4},{3,4},{4,4},{5,4}}
},
{
    "HWSS", "Spaceship", "Conway's group", 1970,
    "Heavyweight Spaceship — the largest of the three 'classic' orthogonal "
    "spaceships, moving at c/2. Its wider body makes it notably harder to "
    "create from Glider collisions.",
    "All three classic spaceships (LWSS/MWSS/HWSS) were found in the first "
    "weeks after Conway published the rules in Scientific American, Oct 1970.",
    {{2,0},{3,0},{0,1},{5,1},{6,2},{0,3},{6,3},{1,4},{2,4},{3,4},{4,4},{5,4},{6,4}}
},
// ---------------------------------------------------------------------------
// GUNS
// ---------------------------------------------------------------------------
{
    "Gosper Glider Gun", "Gun", "Bill Gosper", 1970,
    "The first pattern ever discovered that exhibits infinite growth. "
    "Bill Gosper found it in response to Conway's $50 prize for a pattern that "
    "grows without bound. It emits a new Glider every 30 generations — forever.",
    "The Gosper Gun was the key to proving Life is computationally universal: "
    "you can build logic gates from interacting Glider streams.",
    {
        // Stable left block
        {0,4},{1,4},{0,5},{1,5},
        // Left part of the gun
        {10,4},{10,5},{10,6},
        {11,3},{11,7},
        {12,2},{12,8},
        {13,2},{13,8},
        {14,5},
        {15,3},{15,7},
        {16,4},{16,5},{16,6},
        {17,5},
        // Right part of the gun
        {20,2},{20,3},
        {21,2},{21,3},
        {22,4},{24,4},
        {22,5},
        {24,5},
        // Stable right block
        {34,2},{35,2},
        {34,3},{35,3},
        // Glider being birthed
        {24,1},{22,0}
    }
},
// ---------------------------------------------------------------------------
// OSCILLATORS
// ---------------------------------------------------------------------------
{
    "Blinker", "Oscillator", "Conway's group", 1970,
    "Period-2 oscillator — the simplest and most common oscillator in Life. "
    "Just 3 cells alternating between horizontal and vertical orientation. "
    "Appears constantly in random universes.",
    "There are more Blinkers in a typical random Life soup than any other "
    "non-trivial pattern. They're the 'hydrogen atom' of the Life universe.",
    {{0,0},{1,0},{2,0}}
},
{
    "Toad", "Oscillator", "Simon Norton", 1970,
    "Period-2 oscillator, 6 cells. One of the earliest oscillators discovered "
    "after the Blinker. The two rows shift by one cell each generation, giving "
    "it a vaguely toad-like hopping appearance.",
    "The Toad is the second most common period-2 oscillator after the Blinker. "
    "It can be created by just two Gliders colliding at the right angle.",
    {{1,0},{2,0},{3,0},{0,1},{1,1},{2,1}}
},
{
    "Beacon", "Oscillator", "Conway's group", 1970,
    "Period-2 oscillator, 8 cells. Two diagonally adjacent 2×2 blocks that "
    "alternately share and lose their touching corner cell. "
    "Named for its flickering lighthouse appearance.",
    "The Beacon is the third most common period-2 oscillator. "
    "It is the smallest known oscillator with more than one cell.",
    {{0,0},{1,0},{0,1},{3,2},{2,3},{3,3}}
},
{
    "Pulsar", "Oscillator", "Conway's group", 1970,
    "Period-3 oscillator, 48 cells. The most visually stunning small oscillator "
    "in Life. Its 12-fold symmetry and complex pulsing motion make it immediately "
    "recognisable. Third most common oscillator overall.",
    "The Pulsar has threefold rotational symmetry AND fourfold reflective symmetry "
    "— 12-way symmetry in total. Almost no natural object achieves this.",
    {
        // Top horizontal bars (row 0)
        {2,0},{3,0},{4,0},{8,0},{9,0},{10,0},
        // Left/right verticals (rows 2-4)
        {0,2},{5,2},{7,2},{12,2},
        {0,3},{5,3},{7,3},{12,3},
        {0,4},{5,4},{7,4},{12,4},
        // Middle horizontal bars (row 5)
        {2,5},{3,5},{4,5},{8,5},{9,5},{10,5},
        // Middle horizontal bars (row 7)
        {2,7},{3,7},{4,7},{8,7},{9,7},{10,7},
        // Left/right verticals (rows 8-10)
        {0,8},{5,8},{7,8},{12,8},
        {0,9},{5,9},{7,9},{12,9},
        {0,10},{5,10},{7,10},{12,10},
        // Bottom horizontal bars (row 12)
        {2,12},{3,12},{4,12},{8,12},{9,12},{10,12}
    }
},
{
    "Pentadecathlon", "Oscillator", "Conway's group", 1970,
    "Period-15 oscillator, 13 cells. The longest-period oscillator among the "
    "simplest patterns. Begins as a distinctive 3-row shape and cycles through "
    "a beautiful sequence of 15 distinct phases.",
    "'Pentadecathlon' means 15 (penta=5, deca=10). It has period 15 — the "
    "longest period ever found in such a compact pattern for over a decade.",
    {
        {2,0},{8,0},
        {0,1},{1,1},{3,1},{4,1},{5,1},{6,1},{7,1},{9,1},{10,1},
        {2,2},{8,2}
    }
},
// ---------------------------------------------------------------------------
// STILL LIFES
// ---------------------------------------------------------------------------
{
    "Block", "Still Life", "Conway's group", 1970,
    "The most common still life — a 2×2 square that is completely stable. "
    "Every cell has exactly 3 live neighbours, and no dead cell adjacent to "
    "the block has 3 live neighbours. Perfect self-stability.",
    "Blocks are the most frequently produced object in Life. A random soup "
    "of moderate density typically settles with hundreds of Blocks.",
    {{0,0},{1,0},{0,1},{1,1}}
},
{
    "Beehive", "Still Life", "Conway's group", 1970,
    "Second most common still life, 6 cells. Its hexagonal outline resembles "
    "a honeycomb cell. Frequently produced by decaying unstable patterns "
    "and Glider collisions.",
    "The Beehive is one of the few still lifes that can be naturally produced "
    "by just two Gliders colliding. It appears in almost every large random run.",
    {{1,0},{2,0},{0,1},{3,1},{1,2},{2,2}}
},
// ---------------------------------------------------------------------------
// METHUSELAHS
// ---------------------------------------------------------------------------
{
    "R-pentomino", "Methuselah", "John Horton Conway", 1970,
    "Just 5 cells — Conway's original test case for Life. Despite its tiny size, "
    "it runs for 1,103 generations before finally stabilising. The final "
    "census: 8 Gliders, 4 Blinkers, 1 Beehive, and several Blocks.",
    "Conway spent weeks manually simulating the R-pentomino on a Go board before "
    "computers became available. It was the discovery that convinced him Life "
    "had interesting long-term dynamics.",
    {{1,0},{2,0},{0,1},{1,1},{1,2}}
},
{
    "Acorn", "Methuselah", "Charles Corderman", 1971,
    "Only 7 cells, yet runs for 5,206 generations — one of the longest-running "
    "known Methuselahs. Final population: 633 cells including 13 Gliders, "
    "4 Glider guns, and hundreds of stable objects.",
    "Charles Corderman found Acorn in 1971 while systematically searching for "
    "long-lived small patterns. Its 5206-generation lifespan wasn't surpassed "
    "by any smaller pattern for decades.",
    {{1,0},{3,1},{0,2},{1,2},{4,2},{5,2},{6,2}}
},
{
    "Diehard", "Methuselah", "Unknown", 1971,
    "7 cells that die completely after exactly 130 generations — not a single "
    "live cell survives. A rare 'extinction event'. No pattern of fewer than "
    "7 cells is known to take longer to disappear.",
    "The name 'Diehard' is apt: it fights for 130 generations before every "
    "last cell goes dark. The exact cell count on generation 129 is 57.",
    {{6,0},{0,1},{1,1},{1,2},{5,2},{6,2},{7,2}}
},
// ---------------------------------------------------------------------------
// CURSOR SPECIAL ★
// ---------------------------------------------------------------------------
{
    "The Cursor", "Methuselah", "Cursor AI", 2026,
    "An experimental 9-cell Methuselah in the shape of a classic mouse-cursor "
    "arrow, created specifically for this example in 2026. Its long-term behaviour "
    "under Life's rules is an open experiment — run it and observe.",
    "The first Conway pattern ever attributed to an AI coding assistant. "
    "Draw mode is your canvas; The Cursor is your brush. What does it become?",
    // Arrow pointing upper-left, tip at origin
    {{0,0},
     {0,1},{1,1},
     {0,2},{2,2},
     {0,3},{3,3},
     {0,4},{4,4}}
},
};

// ===========================================================================
// Helpers
// ===========================================================================
static const char* kCategories[] = {
    "All", "Spaceship", "Gun", "Oscillator", "Still Life", "Methuselah"
};
static constexpr int kCatCount = 6;

static ImVec4 categoryColour(const char* cat) {
    if (!strcmp(cat, "Spaceship"))  return {0.3f, 0.6f, 1.0f, 1.f};
    if (!strcmp(cat, "Gun"))        return {1.0f, 0.4f, 0.3f, 1.f};
    if (!strcmp(cat, "Oscillator")) return {0.7f, 0.4f, 1.0f, 1.f};
    if (!strcmp(cat, "Still Life")) return {0.6f, 0.6f, 0.6f, 1.f};
    if (!strcmp(cat, "Methuselah")) return {1.0f, 0.8f, 0.2f, 1.f};
    return {0.2f, 1.0f, 0.7f, 1.f}; // Cursor Special / unknown
}

// ===========================================================================
// Setup
// ===========================================================================
void ofApp::setup() {
    ofSetWindowTitle("ofxImGui — example-Conway");
    gui_.setup(nullptr, true);
    resizeGrid(kW_, kH_);
}

// ===========================================================================
// Update
// ===========================================================================
void ofApp::update() {
    if (!running_) return;
    float now = ofGetElapsedTimef();
    float interval = 1.f / std::max(speed_, 0.5f);
    while (now - lastStepTime_ >= interval) {
        stepConway();
        lastStepTime_ += interval;
    }
}

// ===========================================================================
// Draw
// ===========================================================================
void ofApp::draw() {
    float bg = deadColB_ * 255.f;
    ofBackground(bg, bg, bg);

    // ---- 1. Tiled cell rendering ----
    drawCells();

    // ---- 2. ImGui frame ----
    gui_.begin();
    vp_.update(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));

    // Draw mode — LMB alive, RMB erase
    if (mode_ == Mode::Draw && vp_.isActive()) {
        bool erase = ImGui::GetIO().MouseDown[1];
        bool paint = ImGui::GetIO().MouseDown[0];
        if (paint || erase)
            paintAt(vp_.getWorldMouse(), erase);
    }
    // Stamp mode — single click to place
    if (mode_ == Mode::Stamp && vp_.isClicked()) {
        glm::vec2 mp = vp_.getWorldMouse();
        int cx = ((int)std::floor(mp.x / cellSz_) % kW_ + kW_) % kW_;
        int cy = ((int)std::floor(mp.y / cellSz_) % kH_ + kH_) % kH_;
        stamp(cx, cy, patternIdx_);
    }

    drawControlsPanel();
    drawLibraryPanel();
    drawNotesPanel();
    gui_.end();

    // ---- 3. Overlays drawn AFTER gui_.end() (above ImGui windows) ----
    if (mode_ == Mode::Stamp && vp_.isHovered())
        drawStampGhost();
    else if (mode_ == Mode::Draw && vp_.isHovered())
        drawHoverHighlight();
}

// ===========================================================================
// Conway step (toroidal)
// ===========================================================================
void ofApp::stepConway() {
    for (int y = 0; y < kH_; ++y) {
        for (int x = 0; x < kW_; ++x) {
            int n = 0;
            for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx)
                    if (dx || dy)
                        n += cells_[cellIdx(wrapX(x+dx), wrapY(y+dy))];
            bool alive = cells_[cellIdx(x, y)];
            next_[cellIdx(x, y)] = uint8_t(alive ? (n==2||n==3) : (n==3));
        }
    }
    std::swap(cells_, next_);
    ++generation_;

    population_ = 0;
    for (auto c : cells_) population_ += c;

    popHistory_.push_back(population_);
    if ((int)popHistory_.size() > kHistLen)
        popHistory_.erase(popHistory_.begin());
}

// ===========================================================================
// Grid rendering
// ===========================================================================
void ofApp::drawCells() {
    float tileW  = kW_ * cellSz_;
    float tileH  = kH_ * cellSz_;
    int   tilesX = (int)std::ceil(ofGetWidth()  / tileW) + 1;
    int   tilesY = (int)std::ceil(ofGetHeight() / tileH) + 1;

    ofFill();
    for (int ty = 0; ty < tilesY; ++ty) {
        for (int tx = 0; tx < tilesX; ++tx) {
            float ox = tx * tileW;
            float oy = ty * tileH;

            // Live cells
            ofSetColor(
                liveCol_[0] * 255.f,
                liveCol_[1] * 255.f,
                liveCol_[2] * 255.f);
            for (int y = 0; y < kH_; ++y)
                for (int x = 0; x < kW_; ++x)
                    if (cells_[cellIdx(x, y)])
                        ofDrawRectangle(ox + x*cellSz_, oy + y*cellSz_, cellSz_, cellSz_);

            // Pixel grid
            if (showGrid_ && cellSz_ >= 6.f) {
                ofSetColor(255, 255, 255, 18);
                ofNoFill();
                ofSetLineWidth(0.5f);
                for (int x = 0; x <= kW_; ++x)
                    ofDrawLine(ox + x*cellSz_, oy, ox + x*cellSz_, oy + tileH);
                for (int y = 0; y <= kH_; ++y)
                    ofDrawLine(ox, oy + y*cellSz_, ox + tileW, oy + y*cellSz_);
            }
        }
    }

    // Tile boundary lines
    if (showTileBounds_) {
        ofSetColor(255, 255, 255, 35);
        ofSetLineWidth(1.f);
        for (int tx = 1; tx < tilesX; ++tx)
            ofDrawLine(tx*tileW, 0, tx*tileW, ofGetHeight());
        for (int ty = 1; ty < tilesY; ++ty)
            ofDrawLine(0, ty*tileH, ofGetWidth(), ty*tileH);
    }
}

// ===========================================================================
// Hover / ghost overlays
// ===========================================================================
glm::ivec2 ofApp::patternCenter(int pi) const {
    const auto& cells = kPatterns[pi].cells;
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    for (const auto& c : cells) {
        minX = std::min(minX, c.x); maxX = std::max(maxX, c.x);
        minY = std::min(minY, c.y); maxY = std::max(maxY, c.y);
    }
    return { (minX + maxX) / 2, (minY + maxY) / 2 };
}

void ofApp::drawStampGhost() {
    glm::vec2 mp = vp_.getWorldMouse();
    int cx = ((int)std::floor(mp.x / cellSz_) % kW_ + kW_) % kW_;
    int cy = ((int)std::floor(mp.y / cellSz_) % kH_ + kH_) % kH_;
    glm::ivec2 ctr = patternCenter(patternIdx_);

    float tileW  = kW_ * cellSz_;
    float tileH  = kH_ * cellSz_;
    int   tilesX = (int)std::ceil(ofGetWidth()  / tileW) + 1;
    int   tilesY = (int)std::ceil(ofGetHeight() / tileH) + 1;

    ofSetColor(liveCol_[0]*255.f, liveCol_[1]*255.f, liveCol_[2]*255.f, 90);
    ofFill();
    for (int ty = 0; ty < tilesY; ++ty) {
        for (int tx = 0; tx < tilesX; ++tx) {
            float ox = tx * tileW, oy = ty * tileH;
            for (const auto& cell : kPatterns[patternIdx_].cells) {
                int gx = ((cx + cell.x - ctr.x) % kW_ + kW_) % kW_;
                int gy = ((cy + cell.y - ctr.y) % kH_ + kH_) % kH_;
                ofDrawRectangle(ox + gx*cellSz_, oy + gy*cellSz_, cellSz_, cellSz_);
            }
        }
    }
}

void ofApp::drawHoverHighlight() {
    if (cellSz_ < 4.f) return;
    glm::vec2 mp = vp_.getWorldMouse();
    int hx = ((int)std::floor(mp.x / cellSz_) % kW_ + kW_) % kW_;
    int hy = ((int)std::floor(mp.y / cellSz_) % kH_ + kH_) % kH_;

    float tileW  = kW_ * cellSz_;
    float tileH  = kH_ * cellSz_;
    int   tilesX = (int)std::ceil(ofGetWidth()  / tileW) + 1;
    int   tilesY = (int)std::ceil(ofGetHeight() / tileH) + 1;

    ofSetColor(255, 255, 255, 50);
    ofFill();
    for (int ty = 0; ty < tilesY; ++ty)
        for (int tx = 0; tx < tilesX; ++tx)
            ofDrawRectangle(tx*tileW + hx*cellSz_, ty*tileH + hy*cellSz_, cellSz_, cellSz_);

    ofSetColor(255, 255, 255, 180);
    ofNoFill();
    ofSetLineWidth(1.5f);
    for (int ty = 0; ty < tilesY; ++ty)
        for (int tx = 0; tx < tilesX; ++tx)
            ofDrawRectangle(tx*tileW + hx*cellSz_, ty*tileH + hy*cellSz_, cellSz_, cellSz_);
}

// ===========================================================================
// ImGui — Controls panel
// ===========================================================================
void ofApp::drawControlsPanel() {
    ImGui::SetNextWindowPos({10, 10}, ImGuiCond_Once);
    ImGui::SetNextWindowSize({210, 0}, ImGuiCond_Once);
    ImGui::Begin("Conway Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PushItemWidth(150.f);

    // -- Simulation --
    ImGui::SeparatorText("Simulation");
    if (ImGui::Button(running_ ? "  Pause  " : "  Play   "))
        running_ = !running_;
    ImGui::SameLine();
    if (ImGui::Button("Step"))
        stepConway();
    ImGui::SliderFloat("Speed", &speed_, 1.f, 60.f, "%.0f gen/s");

    ImGui::Spacing();
    ImGui::Text("Generation  %d", generation_);
    ImGui::Text("Population  %d / %d", population_, kW_ * kH_);

    // Population sparkline (Canvas widget)
    float graphW = ImGui::GetContentRegionAvail().x;
    if (canvas_.begin("##pop", {graphW, 55.f}, true)) {
        auto& dl   = canvas_.getDrawList();
        glm::vec2 sz = canvas_.getSize();
        dl.raw()->AddRectFilled(dl.toScreen(0,0), dl.toScreen(sz.x, sz.y),
                                IM_COL32(10, 12, 10, 255));
        if ((int)popHistory_.size() > 1) {
            int maxPop = *std::max_element(popHistory_.begin(), popHistory_.end());
            maxPop = std::max(maxPop, 1);
            dl.setColor(ofColor(
                (int)(liveCol_[0]*255), (int)(liveCol_[1]*255), (int)(liveCol_[2]*255), 220));
            dl.setLineWidth(1.5f);
            dl.setFill(false);
            dl.beginShape();
            for (int i = 0; i < (int)popHistory_.size(); ++i) {
                float x = (float)i / (kHistLen-1) * sz.x;
                float y = sz.y - (float)popHistory_[i] / maxPop * sz.y * 0.92f;
                dl.vertex(x, y);
            }
            dl.endShape(false);
        }
        dl.raw()->AddText(dl.toScreen(3.f, sz.y - 13.f),
                          IM_COL32(80,100,80,200), "population history");
        canvas_.end();
    }

    // -- Mode --
    ImGui::SeparatorText("Mode");
    bool drawMode  = (mode_ == Mode::Draw);
    bool stampMode = (mode_ == Mode::Stamp);
    if (ImGui::RadioButton("Draw",  drawMode))  mode_ = Mode::Draw;
    ImGui::SameLine();
    if (ImGui::RadioButton("Stamp", stampMode)) mode_ = Mode::Stamp;

    // -- View --
    ImGui::SeparatorText("View");
    ImGui::SliderFloat("Zoom", &cellSz_, 4.f, 32.f, "%.0f px");
    ImGui::Checkbox("Pixel grid",   &showGrid_);
    ImGui::Checkbox("Tile bounds",  &showTileBounds_);

    // -- Grid size --
    ImGui::SeparatorText("Grid");
    static const char* sizeLabels[] = { "32×32","48×48","64×64","96×96","128×128" };
    if (ImGui::Combo("Size", &sizePreset_, sizeLabels, kSizeCount)) {
        resizeGrid(kSizes[sizePreset_][0], kSizes[sizePreset_][1]);
    }
    ImGui::SliderFloat("Random density", &randomDens_, 0.05f, 0.7f, "%.2f");
    if (ImGui::Button("Randomise", {95, 0})) randomize(randomDens_);
    ImGui::SameLine();
    if (ImGui::Button("Clear", {60, 0})) clearGrid();

    // -- Appearance --
    ImGui::SeparatorText("Colour");
    ImGui::ColorEdit3("Live", liveCol_);
    ImGui::SliderFloat("BG bright", &deadColB_, 0.f, 0.25f, "%.2f");

    // -- Help --
    ImGui::SeparatorText("Help");
    ImGui::TextDisabled("Space     play/pause");
    ImGui::TextDisabled("S         single step");
    ImGui::TextDisabled("R         randomise");
    ImGui::TextDisabled("C         clear");
    ImGui::TextDisabled("Scroll    zoom");
    ImGui::TextDisabled("Edges wrap — toroidal");

    ImGui::PopItemWidth();
    ImGui::End();
}

// ===========================================================================
// ImGui — Pattern Library panel
// ===========================================================================
void ofApp::drawLibraryPanel() {
    ImGui::SetNextWindowPos({(float)ofGetWidth() - 260.f, 10.f}, ImGuiCond_Once);
    ImGui::SetNextWindowSize({250.f, 480.f}, ImGuiCond_Once);
    ImGui::Begin("Pattern Library");

    // Category filter
    ImGui::SetNextItemWidth(-1.f);
    ImGui::Combo("##cat", &filterCat_, kCategories, kCatCount);
    ImGui::Separator();

    ImGui::BeginChild("##list", {0, 0}, false);
    for (int i = 0; i < (int)kPatterns.size(); ++i) {
        const auto& p = kPatterns[i];
        if (filterCat_ > 0 && strcmp(p.category, kCategories[filterCat_]) != 0)
            continue;

        ImVec4 catCol = categoryColour(p.category);
        bool selected = (patternIdx_ == i);

        // Category badge
        ImGui::PushStyleColor(ImGuiCol_Text, catCol);
        ImGui::TextUnformatted(p.category);
        ImGui::PopStyleColor();
        ImGui::SameLine(85.f);

        // Pattern name — selectable
        ImGui::PushID(i);
        if (ImGui::Selectable(p.name, selected,
                              ImGuiSelectableFlags_AllowDoubleClick)) {
            patternIdx_ = i;
            if (ImGui::IsMouseDoubleClicked(0))
                mode_ = Mode::Stamp;  // double-click switches to stamp mode
        }
        ImGui::PopID();

        // Year in dim text
        ImGui::SameLine();
        ImGui::TextDisabled(" %d", p.year);
    }
    ImGui::EndChild();
    ImGui::End();
}

// ===========================================================================
// ImGui — Notes panel
// ===========================================================================
void ofApp::drawNotesPanel() {
    ImGui::SetNextWindowPos({(float)ofGetWidth() - 260.f, 500.f}, ImGuiCond_Once);
    ImGui::SetNextWindowSize({250.f, 290.f}, ImGuiCond_Once);
    ImGui::Begin("Pattern Notes");

    const auto& p = kPatterns[patternIdx_];
    ImVec4 catCol = categoryColour(p.category);

    // Header
    ImGui::PushStyleColor(ImGuiCol_Text, {1.f,1.f,1.f,1.f});
    ImGui::TextUnformatted(p.name);
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Text, catCol);
    ImGui::TextUnformatted(p.category);
    ImGui::PopStyleColor();
    ImGui::SameLine();
    ImGui::TextDisabled("· %s, %d", p.creator, p.year);

    ImGui::Separator();

    // Description
    ImGui::TextWrapped("%s", p.description);

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, {0.85f, 0.75f, 0.3f, 1.f});
    ImGui::TextWrapped("★ %s", p.funFact);
    ImGui::PopStyleColor();

    ImGui::Spacing();
    ImGui::SeparatorText("Preview");

    // Pattern preview Canvas
    // Render the initial cells in a small Canvas (local bbox coords)
    float cvSz = ImGui::GetContentRegionAvail().x;
    if (canvas_.begin("##preview", {cvSz, cvSz * 0.5f})) {
        auto& dl   = canvas_.getDrawList();
        glm::vec2 sz = canvas_.getSize();

        dl.raw()->AddRectFilled(dl.toScreen(0,0), dl.toScreen(sz.x, sz.y),
                                IM_COL32(10, 14, 10, 255));

        if (!p.cells.empty()) {
            // Compute bounding box
            int minX = p.cells[0].x, maxX = p.cells[0].x;
            int minY = p.cells[0].y, maxY = p.cells[0].y;
            for (const auto& c : p.cells) {
                minX = std::min(minX, c.x); maxX = std::max(maxX, c.x);
                minY = std::min(minY, c.y); maxY = std::max(maxY, c.y);
            }
            float rangeX = float(maxX - minX + 1);
            float rangeY = float(maxY - minY + 1);
            const float pad = 8.f;
            float scaleX = (sz.x - 2*pad) / std::max(rangeX, 1.f);
            float scaleY = (sz.y - 2*pad) / std::max(rangeY, 1.f);
            float s      = std::min(scaleX, scaleY);
            float offX   = pad + (sz.x - 2*pad - rangeX * s) * 0.5f;
            float offY   = pad + (sz.y - 2*pad - rangeY * s) * 0.5f;
            float cs     = std::max(s - 1.f, 1.f);

            dl.setColor(ofColor(
                (int)(liveCol_[0]*255), (int)(liveCol_[1]*255), (int)(liveCol_[2]*255)));
            dl.setFill(true);
            for (const auto& c : p.cells) {
                float cx = offX + (c.x - minX) * s;
                float cy = offY + (c.y - minY) * s;
                dl.drawRect(cx, cy, cs, cs);
            }
        }
        canvas_.end();
    }

    // Stamp button
    ImGui::Spacing();
    if (ImGui::Button("Stamp on click", {-1, 0}))
        mode_ = Mode::Stamp;

    ImGui::End();
}

// ===========================================================================
// Paint / Stamp
// ===========================================================================
void ofApp::paintAt(glm::vec2 worldMouse, bool erase) {
    int cx = ((int)std::floor(worldMouse.x / cellSz_) % kW_ + kW_) % kW_;
    int cy = ((int)std::floor(worldMouse.y / cellSz_) % kH_ + kH_) % kH_;
    cells_[cellIdx(cx, cy)] = erase ? 0 : 1;
}

void ofApp::stamp(int cx, int cy, int pi) {
    glm::ivec2 ctr = patternCenter(pi);
    for (const auto& cell : kPatterns[pi].cells) {
        int x = ((cx + cell.x - ctr.x) % kW_ + kW_) % kW_;
        int y = ((cy + cell.y - ctr.y) % kH_ + kH_) % kH_;
        cells_[cellIdx(x, y)] = 1;
    }
}

// ===========================================================================
// Grid management
// ===========================================================================
void ofApp::resizeGrid(int w, int h) {
    kW_ = w; kH_ = h;
    cells_.assign(kW_ * kH_, 0);
    next_.assign(kW_ * kH_, 0);
    generation_ = 0;
    population_ = 0;
    popHistory_.clear();
    lastStepTime_ = ofGetElapsedTimef();
}

void ofApp::randomize(float density) {
    for (int i = 0; i < kW_ * kH_; ++i)
        cells_[i] = uint8_t(ofRandom(1.f) < density ? 1 : 0);
    generation_ = 0;
    population_ = 0;
    popHistory_.clear();
    lastStepTime_ = ofGetElapsedTimef();
}

void ofApp::clearGrid() {
    std::fill(cells_.begin(), cells_.end(), 0);
    generation_ = 0;
    population_ = 0;
    popHistory_.clear();
}

// ===========================================================================
// Input
// ===========================================================================
void ofApp::mouseScrolled(ofMouseEventArgs& e) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    cellSz_ = std::max(4.f, std::min(32.f, cellSz_ + e.scrollY * 0.8f));
}

void ofApp::keyPressed(ofKeyEventArgs& e) {
    switch (e.key) {
        case ' ': running_ = !running_;           break;
        case 's': case 'S': stepConway();         break;
        case 'r': case 'R': randomize(randomDens_); break;
        case 'c': case 'C': clearGrid();          break;
        default: break;
    }
}

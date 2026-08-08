#include "ImViewport.h"
#include "ofAppRunner.h"  // ofGetWidth / ofGetHeight

namespace ofxImGui {

// ---------------------------------------------------------------------------
// Viewport
// ---------------------------------------------------------------------------

void Viewport::update(ofRectangle screenRect) {
    cam_ = nullptr;
    resolveRect(screenRect);
    resolveMouseState();
}

void Viewport::update(ofCamera& cam, ofRectangle screenRect) {
    cam_ = &cam;
    resolveRect(screenRect);
    resolveMouseState();
}

// ---------------------------------------------------------------------------
// private helpers

void Viewport::resolveRect(ofRectangle requested) {
    if (requested.width <= 0 || requested.height <= 0)
        rect_ = ofRectangle(0, 0, (float)ofGetWidth(), (float)ofGetHeight());
    else
        rect_ = requested;
}

void Viewport::resolveMouseState() {
    ImGuiIO& io = ImGui::GetIO();

    // Screen mouse (ImGui uses top-left origin, same as OF on most platforms)
    screenMouse_ = { io.MousePos.x, io.MousePos.y };

    // World mouse — carry forward previous value before overwriting
    prevWorldMouse_ = worldMouse_;
    worldMouse_ = screenToWorld(screenMouse_);

    // Hovered: mouse in rect AND ImGui not consuming the mouse
    hovered_ = !io.WantCaptureMouse
               && rect_.inside(screenMouse_.x, screenMouse_.y);

    wasActive_ = active_;

    // Active: left button held, AND the press started inside this viewport.
    // We track the "started inside" condition by checking hovered_ on press.
    if (io.MouseClicked[0] && hovered_) {
        active_  = true;
        clicked_ = true;
    } else {
        clicked_ = false;
    }

    if (active_ && !io.MouseDown[0]) {
        released_ = true;
        active_   = false;
    } else {
        released_ = false;
    }
}

// ---------------------------------------------------------------------------

bool Viewport::isNear(const glm::vec2& worldPt, float screenRadius) const {
    glm::vec2 sp = worldToScreen(worldPt);
    return glm::length(sp - screenMouse_) < screenRadius;
}

bool Viewport::isNear(const glm::vec3& worldPt, float screenRadius) const {
    glm::vec2 sp = worldToScreen(worldPt);
    return glm::length(sp - screenMouse_) < screenRadius;
}

// ---------------------------------------------------------------------------

glm::vec2 Viewport::worldToScreen(const glm::vec2& worldPt) const {
    if (!cam_) return worldPt;
    glm::vec3 s = cam_->worldToScreen(glm::vec3(worldPt.x, worldPt.y, 0.f));
    return { s.x, s.y };
}

glm::vec2 Viewport::worldToScreen(const glm::vec3& worldPt) const {
    if (!cam_) return { worldPt.x, worldPt.y };
    glm::vec3 s = cam_->worldToScreen(worldPt);
    return { s.x, s.y };
}

glm::vec2 Viewport::screenToWorld(const glm::vec2& screenPt) const {
    if (!cam_) return screenPt;
    // Unproject two points at different screen depths, then ray-march to z=0.
    // Using z=0 (near plane) alone gives the wrong answer for a perspective cam.
    // Note: 'near'/'far' are Windows macros — use p0/p1 to avoid expansion.
    glm::vec3 p0 = cam_->screenToWorld(glm::vec3(screenPt.x, screenPt.y, 0.f));
    glm::vec3 p1 = cam_->screenToWorld(glm::vec3(screenPt.x, screenPt.y, 1.f));
    float dz = p1.z - p0.z;
    if (std::abs(dz) < 1e-6f) return { p0.x, p0.y }; // orthographic / degenerate
    float t = -p0.z / dz;         // t where the ray crosses z=0
    glm::vec3 world = glm::mix(p0, p1, t);
    return { world.x, world.y };
}

// ---------------------------------------------------------------------------

void Viewport::beginDrag(int handleId) {
    dragId_ = handleId;
}

void Viewport::endDrag() {
    dragId_ = -1;
}

} // namespace ofxImGui

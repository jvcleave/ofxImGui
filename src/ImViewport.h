#pragma once

#include "imgui.h"
#include "ofCamera.h"
#include "ofRectangle.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// ---------------------------------------------------------------------------
// ofxImGui::Viewport
//
// Camera-aware input helper for interactive editing in the OF main viewport.
// Solves the screen↔world coordinate battle so you never write the conversion
// manually.
//
// Pattern:
//   1. Draw your scene content with OF (cam_.begin() / cam_.end()).
//   2. Call vp_.update(cam_) inside the ImGui frame — reads mouse state and
//      resolves world coords via the camera.
//   3. Do drag/pick logic using vp_.getWorldMouse(), vp_.isNear(), etc.
//   4. Draw editing handles with plain OF calls AFTER gui_.end(), so they
//      appear above ImGui windows.
//
// Without a camera (e.g. pixel editors): call vp_.update() — screen coords
// and world coords are identical (identity mapping).
//
// Quick example:
//
//   ofxImGui::Viewport vp;
//
//   void ofApp::draw() {
//       cam_.begin(); scene_.draw(); cam_.end();
//
//       gui_.begin();
//       vp_.update(cam_);
//       if (vp_.isActive()) {
//           if (!vp_.isDragging())
//               for (int i=0; i<4; ++i)
//                   if (vp_.isNear(pts_[i])) { vp_.beginDrag(i); break; }
//           if (vp_.isDragging())
//               pts_[vp_.getDragId()] = vp_.getWorldMouse();
//       } else { vp_.endDrag(); }
//       ImGui::Begin("Controls"); /* ... */ ImGui::End();
//       gui_.end();
//
//       // Handles drawn AFTER gui_.end() → appear above ImGui panels
//       cam_.begin();
//       for (auto& p : pts_) { ofFill(); ofDrawCircle(p, 8); }
//       cam_.end();
//   }
// ---------------------------------------------------------------------------

namespace ofxImGui {

class Viewport {
public:
    // --- Setup ---

    // Call once per frame, typically inside the ImGui frame (between
    // gui_.begin() and gui_.end()), before any interaction logic.
    // screenRect: the region that accepts input; defaults to the full window.
    void update(ofRectangle screenRect = {});
    void update(ofCamera& cam, ofRectangle screenRect = {});

    // --- Input state ---

    // True when the mouse is inside the viewport rect and ImGui is not
    // capturing the mouse (no ImGui window is active under the cursor).
    bool isHovered()  const { return hovered_;  }

    // True while the left mouse button is held, having started a press inside
    // this viewport.
    bool isActive()   const { return active_;   }

    // True on the single frame the left mouse button is first pressed inside
    // this viewport.
    bool isClicked()  const { return clicked_;  }

    // True on the single frame the left mouse button is released (if the
    // press started inside this viewport).
    bool isReleased() const { return released_; }

    // --- Mouse position ---

    // Mouse position in screen pixels.
    glm::vec2 getScreenMouse()     const { return screenMouse_;    }

    // Mouse position in world coordinates (via camera, or screen coords if no
    // camera was passed to update()).
    glm::vec2 getWorldMouse()      const { return worldMouse_;     }

    // World-space mouse delta since last frame.
    glm::vec2 getWorldMouseDelta() const { return worldMouse_ - prevWorldMouse_; }

    // --- Hit testing ---

    // Returns true when the screen-projected distance from worldPt to the
    // mouse is less than screenRadius pixels.  Zoom-invariant: handles remain
    // pickable at a consistent visual size regardless of camera zoom.
    bool isNear(const glm::vec2& worldPt, float screenRadius = 8.f) const;
    bool isNear(const glm::vec3& worldPt, float screenRadius = 8.f) const;

    // --- Coordinate conversion ---

    // worldPt → screen pixels.  Without a camera this is the identity.
    glm::vec2 worldToScreen(const glm::vec2& worldPt) const;
    glm::vec2 worldToScreen(const glm::vec3& worldPt) const;

    // Screen pixels → world coords.  Without a camera this is the identity.
    glm::vec2 screenToWorld(const glm::vec2& screenPt) const;

    // --- Drag tracking ---

    // Associate a handle index with the current drag gesture.
    void beginDrag(int handleId);
    void endDrag();
    bool isDragging() const { return dragId_ >= 0;  }
    int  getDragId()  const { return dragId_;        }

    // --- Rect access ---
    ofRectangle getScreenRect() const { return rect_; }

private:
    void resolveRect(ofRectangle requested);
    void resolveMouseState();

    ofCamera*   cam_             = nullptr;
    ofRectangle rect_;
    glm::vec2   screenMouse_     = {};
    glm::vec2   worldMouse_      = {};
    glm::vec2   prevWorldMouse_  = {};
    bool        hovered_         = false;
    bool        active_          = false;
    bool        clicked_         = false;
    bool        released_        = false;
    bool        wasActive_       = false;  // active state last frame
    int         dragId_          = -1;
};

} // namespace ofxImGui

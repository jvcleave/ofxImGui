#include "ImCanvas.h"
#include "ofColor.h"
#include "ofRectangle.h"
#include "ofPath.h"

namespace ofxImGui {

// ---------------------------------------------------------------------------
// DrawList
// ---------------------------------------------------------------------------

DrawList::DrawList(ImDrawList* dl, ImVec2 origin)
    : dl_(dl), origin_(origin) {}

void DrawList::setColor(const ofColor& c) {
    color_ = IM_COL32(c.r, c.g, c.b, c.a);
}

void DrawList::setColor(const ofFloatColor& c) {
    color_ = IM_COL32(
        static_cast<int>(c.r * 255.f),
        static_cast<int>(c.g * 255.f),
        static_cast<int>(c.b * 255.f),
        static_cast<int>(c.a * 255.f)
    );
}

void DrawList::setColor(int r, int g, int b, int a) {
    color_ = IM_COL32(r, g, b, a);
}

void DrawList::setFill(bool fill)      { fill_      = fill; }
void DrawList::setLineWidth(float w)   { lineWidth_ = w;    }

ImVec2 DrawList::toScreen(float x, float y) const {
    return { origin_.x + x, origin_.y + y };
}
ImVec2 DrawList::toScreen(const glm::vec2& p) const {
    return { origin_.x + p.x, origin_.y + p.y };
}

void DrawList::drawRect(float x, float y, float w, float h) {
    if (!dl_) return;
    ImVec2 a = toScreen(x,     y    );
    ImVec2 b = toScreen(x + w, y + h);
    if (fill_) dl_->AddRectFilled(a, b, col());
    else       dl_->AddRect(a, b, col(), 0.f, 0, lineWidth_);
}

void DrawList::drawRect(const ofRectangle& r) {
    drawRect(r.x, r.y, r.width, r.height);
}

void DrawList::drawRectRounded(float x, float y, float w, float h, float radius) {
    if (!dl_) return;
    ImVec2 a = toScreen(x,     y    );
    ImVec2 b = toScreen(x + w, y + h);
    if (fill_) dl_->AddRectFilled(a, b, col(), radius);
    else       dl_->AddRect(a, b, col(), radius, 0, lineWidth_);
}

void DrawList::drawCircle(float x, float y, float radius) {
    if (!dl_) return;
    ImVec2 c = toScreen(x, y);
    if (fill_) dl_->AddCircleFilled(c, radius, col());
    else       dl_->AddCircle(c, radius, col(), 0, lineWidth_);
}

void DrawList::drawEllipse(float x, float y, float rx, float ry) {
    if (!dl_) return;
    ImVec2 c = toScreen(x, y);
    ImVec2 r = { rx, ry };
    if (fill_) dl_->AddEllipseFilled(c, r, col());
    else       dl_->AddEllipse(c, r, col(), 0.f, 0, lineWidth_);
}

void DrawList::drawLine(float x1, float y1, float x2, float y2) {
    if (!dl_) return;
    dl_->AddLine(toScreen(x1, y1), toScreen(x2, y2), col(), lineWidth_);
}

void DrawList::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    if (!dl_) return;
    if (fill_)
        dl_->AddTriangleFilled(toScreen(x1, y1), toScreen(x2, y2), toScreen(x3, y3), col());
    else
        dl_->AddTriangle(toScreen(x1, y1), toScreen(x2, y2), toScreen(x3, y3), col(), lineWidth_);
}

void DrawList::drawBezier(float x0, float y0, float x1, float y1,
                           float x2, float y2, float x3, float y3) {
    if (!dl_) return;
    // Fills are not meaningful for open curves — always stroke.
    dl_->AddBezierCubic(
        toScreen(x0, y0), toScreen(x1, y1),
        toScreen(x2, y2), toScreen(x3, y3),
        col(), lineWidth_
    );
}

void DrawList::beginShape() {
    path_.clear();
}

void DrawList::vertex(float x, float y) {
    path_.push_back(toScreen(x, y));
}

void DrawList::vertex(const glm::vec2& p) {
    path_.push_back(toScreen(p));
}

void DrawList::drawPath(const ofPath& p) {
    drawPath(p, {0.f, 0.f}, 1.0f);
}

void DrawList::drawPath(const ofPath& p, glm::vec2 translate, float scale) {
    if (!dl_) return;
    const auto& cmds = p.getCommands();
    if (cmds.empty()) return;

    // Map a path-space point to screen space via the provided transform then
    // the DrawList origin offset.
    auto xf = [&](float x, float y) -> ImVec2 {
        return toScreen(translate.x + x * scale, translate.y + y * scale);
    };

    // Flush the accumulated ImDrawList path as either a fill or a stroke,
    // then reset the sub-path tracking state.
    bool subPathOpen   = false;
    bool subPathClosed = false;

    auto flush = [&]() {
        if (!subPathOpen) return;
        if (fill_)
            dl_->PathFillConvex(col());
        else
            dl_->PathStroke(col(),
                            subPathClosed ? ImDrawFlags_Closed : ImDrawFlags_None,
                            lineWidth_);
        subPathOpen   = false;
        subPathClosed = false;
    };

    for (const auto& cmd : cmds) {
        switch (cmd.type) {

        case ofPath::Command::moveTo:
            flush();
            dl_->PathLineTo(xf(cmd.to.x, cmd.to.y));
            subPathOpen = true;
            break;

        case ofPath::Command::lineTo:
            dl_->PathLineTo(xf(cmd.to.x, cmd.to.y));
            break;

        case ofPath::Command::bezierTo:
            dl_->PathBezierCubicCurveTo(
                xf(cmd.cp1.x, cmd.cp1.y),
                xf(cmd.cp2.x, cmd.cp2.y),
                xf(cmd.to.x,  cmd.to.y));
            break;

        case ofPath::Command::quadBezierTo: {
            // OF convention: cp1 = start (= prevTo), cp2 = control, to = end.
            // Elevate quadratic → cubic: cp1_c = start + 2/3*(ctrl-start)
            //                            cp2_c = end   + 2/3*(ctrl-end)
            const glm::vec2 s  { cmd.cp1.x, cmd.cp1.y };
            const glm::vec2 ct { cmd.cp2.x, cmd.cp2.y };
            const glm::vec2 e  { cmd.to.x,  cmd.to.y  };
            const glm::vec2 c1 = s  + (2.f / 3.f) * (ct - s);
            const glm::vec2 c2 = e  + (2.f / 3.f) * (ct - e);
            dl_->PathBezierCubicCurveTo(
                xf(c1.x, c1.y), xf(c2.x, c2.y), xf(e.x, e.y));
            break;
        }

        case ofPath::Command::close:
            // The closing lineTo/bezierTo was already emitted explicitly
            // (by syncFromEditor).  ImDrawFlags_Closed adds a zero-length
            // degenerate segment for any path that already ends at its start,
            // and draws the closing segment for raw SVG paths that lack one.
            subPathClosed = true;
            break;

        default:
            break;
        }
    }

    flush();
}

void DrawList::endShape(bool close) {
    if (!dl_ || path_.empty()) return;
    if (fill_)
        dl_->AddConvexPolyFilled(path_.data(), static_cast<int>(path_.size()), col());
    else
        dl_->AddPolyline(path_.data(), static_cast<int>(path_.size()), col(),
                         close ? ImDrawFlags_Closed : ImDrawFlags_None, lineWidth_);
    path_.clear();
}

// ---------------------------------------------------------------------------
// Canvas
// ---------------------------------------------------------------------------

bool Canvas::begin(const char* id, ImVec2 size, bool drawBorder) {
    pos_  = ImGui::GetCursorScreenPos();
    size_ = size;

    ImGui::InvisibleButton(id, size,
        ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

    hovered_ = ImGui::IsItemHovered();
    active_  = ImGui::IsItemActive();

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 clipMax = { pos_.x + size_.x, pos_.y + size_.y };
    drawList->PushClipRect(pos_, clipMax, true);

    if (drawBorder)
        drawList->AddRect(pos_, clipMax, IM_COL32(110, 110, 110, 200), 2.f, 0, 1.f);

    dl_    = DrawList(drawList, pos_);
    begun_ = true;
    return true;
}

void Canvas::end() {
    if (begun_) {
        if (dl_.raw()) dl_.raw()->PopClipRect();
        begun_ = false;
    }
}

glm::vec2 Canvas::getMousePos() const {
    ImVec2 mp = ImGui::GetIO().MousePos;
    return { mp.x - pos_.x, mp.y - pos_.y };
}

} // namespace ofxImGui

#pragma once

#include "ofMain.h"
//#include "ofxImGui.h"
#include "imgui.h"
#include "imgui_internal.h"

// Simple extension to query imgui windows, which requires access to imgui_internals
namespace ImGuiEx {

    struct WindowInformation {
        WindowInformation(char* _name, ImVec2 _pos, ImVec2 _size) : name(_name), x(_pos.x), y(_pos.y), width(_size.x), height(_size.y){

        }
        std::string name;
        int x;
        int y;
        int width;
        int height;
    };

    // Returns a list of active windows
    static std::vector<WindowInformation> getWindowsInformation() {
        ImGuiContext& g = *GImGui;

        std::vector<WindowInformation> ret;

        // Iterate front to back
        for (int i = g.Windows.Size - 1; i >= 0; i--){
            const ImGuiWindow* w = g.Windows[i];

            // ignore inactive windows
            if(!w->WasActive) continue;

            ret.push_back( WindowInformation(w->Name, w->Pos, w->Size) );
        }
        return ret;
    }
}

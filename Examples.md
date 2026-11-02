# Examples

Here's an overview of the bundled examples.

## example-simple

A very **minimal example** of how to use imgui within OpenFrameworks.

- Bootstrapping ofxImGui : ofApp integration.
- GUI render order within ofApp.
- Autodraw vs Manual.
- ofxImGui event listening (afterdraw/beforedraw)

## example-advanced

Shows advanced use of imgui within OF, below are some covered topics :

 - How to create helper functions (a minimal Dear ImGui addon).
 - How to use a Dear ImGui addon.
 - How to relate ImGui and oF coordinate systems.
 - How to use the ImGui draw API.
 - It also lets you view the ImGui metrics window, useful for seeing how ImGui is interfaced with oF, it's current inner state and global inspector for debugging purposes.
 - User-input filtering (distinguish OF events from imgui-consumed-events), manually or with helper.

## example-demo    

Shows the imgui demo window, very useful for **discovering all the available widgets** and their respective ImGui API code to implement them. To be explored together with the ImGui demo's source code.  
An easier alternative is to use an online web-assembly-precompiled ImGui-Demo explorer like [pthom/imgui_manual_online](https://pthom.github.io/imgui_manual_online/).

## example-fonts

Load custom fonts and fontawesome into ofxImGui.

## example-helpers

ofxImGui helper functions, mostly for interfacing with `ofParameter`. Useful for transitioning from ofxGui, or usage with ofParameters.

## example-ios

iOS specific with keyboard input helper.

## example-sharedcontext

When several projects need to submit ofxImGui draw calls (*an addon and your ofApp, for example*), you need to share the ofxImGui context and setup as master/slaves. 
If you can, always prefer to provide ImGui functions from your addon and call them from your ofApp code to build your unique GUI. See [ofxImGui integration within ofxAddons](./Developers.md#ofximgui-integration-within-ofxaddons). 

## example-textures

 - OF specific image loading compared to ImGui.
 - Displaying textures / fbo to the GUI.
 - Image GUI buttons.

## example-multiwindow

How to use one ImGui instance in a **multiwindow ofApp**. GUIs are isolated. Some bugs remain, implementation to be finished.

## example-imguizmo

Shows how to **use an ofxImGui addon**. Requires [ofxImGuizmo](https://github.com/nariakiiwatani/ofxImGuizmo.git).  
For pure ImGui addons (many more), see example-advanced.

## example-conway

Playful example implementing the Conway Way Of Life while demonstratring how to use ofxImGui::Canvas and ofxImGui::viewport.

## example-pixeleditor

Simple custom pixel editor featuring the `ImGuiDrawList` usage with `Canvas` and `Viewport`.


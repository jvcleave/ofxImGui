#pragma once

#include "ofMain.h"
#include "ofApp.h"
#include "ofxImGui.h"
#include "imgui_windows_info.h"
#include "ofConstants.h"

#ifdef OFXIMGUI_DEBUG
#pragma message "Compiling with debug flags. Below you'll find some build information."
#if defined(IMGUI_IMPL_OPENGL_ES2)
#pragma message "IMGUI_IMPL_OPENGL_ES2"
#endif
#if defined(IMGUI_IMPL_OPENGL_ES3)
#pragma message "IMGUI_IMPL_OPENGL_ES3"
#endif
#if defined(TARGET_OPENGLES)
#pragma message "TARGET_OPENGLES"
#endif
#if defined(TARGET_OPENGL) // 404 ?
#pragma message "TARGET_OPENGL"
#endif
#if defined(TARGET_PROGRAMMABLE_GL) // 404, Not set by of ?
#pragma message "TARGET_PROGRAMMABLE_GL"
#endif
#if defined(TARGET_GLFW)
#pragma message "TARGET_GLFW"
#endif
#if defined(TARGET_RASPBERRY_PI)
#pragma message "TARGET_RASPBERRY_PI"
#endif
#if defined(TARGET_RASPBERRY_PI_LEGACY)
#pragma message "TARGET_RASPBERRY_PI_LEGACY"
#endif
#if defined(TARGET_LINUX)
#pragma message "TARGET_LINUX"
#endif
#if defined(TARGET_OSX)
#pragma message "TARGET_OSX"
#endif
#if defined(TARGET_WIN32)
#pragma message "TARGET_WIN32"
#endif
#if defined(TARGET_OF_IOS)
#pragma message "TARGET_OF_IOS"
#endif
#if defined(TARGET_ANDROID)
#pragma message "TARGET_ANDROID"
#endif
#if defined(TARGET_EMSCRIPTEN)
#pragma message "TARGET_EMSCRIPTEN"
#endif
#if defined(OF_TARGET_API_VULKAN)
#pragma message "OF_TARGET_API_VULKAN"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GLEW"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GLAD"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GLAD2"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GL3W"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GLBINDING3"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GLBINDING2"
#endif
#endif // OFXIMGUI_DEBUG

// Comment below to use manual drawing
#define USE_AUTODRAW

// Only on GLFW platforms (TARGET_GLFW_WINDOW is only set on rpi, default on other platforms)
#if !defined( TARGET_RASPBERRY_PI ) || defined( TARGET_GLFW_WINDOW )
	#define GLFW_WINDOWING_SYSTEM_IS_USED
	#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3 // Only GLFW > 3.3 has gamepad/joystick functions
		#define GLFW_HAS_GAMEPAD_SUPPORT
	#endif
#endif

class ofApp : public ofBaseApp {

	public:
        ofApp() {}

		void setup() {
            ImGuiConfigFlags flags = ImGuiConfigFlags_None;
            flags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            flags |= ImGuiConfigFlags_NavEnableKeyboard;
            flags |= ImGuiConfigFlags_ViewportsEnable;

            #ifdef USE_AUTODRAW
            gui.setup(nullptr, true, flags);
            #else
            gui.setup(nullptr, false, flags);
            #endif

#ifndef TARGET_OPENGLES
            ofEnableGLDebugLog();
#endif

            ofLogNotice("Renderer Type") << ofGetCurrentRenderer()->getType().c_str();
            ofLogNotice("Window Context") << ofGetCurrentWindow()->getWindowContext();
            auto window = ofGetCurrentWindow();
            if( dynamic_cast< ofAppBaseWindow* >(window.get()) )
                ofLogNotice("Window is Base");
//            if( dynamic_cast< ofAppGLWindow* >(window.get()) )
//                ofLogNotice("Window is GL");
//            if( dynamic_cast< ofGLESWindowSettings * >(window.get()->renderer()->getWindowContext()) )
//                ofLogNotice("Window is GLES");
//#ifdef TARGET_OPENGLES
            if( auto glfwWin = dynamic_cast< ofAppGLFWWindow* >(window.get()) ){
                ofLogNotice("Window is GLFW");
                auto GLFWsettings = glfwWin->getSettings();
#if defined TARGET_OPENGLES
                if( dynamic_cast< ofGLESWindowSettings* >( &GLFWsettings ) ){
                    ofLogNotice("OpenGL is GLES") << "Version " << GLFWsettings.glesVersion;
                }
#else //if defined TARGET_OPENGL
                if( dynamic_cast< ofGLWindowSettings* >( &GLFWsettings ) ){
                    ofLogNotice("OpenGL is GL") << "Version " << GLFWsettings.glVersionMajor << "." << GLFWsettings.glVersionMinor ;
                }
#endif

#ifdef GLFW_HAS_GAMEPAD_SUPPORT
                // Load custom gamepad mapping
                // GLFW uses the SDL database
                // The default 360 binding doesn't work with 360controller driver, this one does.
                glfwUpdateGamepadMappings("030000005e0400008e02000000000000,X360 Controller (custom),a:b0,b:b1,back:b9,dpdown:b12,dpleft:b13,dpright:b14,dpup:b11,guide:b10,leftshoulder:b4,leftstick:b6,lefttrigger:a2,leftx:a0,lefty:a1,rightshoulder:b5,rightstick:b7,righttrigger:a5,rightx:a3,righty:a4,start:b8,x:b2,y:b3,platform:Mac OS X,");
#endif
            }
//            if( dynamic_cast< ofAppGlutWindow* >(window.get()) )
//                ofLogNotice("Window is Glut");
//            if( dynamic_cast< ofAppEGLWindow* >(window.get()) )
//                ofLogNotice("Window is GLFW");
//            if( dynamic_cast< ofAppNoWindow* >(window.get()) )
//                ofLogNotice("Window is GLFW");
		}

		void draw() {
            // Start drawing to ImGui (newFrame)
			gui.begin();

            // Create a new window
            ImGui::Begin("ofxImGui debug");

            ImGui::TextWrapped("Useful information for debugging how ofxImGui is interfaced on its runtime environment.");

            ImGui::Dummy(ImVec2(200,10));
            ImGui::Separator();

            // Show Compilation information
            if( ImGui::CollapsingHeader("Compilation Info", ImGuiTreeNodeFlags_None) ){
#if defined(IMGUI_IMPL_OPENGL_ES2)
                ImGui::TextUnformatted("IMGUI_IMPL_OPENGL_ES2");
#endif
#if defined(IMGUI_IMPL_OPENGL_ES3)
                ImGui::TextUnformatted("IMGUI_IMPL_OPENGL_ES3");
#endif
#if defined(TARGET_OPENGLES)
                ImGui::TextUnformatted("TARGET_OPENGLES");
#endif
#if defined(TARGET_OPENGL) // 404 ?
                ImGui::TextUnformatted("TARGET_OPENGL");
#endif
#if defined(TARGET_PROGRAMMABLE_GL) // 404, Not set by of ?
                ImGui::TextUnformatted("TARGET_PROGRAMMABLE_GL");
#endif
#if defined(TARGET_GLFW)
                ImGui::TextUnformatted("TARGET_GLFW"); // Not explicitly set by of ?
#endif
#if defined(TARGET_RASPBERRY_PI)
                ImGui::TextUnformatted("TARGET_RASPBERRY_PI");
#endif
#if defined(TARGET_RASPBERRY_PI_LEGACY)
                ImGui::TextUnformatted("TARGET_RASPBERRY_PI_LEGACY");
#endif
#if defined(TARGET_LINUX)
                ImGui::TextUnformatted("TARGET_LINUX");
#endif
#if defined(TARGET_OSX)
                ImGui::TextUnformatted("TARGET_OSX");
#endif
#if defined(TARGET_WIN32)
                ImGui::TextUnformatted("TARGET_WIN32");
#endif
#if defined(TARGET_OF_IOS)
                ImGui::TextUnformatted("TARGET_OF_IOS");
#endif
#if defined(TARGET_ANDROID)
                ImGui::TextUnformatted("TARGET_ANDROID");
#endif
#if defined(TARGET_EMSCRIPTEN)
                ImGui::TextUnformatted("TARGET_EMSCRIPTEN");
#endif
#if defined(OF_TARGET_API_VULKAN)
                ImGui::TextUnformatted("OF_TARGET_API_VULKAN");
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
                ImGui::TextUnformatted("IMGUI_IMPL_OPENGL_LOADER_GLEW");
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
                ImGui::TextUnformatted("IMGUI_IMPL_OPENGL_LOADER_GLAD");
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
                ImGui::TextUnformatted("IMGUI_IMPL_OPENGL_LOADER_GLAD2");
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
                ImGui::TextUnformatted("IMGUI_IMPL_OPENGL_LOADER_GL3W");
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
                ImGui::TextUnformatted("IMGUI_IMPL_OPENGL_LOADER_GLBINDING3");
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
                ImGui::TextUnformatted("IMGUI_IMPL_OPENGL_LOADER_GLBINDING2");
#endif
            }

            // Show Platform info
            {
#if defined(TARGET_LINUX)
    #if defined(TARGET_RASPBERRY_PI)
        #if defined(TARGET_RASPBERRY_PI_LEGACY)
            ImGui::Text("You are on Raspberry Pi using the legacy X11 windows.");
        #else
            ImGui::Text("You are on Raspberry Pi using the new GLFW windows.");
        #endif
    #else
        #if defined(TARGET_OPENGLES)
            ImGui::Text("You are on Linux using OpenGLES.");
        #else
            ImGui::Text("You are on Linux using OpenGL.");
        #endif
    #endif
#elif defined(TARGET_OSX)
            ImGui::Text("You are on OSX, probably using GLFW bindings.");
#elif defined(TARGET_WIN32)
            ImGui::Text("You are on Windows, probably using GLFW bindings.");
#elif defined(TARGET_ANDROID)
            ImGui::Text("You are on Android.");
#elif defined(TARGET_IOS)
            ImGui::Text("You are on iOS.");
#elif defined(TARGET_EMSCRIPTEN)
            ImGui::Text("You are on Emscripten.");
#endif
            }

            // OpenGL info
            if( ImGui::CollapsingHeader("GL Info", ImGuiTreeNodeFlags_DefaultOpen) ){
                char *version = NULL;
                char *vendor = NULL;
                char *renderer = NULL;
                version =  (char*)glGetString(GL_VERSION);
                vendor =   (char*)glGetString(GL_VENDOR);
                renderer = (char*)glGetString(GL_RENDERER);

                ImGui::Text("version =  %s\nvendor =   %s\nrenderer = %s\n", version, vendor, renderer);
            }

            // oF info
            if( ImGui::CollapsingHeader("oF environment", ImGuiTreeNodeFlags_DefaultOpen) ){
                ImGui::Text("OpenFrameworks version : %s", ofGetVersionInfo().c_str() );
                ImGui::Text("Renderer type = %s", ofGetCurrentRenderer()->getType().c_str() );
                ImGui::Text("Renderer = %s", glGetString(GL_VERSION) );
                ImGui::Text("Screen = %i, %i", ofGetScreenWidth(), ofGetScreenHeight());
                auto viewport = ofGetCurrentViewport();
                ImGui::Text("Window = %F, %F", ofGetCurrentWindow()->getWindowSize().x, ofGetCurrentWindow()->getWindowSize().y );
                ImGui::Text("Viewport = [%f, %f] [%f x %f]", viewport.position.x, viewport.position.y, viewport.getWidth(), viewport.getHeight());
            }

            // GLFW info
#ifdef GLFW_WINDOWING_SYSTEM_IS_USED
            if( ofAppGLFWWindow* glfwWin = dynamic_cast< ofAppGLFWWindow* >(ofGetCurrentWindow().get()) ){
                if( ImGui::CollapsingHeader("GLFW Info", ImGuiTreeNodeFlags_DefaultOpen) ){
                    ImGui::Text("Your oF window seems to be a GLFW window.");
                    ImGui::Text("getPixelScreenCoordScale = %i", glfwWin->getPixelScreenCoordScale());
                    ofGLFWWindowSettings GLFWsettings = glfwWin->getSettings();
                    if( auto GLESsettings = dynamic_cast< ofGLESWindowSettings* >( &GLFWsettings ) ){
                        #ifdef TARGET_OPENGLES
                        ImGui::Text("OpenGL ES version = %i", GLESsettings->glesVersion );
                        #else
                        ImGui::Text("OpenGL ES version = (unknown)");
                        #endif
                    }

                    if( dynamic_cast< ofGLWindowSettings* >( &GLFWsettings ) ){
                        #ifndef TARGET_OPENGLES
                        ImGui::Text("OpenGL version = %i.%i", GLFWsettings.glVersionMajor, GLFWsettings.glVersionMinor) ;
                        #else
                        ImGui::Text("OpenGL version = (unknown)");
                        #endif
                    }

                    // Add GLFW versions and check against pre-3.3, 3.3 or 3.4
                    ImGui::Text("GLFW version : %i.%i rev %i", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
                    if( GLFW_VERSION_MAJOR == 3 && GLFW_VERSION_MINOR == 3 ){
                        // of 0.11.0 and of 0.11.3+
                        if( OF_VERSION_MAJOR==0 && OF_VERSION_MINOR==11 ){
                            // See https://github.com/openframeworks/apothecary/commit/68a0ec866341a8487d5c555311f3d5975bd62436
                            // See https://github.com/openframeworks/apothecary/pull/197
                            if( OF_VERSION_PATCH <=0 || OF_VERSION_PATCH >=3 ){
                                ImGui::TextColored(ImVec4(255,0,0,255), "Warning !");
                                ImGui::TextWrapped("Your oF version shipped with a pre-3.3 GLFW version, missing some optional imgui features, mostly related to cursors.");
                                if( ImGui::IsItemHovered() ){
                                    ImGui::BeginTooltip();
                                    ImGui::Dummy(ImVec2(400, 1));
                                    ImGui::TextWrapped("You can manually update to GLFW 3.4 to get the most out of imgui.\n(imgui will be more aware of where its windows are on screen).\n Do not proceed on BigSur.");
                                    ImGui::EndTooltip();
                                }
                            }
                        }
                        // of 0.11.1 and 0.11.2
                        else {
                            ImGui::TextWrapped("GLFW 3.3 misses some API calls to work with window positions in viewports and with multiple monitors.");
                            if( ImGui::IsItemHovered() ){
                                ImGui::BeginTooltip();
                                ImGui::Dummy(ImVec2(400, 1));
                                ImGui::TextWrapped("You can update to GLFW 3.4 to have better viewports handling.\n(imgui will be more aware of where its windows are on screen). Do not proceed on BigSur.");
                                ImGui::EndTooltip();
                            }
                        }
                    }
                    else if( GLFW_VERSION_MAJOR == 3 && GLFW_VERSION_MINOR >= 4 ){
                        //ImGui::BeginTooltip();
                        ImGui::TextWrapped("GLFW is up to date, you are using most out of it with imgui.");
                        //ImGui::EndTooltip();
                    }
                    else {
                        ImGui::TextWrapped("No diagnostic to show for your GLFW version.");
                    }
                }
            }
#else
            if( ImGui::CollapsingHeader("GLFW Info", ImGuiTreeNodeFlags_DefaultOpen) ){
                ImGui::TextWrapped("Your windowing system is something else then GLFW...");
            }
#endif

            // ImGui environment
            if( ImGui::CollapsingHeader("DearImGui environment", ImGuiTreeNodeFlags_DefaultOpen) ){
                ImGui::TreePush();
                ImGui::Text("Dear ImGui version : %s", IMGUI_VERSION);

                // ImGui Backend flags (from imgui demo)
                ImGuiBackendFlags backend_flags = ImGui::GetIO().BackendFlags; // Make a local copy to avoid modifying actual back-end flags.
                ImGui::CheckboxFlags("io.BackendFlags: HasGamepad",             (unsigned int*)&backend_flags, ImGuiBackendFlags_HasGamepad);
                ImGui::CheckboxFlags("io.BackendFlags: HasMouseCursors",        (unsigned int*)&backend_flags, ImGuiBackendFlags_HasMouseCursors);
                ImGui::CheckboxFlags("io.BackendFlags: HasSetMousePos",         (unsigned int*)&backend_flags, ImGuiBackendFlags_HasSetMousePos);
                ImGui::CheckboxFlags("io.BackendFlags: PlatformHasViewports",   (unsigned int*)&backend_flags, ImGuiBackendFlags_PlatformHasViewports);
                ImGui::CheckboxFlags("io.BackendFlags: HasMouseHoveredViewport",(unsigned int*)&backend_flags, ImGuiBackendFlags_HasMouseHoveredViewport);
                ImGui::CheckboxFlags("io.BackendFlags: RendererHasVtxOffset",   (unsigned int*)&backend_flags, ImGuiBackendFlags_RendererHasVtxOffset);
                ImGui::CheckboxFlags("io.BackendFlags: RendererHasViewports",   (unsigned int*)&backend_flags, ImGuiBackendFlags_RendererHasViewports);

                ImGui::Separator();

                // DemoWindow
                static bool showDemo = false;
                ImGui::Checkbox("Show demo window", &showDemo);
                if( showDemo ){
                    ImGui::ShowDemoWindow();
                }
                static bool showMetrics = false;
                ImGui::Checkbox("Show metrics window", &showMetrics);
                if( showMetrics ){
                    ImGui::ShowMetricsWindow();
                }

                // Gamepad support
                if( ImGui::CollapsingHeader("Gamepad Input", ImGuiTreeNodeFlags_None) ){
                    ImGui::TreePush();

                    auto io = ImGui::GetIO();
                    auto drawlist = ImGui::GetWindowDrawList();
                    if(drawlist){
                        float availWidth = ImGui::GetContentRegionAvailWidth();
                        ImVec2 gamepadZonePos = ImGui::GetCursorScreenPos() + ImVec2(availWidth*.05f, 10);
                        ImVec2 gamepadZoneSize = { availWidth*.4f, 50 };
                        gamepadZoneSize.y = { gamepadZoneSize.x * .66f };

                        // Bumpers (LB + RB)
                        ImVec2 triggersPos = gamepadZonePos+ImVec2(.5f,0.0f)*gamepadZoneSize;
                        drawlist->AddTriangleFilled( triggersPos + ImVec2(-0.35f, 0.1f)*gamepadZoneSize, triggersPos + ImVec2(-0.33f, 0.f)*gamepadZoneSize, triggersPos + ImVec2(-0.09f, 0.02f)*gamepadZoneSize, ImGui::GetColorU32(io.NavInputs[ImGuiNavInput_FocusPrev]>0?ImGuiCol_ScrollbarGrabActive:ImGuiCol_FrameBg));
                        drawlist->AddTriangleFilled( triggersPos + ImVec2(+0.35f, 0.1f)*gamepadZoneSize, triggersPos + ImVec2(+0.33f, 0.f)*gamepadZoneSize, triggersPos + ImVec2(+0.09f, 0.02f)*gamepadZoneSize, ImGui::GetColorU32(io.NavInputs[ImGuiNavInput_FocusNext]>0?ImGuiCol_ScrollbarGrabActive:ImGuiCol_FrameBg));

                        ImVec2 gamepadPoints[12] = {
                            gamepadZonePos+ImVec2(.5f,0.f)*gamepadZoneSize,
                            gamepadZonePos+ImVec2(.9f,.1f)*gamepadZoneSize,
                            gamepadZonePos+ImVec2(1.f,.6f)*gamepadZoneSize,
                            gamepadZonePos+ImVec2(.9f,.9f)*gamepadZoneSize,
                            gamepadZonePos+ImVec2(.8f,.9f)*gamepadZoneSize,
                            gamepadZonePos+ImVec2(.7f,.7f)*gamepadZoneSize,
                            gamepadZonePos+ImVec2(.5f,.6f)*gamepadZoneSize,
                            gamepadZonePos+ImVec2(.3f,.7f)*gamepadZoneSize,
                            gamepadZonePos+ImVec2(.2f,.9f)*gamepadZoneSize,
                            gamepadZonePos+ImVec2(.1f,.9f)*gamepadZoneSize,
                            gamepadZonePos+ImVec2(0.f,.6f)*gamepadZoneSize,
                            gamepadZonePos+ImVec2(.1f,.1f)*gamepadZoneSize,
                        };
                        drawlist->AddConvexPolyFilled(&gamepadPoints[0], 12, ImGui::GetColorU32(ImGuiCol_FrameBg));
                        drawlist->AddPolyline(&gamepadPoints[0], 12, ImGui::GetColorU32(ImGuiCol_Text), true, 0.5f);

                        // Move Pad
                        ImVec2 padPos = gamepadZonePos+ImVec2(.3f,0.5f)*gamepadZoneSize;
                        float padSize = gamepadZoneSize.x*.05f;
                        drawlist->AddCircleFilled(padPos, padSize, ImGui::GetColorU32(ImGuiCol_Text));
                        if(io.NavInputs[ImGuiNavInput_DpadUp]>0)    drawlist->AddCircleFilled(padPos+padSize*.6f*ImVec2(0,-1.f), padSize, ImGui::GetColorU32(ImGuiCol_ScrollbarGrabActive));
                        if(io.NavInputs[ImGuiNavInput_DpadDown]>0)  drawlist->AddCircleFilled(padPos+padSize*.6f*ImVec2(0,+1.f), padSize, ImGui::GetColorU32(ImGuiCol_ScrollbarGrabActive));
                        if(io.NavInputs[ImGuiNavInput_DpadLeft]>0)  drawlist->AddCircleFilled(padPos+padSize*.6f*ImVec2(-1.f,0), padSize, ImGui::GetColorU32(ImGuiCol_ScrollbarGrabActive));
                        if(io.NavInputs[ImGuiNavInput_DpadRight]>0) drawlist->AddCircleFilled(padPos+padSize*.6f*ImVec2(+1.f,0), padSize, ImGui::GetColorU32(ImGuiCol_ScrollbarGrabActive));

                        // Left Joystick
                        ImVec2 leftStickPos = gamepadZonePos+ImVec2(.2f,0.3f)*gamepadZoneSize;
                        float leftStickSize = gamepadZoneSize.x*.05f;
                        drawlist->AddCircleFilled(leftStickPos, leftStickSize, ImGui::GetColorU32(ImGuiCol_Text));
                        ImVec2 leftStickDir = ImVec2(io.NavInputs[ImGuiNavInput_LStickRight]-io.NavInputs[ImGuiNavInput_LStickLeft],io.NavInputs[ImGuiNavInput_LStickDown]-io.NavInputs[ImGuiNavInput_LStickUp]);
                        if( leftStickDir.x != 0 || leftStickDir.y != 0 )
                            drawlist->AddCircleFilled(leftStickPos+(leftStickSize*leftStickDir*.6f), leftStickSize, ImGui::GetColorU32(ImGuiCol_ScrollbarGrabActive));

                        // Modifier Buttons
                        ImVec2 buttonsPos = gamepadZonePos+ImVec2(.75f,0.35f)*gamepadZoneSize;
                        float buttonsSize = gamepadZoneSize.x*.04f;
                        //drawlist->AddCircleFilled(buttonsPos, buttonsSize, ImGui::GetColorU32(ImGuiCol_Text));
                        drawlist->AddCircleFilled(buttonsPos+buttonsSize*2.f*ImVec2(0,+1.f), buttonsSize, ImGui::GetColorU32(io.NavInputs[ImGuiNavInput_Activate]>0?ImGuiCol_ScrollbarGrabActive:ImGuiCol_Text));
                        drawlist->AddCircleFilled(buttonsPos+buttonsSize*2.f*ImVec2(-1.f,0), buttonsSize, ImGui::GetColorU32(io.NavInputs[ImGuiNavInput_Menu]>0?ImGuiCol_ScrollbarGrabActive:ImGuiCol_Text));
                        drawlist->AddCircleFilled(buttonsPos+buttonsSize*2.f*ImVec2(0,-1.f), buttonsSize, ImGui::GetColorU32(io.NavInputs[ImGuiNavInput_Input]>0?ImGuiCol_ScrollbarGrabActive:ImGuiCol_Text));
                        drawlist->AddCircleFilled(buttonsPos+buttonsSize*2.f*ImVec2(+1.f,0), buttonsSize, ImGui::GetColorU32(io.NavInputs[ImGuiNavInput_Cancel]>0?ImGuiCol_ScrollbarGrabActive:ImGuiCol_Text));

                        // Vertical spacer
                        ImGui::Dummy(ImVec2(availWidth, 10));

                        // Dummy above gamepad
                        ImGui::Dummy(gamepadZoneSize+ImVec2(availWidth*.1f,0));

#ifdef GLFW_WINDOWING_SYSTEM_IS_USED
                        // Count joysticks
                        int detectedJoysticks = 0;
                        for(int joyNum = GLFW_JOYSTICK_1; joyNum < GLFW_JOYSTICK_LAST; joyNum++){
                            if( glfwJoystickPresent(joyNum) ) detectedJoysticks++;
                        }

                        ImGui::SameLine();
                        ImGui::BeginGroup();

                        ImGui::Text("GLFW has detected %i joysticks (total).", detectedJoysticks);
                        ImGui::Text("DearImGui uses GLFW_JOYSTICK_1 as input.");

                        ImGui::Dummy(ImVec2(0,6));
    #ifdef GLFW_HAS_GAMEPAD_SUPPORT
                        if(glfwJoystickIsGamepad(GLFW_JOYSTICK_1)){
                            ImGui::Text("Gamepad name : %s", glfwGetGamepadName(GLFW_JOYSTICK_1));
                            ImGui::Text("Gamepad GUID : %s", glfwGetJoystickGUID(GLFW_JOYSTICK_1));
                        }
                        else if( glfwJoystickPresent(GLFW_JOYSTICK_1) ){
                            ImGui::Text("The device mapped to GLFW_JOYSTICK_1 has no GLFW gamepad mappings.)");
                            ImGui::Text("Joystick GUID : %s", glfwGetJoystickGUID(GLFW_JOYSTICK_1));
                        }
                        else {
                            ImGui::Text("No joystick detected by GLFW as GLFW_JOYSTICK_1.");
                        }
    #else
                        ImGui::TextWrapped("Your GLFW version does not support gamepads or joysticks.");
    #endif

                        ImGui::EndGroup();
#else
                        ImGui::TextWrapped("GLFW joystick information not available in non-glfw windowing modes.");
#endif
                        //ImGui::Dummy(ImVec2(availWidth, gamepadZoneSize.y));
                        //ImGui::Button("test", gamepadZoneSize);

                        if( ImGui::CollapsingHeader("Dear ImGui Navigation Input") ){
                            ImGui::TreePush();//Node("navinput");
                            ImGui::Columns(2);

                            ImGui::SliderFloat("Up", &io.NavInputs[ImGuiNavInput_LStickUp], 0.f, 1.f);
                            ImGui::SliderFloat("Right", &io.NavInputs[ImGuiNavInput_LStickRight], 0.f, 1.f);
                            ImGui::SliderFloat("Down", &io.NavInputs[ImGuiNavInput_LStickDown], 0.f, 1.f);
                            ImGui::SliderFloat("Left", &io.NavInputs[ImGuiNavInput_LStickLeft], 0.f, 1.f);
//                            ImGui::SliderFloat("LT", &io.NavInputs[ImGuiNavInput_TweakSlow], 0.f, 1.f);
//                            ImGui::SliderFloat("RT", &io.NavInputs[ImGuiNavInput_TweakFast], 0.f, 1.f);
//                            ImGui::SliderFloat("LB", &io.NavInputs[ImGuiNavInput_FocusPrev], 0.f, 1.f);
//                            ImGui::SliderFloat("RB", &io.NavInputs[ImGuiNavInput_FocusNext], 0.f, 1.f);

                            ImGui::NextColumn();

#define printBTN(NAME, IMGUI_NAVINPUT) { if(io.NavInputs[IMGUI_NAVINPUT]>0){ ImGui::Text(NAME); } else { ImGui::TextDisabled(NAME); } }
                            printBTN("Activate",    ImGuiNavInput_Activate);
                            printBTN("Cancel",      ImGuiNavInput_Cancel);
                            printBTN("Menu",        ImGuiNavInput_Menu);
                            printBTN("Input",       ImGuiNavInput_Input);
                            printBTN("DpadLeft",    ImGuiNavInput_DpadLeft);
                            printBTN("DpadRight",   ImGuiNavInput_DpadRight);
                            printBTN("DpadUp",      ImGuiNavInput_DpadUp);
                            printBTN("DpadDown",    ImGuiNavInput_DpadDown);
                            printBTN("FocusPrev",   ImGuiNavInput_FocusPrev);
                            printBTN("FocusNext",   ImGuiNavInput_FocusNext);
                            printBTN("TweakSlow",   ImGuiNavInput_TweakSlow);
                            printBTN("TweakFast",   ImGuiNavInput_TweakFast);
#undef printBTN
                            ImGui::Columns(1);
                            ImGui::TreePop();
                        }

                        if( ImGui::CollapsingHeader("GLFW Joysticks Input") ){
#if defined( GLFW_WINDOWING_SYSTEM_IS_USED )  && defined( GLFW_HAS_GAMEPAD_SUPPORT )
                            ImGui::TextWrapped("This section lets you inspect the GLFW side of your gamepad mappings.");

                            // Glfw Polling
                            static int glwfBtns[15] = {
                                GLFW_GAMEPAD_BUTTON_A,
                                GLFW_GAMEPAD_BUTTON_B,
                                GLFW_GAMEPAD_BUTTON_X,
                                GLFW_GAMEPAD_BUTTON_Y,
                                GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
                                GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
                                GLFW_GAMEPAD_BUTTON_BACK,
                                GLFW_GAMEPAD_BUTTON_START,
                                GLFW_GAMEPAD_BUTTON_GUIDE,
                                GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
                                GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
                                GLFW_GAMEPAD_BUTTON_DPAD_UP,
                                GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
                                GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
                                GLFW_GAMEPAD_BUTTON_DPAD_LEFT
                            };
                            static const char* glwfBtnNames[15] = {
                                "BUTTON_A",            //"GLFW_GAMEPAD_BUTTON_A",
                                "BUTTON_B",            //"GLFW_GAMEPAD_BUTTON_B",
                                "BUTTON_X",            //"GLFW_GAMEPAD_BUTTON_X",
                                "BUTTON_Y",            //"GLFW_GAMEPAD_BUTTON_Y",
                                "BUTTON_LEFT_BUMPER",  //"GLFW_GAMEPAD_BUTTON_LEFT_BUMPER",
                                "BUTTON_RIGHT_BUMPER", //"GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER",
                                "BUTTON_BACK",         //"GLFW_GAMEPAD_BUTTON_BACK",
                                "BUTTON_START",        //"GLFW_GAMEPAD_BUTTON_START",
                                "BUTTON_GUIDE",        //"GLFW_GAMEPAD_BUTTON_GUIDE",
                                "BUTTON_LEFT_THUMB",   //"GLFW_GAMEPAD_BUTTON_LEFT_THUMB",
                                "BUTTON_RIGHT_THUMB",  //"GLFW_GAMEPAD_BUTTON_RIGHT_THUMB",
                                "BUTTON_DPAD_UP",      //"GLFW_GAMEPAD_BUTTON_DPAD_UP",
                                "BUTTON_DPAD_RIGHT",   //"GLFW_GAMEPAD_BUTTON_DPAD_RIGHT",
                                "BUTTON_DPAD_DOWN",    //"GLFW_GAMEPAD_BUTTON_DPAD_DOWN",
                                "BUTTON_DPAD_LEFT"     //"GLFW_GAMEPAD_BUTTON_DPAD_LEFT"
                            };

                            //#define MAP_BUTTON(BUTTON_NO) { if (buttons_count > BUTTON_NO && buttons[BUTTON_NO] == GLFW_PRESS) io.NavInputs[NAV_NO] = 1.0f; }
                            int axes_count = 0, buttons_count = 0;

                            const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
                            const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);

                            ImGui::Columns(2);
                            // List buttons
                            ImGui::Text("Num Buttons: %i", buttons_count);
                            if( buttons ) for(int btn : glwfBtns){
                                // ImGui::Checkbox(glwfBtnNames[btn], btn==GLFW_PRESS);
                                //ImGui::RadioButton(glwfBtnNames[btn], buttons[btn]==GLFW_PRESS );
                                if( buttons[btn]==GLFW_PRESS )
                                    //ImGui::Button(glwfBtnNames[btn]);
                                    ImGui::Text("%s = %i", glwfBtnNames[btn], buttons[btn]);
                                else
                                    ImGui::TextDisabled("%s = %i", glwfBtnNames[btn], buttons[btn]);
                                //continue;
                                //if(buttons[btn]!=GLFW_PRESS) continue;
                                //ImGui::Text("%i : %s = %i", btn, glwfBtnNames[btn], buttons[btn]);
                            }
                            ImGui::NextColumn();
                            // List axes
                            ImGui::Text("Num Axes: %i", axes_count);
                            if( axes ) for(int axe = 0; axe < axes_count; axe++){
                                //if(axes[axe]!=GLFW_PRESS) continue;
                                //ImGui::Text("Axe %i = %f", axe, axes[axe]);
                                float tmp = axes[axe];
                                std::string axeName("Axe ");
                                axeName.append( std::to_string(axe) );
                                ImGui::SliderFloat(axeName.c_str(), &tmp, -1.f, 1.f, "%.1f");
                            }
                            ImGui::Columns(1);
#else
                            ImGui::TextWrapped("Unavailable on your setup.");
#endif
                        }
                    }
                    ImGui::TreePop();
                }

                // Draw how imgui sees system windows
                if( ImGui::CollapsingHeader("Window space", ImGuiTreeNodeFlags_DefaultOpen) ){
                    auto io = ImGui::GetIO();
                    auto drawlist = ImGui::GetWindowDrawList();

                    ImGui::TreePush();

                    // Some info
                    ImGui::Text("%d active imgui windows (%d visible)", io.MetricsActiveWindows, io.MetricsRenderWindows);

                    ImGui::Separator();

                    if(drawlist){
                        // Get coordinates
                        ImVec2 windowsZonePos = ImGui::GetCursorScreenPos();
                        ImVec2 windowsZone = {ImGui::GetContentRegionAvailWidth(), 50};
                        ImVec2 ofScreenSize = ImVec2(ofGetScreenWidth(), ofGetScreenHeight());
                        windowsZone.y = windowsZone.x*(ofScreenSize.y/ofScreenSize.x);// Fit height proportionally to screen
                        ImVec2 drawScale = windowsZone / ofScreenSize;
                        ImVec2 ofWindowPos = ofGetWindowRect().getMin();
                        ImVec2 ofWindowSize( ofGetWindowWidth(), ofGetWindowHeight() );
                        ImVec2 bestWindowPos = ofWindowPos;

                        // Main screen
                        static char mainwindowTitle[100];
                        sprintf(mainwindowTitle, "Main display (%.0f x %.0f)", ofWindowSize.x, ofWindowSize.y);
                        drawlist->AddRect(windowsZonePos, windowsZonePos+windowsZone, IM_COL32(255,0,0,255));
                        drawlist->AddText(windowsZonePos+ImVec2(2,1), IM_COL32(255,0,0,255), mainwindowTitle);
                        //ofGetWindowRect();

                        // of Window
                        static char ofWindowTitle[100];
                        sprintf(ofWindowTitle, "oF window (%.0f x %.0f)", ofWindowSize.x, ofWindowSize.y);
                        drawlist->AddRect(windowsZonePos+ImVec2(ofWindowPos.x, ofWindowPos.y)*drawScale, windowsZonePos+(ImVec2(ofWindowPos.x, ofWindowPos.y)+ofWindowSize)*drawScale, IM_COL32(0,255,0,255));
                        drawlist->AddText(windowsZonePos+ImVec2(ofWindowPos.x, ofWindowPos.y)*drawScale+ImVec2(2,1+ImGui::GetFontSize()), IM_COL32(0,255,0,255), ofWindowTitle );

                        // Glfw window
                        // Try get window position from native GLFW, as the of version doesn't always support this.
                        ofAppBaseWindow* windowPtr = ofGetWindowPtr();
                        if(ofAppGLFWWindow* glfwWindow = dynamic_cast< ofAppGLFWWindow* >(windowPtr) ){
                            int glfwWinX = 0, glfwWinY = 0, glfwH = 0, glfwW = 0;

                            glfwGetWindowPos(glfwWindow->getGLFWWindow(), &glfwWinX, &glfwWinY);
                            glfwGetWindowSize(glfwWindow->getGLFWWindow(), &glfwW, &glfwH);
                            if( bestWindowPos.x==0 && bestWindowPos.y==0 ) bestWindowPos = ImVec2(glfwWinX, glfwWinY);

                            // glfw Window
                            char glfwWindowTitle[100];
                            sprintf(glfwWindowTitle, "glfw window (%.0i x %.0i)", glfwW, glfwH);

                            drawlist->AddRect(windowsZonePos+ImVec2(glfwWinX, glfwWinY)*drawScale, windowsZonePos+(ImVec2(glfwWinX+glfwW, glfwWinY+glfwH))*drawScale, IM_COL32(0,255,0,255));
                            drawlist->AddText(windowsZonePos+ImVec2(glfwWinX, glfwWinY)*drawScale+ImVec2(2,1), IM_COL32(0,255,0,255), glfwWindowTitle );
                        }


                        // ImGui windows
                        auto windows = ImGuiEx::getWindowsInformation();
                        ImVec2 windowOffset = (io.ConfigFlags&ImGuiConfigFlags_ViewportsEnable) ? ImVec2(0,0) : bestWindowPos;
                        for(auto win : windows){
                            drawlist->AddRect(windowsZonePos+(windowOffset+ImVec2(win.x, win.y))*drawScale, windowsZonePos+(windowOffset+ImVec2(win.x, win.y)+ImVec2(win.width,win.height))*drawScale, IM_COL32(0,0,255,255));
                            drawlist->AddText(windowsZonePos+(windowOffset+ImVec2(win.x, win.y))*drawScale+ImVec2(2,1), IM_COL32(0,0,255,255), win.name.c_str() );
                        }

                        // Dummy to take up space
                        ImGui::Dummy(windowsZone);

                    }
                    ImGui::TreePop(); // End windows graphics
                }
                ImGui::TreePop(); // End imgui environment
            }

            // Close the main window
            ImGui::End();

            // The GUI hasn't been rendered yet : we can still draw below it
            ofDrawBitmapStringHighlight( ofToString("Render order test :"), 10, 20);
            ofDrawBitmapStringHighlight( ofToString(" - I'm below the GUI"), 10, 40);

            // End our ImGui Frame.
            // Also Renders if using autoDraw AND non-shared mode
            gui.end();

#ifndef USE_AUTODRAW
            gui.draw(); // <-- In manual mode, you can choose to render imgui at a given moment in your rendering pipeline
#endif

#ifdef USE_AUTODRAW
            ofDrawBitmapStringHighlight( " - In autodraw mode, you don't have rendering-order control...", 10, 60); // <-- This text will be below the gui as ofxImGui renders after your ofApp::draw
#else
            ofDrawBitmapStringHighlight( " - I should be over the Gui ! (autoDraw is off)", 10, 60); // <-- This text will be over the gui
#endif
		}

        void update(){

        }

    private:
        ofxImGui::Gui gui;
};

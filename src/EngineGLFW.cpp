#include "EngineGLFW.h"

#if ( !defined(OF_TARGET_API_VULKAN) && (!defined(TARGET_RASPBERRY_PI) || (defined(TARGET_GLFW_WINDOW) && !defined(TARGET_RASPBERRY_PI_LEGACY)) ))

#include "ofAppGLFWWindow.h"
#include "ofGLProgrammableRenderer.h"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_opengl2.h"



namespace ofxImGui
{
	GLuint EngineGLFW::g_FontTexture = 0;

	//--------------------------------------------------------------
	void EngineGLFW::setup(bool autoDraw)
	{
        if (isSetup){
#ifdef OFXIMGUI_DEBUG
            ofLogNotice("EngineGLFW::setup()") << "Ignoring glfw setup, already bound." << std::endl;
#endif
            return;
        }
#ifdef OFXIMGUI_DEBUG
        else ofLogNotice("EngineGLFW::setup()") << "Setting up GLFW in oF window size " << ImGui::GetIO().DisplaySize << " // Ptr=" << ofGetWindowPtr() << " ["<< ofGetWindowPtr()->getWindowSize() <<"]" << std::endl;

        #if defined(OFXIMGUI_DEBUG) && defined(TARGET_OPENGLES) && defined(TARGET_RASPBERRY_PI)
            // oF 0.11 related warnings
            #if defined(TARGET_RASPBERRY_PI_LEGACY)
                #pragma message "ofxImGui detected that you use the RPI in LEGACY mode. Make sure that you use the Legacy Video driver."
            #else
                #pragma message "You are using the new Rpi GLFW binding. Please ensure that your raspi-config doesn't use the Legacy video drivers."
            #endif
        #endif // rpi gles
        #endif // debug

#ifdef OFXIMGUI_ENABLE_OF_BINDINGS

        #ifdef OFXIMGUI_DEBUG
        ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with oF bingings (OFXIMGUI_ENABLE_OF_BINDINGS)";
        #pragma message "ofxImGui compiling GLFW with wull oF bindings. (ofxImGui Legacy mode)"
        #endif

        ImGuiIO& io = ImGui::GetIO();

        io.DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());

        // We only map the keys that ImGui needs for its UI
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
        // Note: No idea where these come from, they were commented anyways
        //io.KeyMap[ImGuiKey_F] = GLFW_KEY_F;
        //io.KeyMap[ImGuiKey_H] = GLFW_KEY_H;
        //io.KeyMap[ImGuiKey_F1] = GLFW_KEY_F1;
        //io.KeyMap[ImGuiKey_F2] = GLFW_KEY_F2;
        //io.KeyMap[ImGuiKey_F3] = GLFW_KEY_F3;
        //io.KeyMap[ImGuiKey_F4] = GLFW_KEY_F4;
        //io.KeyMap[ImGuiKey_F5] = GLFW_KEY_F5;
        //io.KeyMap[ImGuiKey_F6] = GLFW_KEY_F6;
        //io.KeyMap[ImGuiKey_F7] = GLFW_KEY_F7;
        //io.KeyMap[ImGuiKey_F8] = GLFW_KEY_F8;
        //io.KeyMap[ImGuiKey_F9] = GLFW_KEY_F9;
        //io.KeyMap[ImGuiKey_F10] = GLFW_KEY_F10;
        //io.KeyMap[ImGuiKey_F11] = GLFW_KEY_F11;
        //io.KeyMap[ImGuiKey_F12] = GLFW_KEY_F12;

        // Init renderer
        //if (autoDraw)
        {
            if (ofIsGLProgrammableRenderer())
            {
                io.RenderDrawListsFn = programmableDrawData;
            }
            else
            {
                io.RenderDrawListsFn = fixedDrawData;
            }
        }

        io.SetClipboardTextFn = &BaseEngine::setClipboardString;
        io.GetClipboardTextFn = &BaseEngine::getClipboardString;

        createDeviceObjects();

        // Override listeners
        ofAddListener(ofEvents().mousePressed, this, &EngineGLFW::onMousePressed);
        ofAddListener(ofEvents().mouseReleased, this, &EngineGLFW::onMouseReleased);
        ofAddListener(ofEvents().keyReleased, this, &EngineGLFW::onKeyReleased);
        ofAddListener(ofEvents().keyPressed, this, &EngineGLFW::onKeyPressed);

        // BaseEngine listeners
        ofAddListener(ofEvents().mouseDragged, (BaseEngine*)this, &BaseEngine::onMouseDragged);
        ofAddListener(ofEvents().mouseScrolled, (BaseEngine*)this, &BaseEngine::onMouseScrolled);
        ofAddListener(ofEvents().windowResized, (BaseEngine*)this, &BaseEngine::onWindowResized);

#else // Use regular imgui bindings

    #ifdef OFXIMGUI_DEBUG
        #pragma message "ofxImGui compiling with the new native imgui backend."
        ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with native ImGui GLFW backend";
    #endif

        // Init window
        GLFWwindow* curWin = (GLFWwindow*)ofGetWindowPtr()->getWindowContext();
        ImGui_ImplGlfw_InitForOpenGL( curWin, true );

        // Seems unnecessary (only needed for popped-out windows?)
        //glfwSetWindowPosCallback(   curWin, ImGui_ImplGlfw_WindowPosCallback);
        //glfwSetWindowSizeCallback(  curWin, ImGui_ImplGlfw_WindowSizeCallback);

        // Init renderer
        if( ofIsGLProgrammableRenderer() ){
            int minor; int major;
            //glfwGetVersion(&minor, &major, nullptr); // Old way, seems to return maximal version with GLFW 3.4
            major = ofGetGLRenderer()->getGLVersionMajor();
            minor = ofGetGLRenderer()->getGLVersionMinor();

            // See imgui_impl_opengl3.cpp
            //----------------------------------------
            // OpenGL    GLSL      GLSL
            // version   version   string
            //----------------------------------------
            //  2.0       110       "#version 110"
            //  2.1       120       "#version 120"
            //  3.0       130       "#version 130"
            //  3.1       140       "#version 140"
            //  3.2       150       "#version 150"
            //  3.3       330       "#version 330 core"
            //  4.0       400       "#version 400 core"
            //  4.1       410       "#version 410 core"
            //  4.2       420       "#version 410 core"
            //  4.3       430       "#version 430 core"
            //  ES 2.0    100       "#version 100"      = WebGL 1.0
            //  ES 3.0    300       "#version 300 es"   = WebGL 2.0
            //----------------------------------------

            // default version empty --> use imgui backend's default
            const char* glsl_version = NULL;

            // Override imgui versions to fit oF versions
#ifdef TARGET_OPENGLES
            if( major==2 )
                glsl_version = "#version 100";
            if( major==3 ) // Note: not yet available in oF !!!
                glsl_version = "#version 300 es";
#else
            if( major==3 ){
                if( minor==0 )      glsl_version="#version 130";
                else if( minor==1 ) glsl_version="#version 140";
                else if( minor==2 ) glsl_version="#version 150";
                else if( minor==3 ) glsl_version="#version 330 core";
            }
            else if( major==4 ){
                if( minor==0 )      glsl_version="#version 400 core";
                else if( minor==1 ) glsl_version="#version 410 core";
                else if( minor==2 ) glsl_version="#version 420 core";
                else if( minor==3 ) glsl_version="#version 430 core";
            }
#endif

#ifdef OFXIMGUI_DEBUG
    #ifdef TARGET_OPENGLES
            ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with programmable OpenGL ES " << major << "." << minor << " and version string «" << glsl_version << "»";
    #else
            ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with programmable OpenGL " << major << "." << minor << " and version string «" << glsl_version << "»";
    #endif
#endif

            ImGui_ImplOpenGL3_Init(glsl_version); // Called by the function below, but needed with these arguments
            ImGui_ImplOpenGL3_CreateDeviceObjects();
            //ImGui_ImplOpenGL3_CreateFontsTexture(); // called by the above function
        }
        else
        {
#ifdef OFXIMGUI_DEBUG
    #ifdef TARGET_OPENGLES
            ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with OpenGL ES and ofIsGLProgrammableRenderer()=" << (ofIsGLProgrammableRenderer()?"1":"0");
    #else
            ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with OpenGL2 and ofIsGLProgrammableRenderer()=" << (ofIsGLProgrammableRenderer()?"1":"0");
    #endif
#endif
            ImGui_ImplOpenGL2_Init();
            ImGui_ImplOpenGL2_CreateDeviceObjects();
        }

        // These are currently called by the imgui backend.
        //GLFWwindow* curWindow=(GLFWwindow*)ofGetWindowPtr()->getWindowContext();
        //glfwSetMouseButtonCallback( curWindow, ImGui_ImplGlfw_MouseButtonCallback);
        //glfwSetScrollCallback(      curWindow, ImGui_ImplGlfw_ScrollCallback);
        //glfwSetKeyCallback(         curWindow, ImGui_ImplGlfw_KeyCallback);
        //glfwSetCharCallback(        curWindow, ImGui_ImplGlfw_CharCallback);
        //glfwSetWindowCloseCallback( curWindow, ImGui_ImplGlfw_WindowCloseCallback);
        //glfwSetWindowPosCallback(   curWindow, ImGui_ImplGlfw_WindowPosCallback);
        //glfwSetWindowSizeCallback(  curWindow, ImGui_ImplGlfw_WindowSizeCallback);

#endif // IMGUI BACKEND binding

		isSetup = true;
	}

    
    
	//--------------------------------------------------------------
	void EngineGLFW::exit()
	{
		if (!isSetup) return;
        
#ifdef OFXIMGUI_ENABLE_OF_BINDINGS
        // Unregister listeners
        ofRemoveListener(ofEvents().mousePressed, this, &EngineGLFW::onMousePressed);
        ofRemoveListener(ofEvents().mouseReleased, this, &EngineGLFW::onMouseReleased);
        ofRemoveListener(ofEvents().keyReleased, this, &EngineGLFW::onKeyReleased);
        ofRemoveListener(ofEvents().keyPressed, this, &EngineGLFW::onKeyPressed);
        
        ofRemoveListener(ofEvents().mouseDragged, (BaseEngine*)this, &BaseEngine::onMouseDragged);
        ofRemoveListener(ofEvents().mouseScrolled, (BaseEngine*)this, &BaseEngine::onMouseScrolled);
        ofRemoveListener(ofEvents().windowResized, (BaseEngine*)this, &BaseEngine::onWindowResized);

        invalidateDeviceObjects();
 #else
        if (ofIsGLProgrammableRenderer()){
            //ImGui_ImplOpenGL3_DestroyFontsTexture(); // called by function below
            //ImGui_ImplOpenGL3_DestroyDeviceObjects(); // Called below
            ImGui_ImplOpenGL3_Shutdown(); // called by ImGuiDestroyContext() later ?
        }
        else {
            //ImGui_ImplOpenGL2_DestroyFontsTexture(); // called by function below
            //ImGui_ImplOpenGL2_DestroyDeviceObjects(); // Called below
            ImGui_ImplOpenGL2_Shutdown();
        }
        ImGui_ImplGlfw_Shutdown();
#endif

		isSetup = false;
	}

    //--------------------------------------------------------------
    void EngineGLFW::newFrame()
    {
#ifdef OFXIMGUI_ENABLE_OF_BINDINGS

        // New: better set display every frame instead of waiting for the resize() callback. (smoother)
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());
#else
        if (ofIsGLProgrammableRenderer()){
            ImGui_ImplOpenGL3_NewFrame();
        }
        else{
            ImGui_ImplOpenGL2_NewFrame();
        }
        ImGui_ImplGlfw_NewFrame();
#endif
    }

    //--------------------------------------------------------------
    void EngineGLFW::endFrame()
    {
#ifdef OFXIMGUI_MULTIWINDOW_IMPL
        //ImGuiIO& io = ImGui::GetIO();
        //if(  )
        //ImGui::Render();

//        if (ofIsGLProgrammableRenderer()){
//            //std::cout << "isProgramable!" << std::endl;
//            //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//            programmableDrawData(ImGui::GetDrawData());
//        }
//        else{
//            //std::cout << "isNotProgramable!" << std::endl;
//            //ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
//            fixedDrawData(ImGui::GetDrawData());
//        }
        if(!autoDraw){
            this->render();
        }
#endif
    }

	//--------------------------------------------------------------
    void EngineGLFW::render()
	{

#ifdef OFXIMGUI_ENABLE_OF_BINDINGS
        //ImGui::Render(); // also calls ImGui::EndFrame()

        if (ofIsGLProgrammableRenderer()) {
//            int display_w, display_h;
//            glfwGetFramebufferSize(window, &display_w, &display_h);
//            glViewport(0, 0, display_w, display_h);
//            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
//            glClear(GL_COLOR_BUFFER_BIT);
            programmableDrawData(ImGui::GetDrawData());
        }
        else {
            fixedDrawData(ImGui::GetDrawData());
        }
//        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//        {
//            //           GLFWwindow* backup_current_context = glfwGetCurrentContext();
//            ImGui::UpdatePlatformWindows();
//            ImGui::RenderPlatformWindowsDefault();
//            //           glfwMakeContextCurrent(backup_current_context);
//        }
#else

        if (ofIsGLProgrammableRenderer()) {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        else {
            // GLint last_program;
            // glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
            // glUseProgram(0);
            //There are potentially many more states you could need to clear/setup that we can't access from default headers.
            //e.g. glBindBuffer(GL_ARRAY_BUFFER, 0), glDisable(GL_TEXTURE_CUBE_MAP).
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
            //glUseProgram(last_program);
        }

        // Handle multi-viewports
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
            GLFWwindow* backup_current_context = glfwGetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            // Restore context so we can continue to render with oF
            glfwMakeContextCurrent(backup_current_context);
        }
#endif
	}

#ifdef OFXIMGUI_ENABLE_OF_BINDINGS
    //--------------------------------------------------------------
    bool EngineGLFW::createFontsTexture()
    {
        // Build texture atlas
        ImGuiIO& io = ImGui::GetIO();
        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

        // Upload texture to graphics system
        GLint last_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
        glGenTextures(1, &g_FontTexture);
        glBindTexture(GL_TEXTURE_2D, g_FontTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        // Store our identifier
        io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

        // Restore state
        glBindTexture(GL_TEXTURE_2D, last_texture);

        return true;
    }

	//--------------------------------------------------------------
	void remapToGLFWConvention(int& button)
	{
		switch (button)
		{

		case 0:
		{
			break;
		}
		case 1:
		{
			button = 2;
			break;
		}
		case 2:
		{
			button = 1;
			break;
		}
		}
	}

	//--------------------------------------------------------------
	void EngineGLFW::onMousePressed(ofMouseEventArgs& event)
	{
		int button = event.button;
		if (button >= 0 && button < 5)
		{
			remapToGLFWConvention(button);
			mousePressed[button] = true;
		}
	}

	//--------------------------------------------------------------
	void EngineGLFW::onMouseReleased(ofMouseEventArgs& event)
	{
		int button = event.button;
		if (button >= 0 && button < 5)
		{
			remapToGLFWConvention(button);
			mousePressed[button] = false;
		}
	}

	//--------------------------------------------------------------
	void EngineGLFW::programmableDrawData(ImDrawData * draw_data)
	{
		// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		ImGuiIO& io = ImGui::GetIO();
		int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
		int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
		if (fb_width == 0 || fb_height == 0)
			return;
		draw_data->ScaleClipRects(io.DisplayFramebufferScale);

		// Backup GL state
		GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
		glActiveTexture(GL_TEXTURE0);
		GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
		GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		GLint last_sampler; glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
		GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
		GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
		GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
		GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
		GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
		GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
		GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
		GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
		GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
		GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
		GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
		GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
		GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
		GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
		GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
		GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

		// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Setup viewport, orthographic projection matrix
		glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
		const float ortho_projection[4][4] =
		{
			{ 2.0f/io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
			{ 0.0f,                  2.0f/-io.DisplaySize.y, 0.0f, 0.0f },
			{ 0.0f,                  0.0f,                  -1.0f, 0.0f },
			{-1.0f,                  1.0f,                   0.0f, 1.0f },
		};
		glUseProgram(g_ShaderHandle);
		glUniform1i(g_UniformLocationTex, 0);
		glUniformMatrix4fv(g_UniformLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
		glBindVertexArray(g_VaoHandle);
		glBindSampler(0, 0); // Rely on combined texture/sampler state.

		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			const ImDrawIdx* idx_buffer_offset = 0;

			glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
					glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
					glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
				}
				idx_buffer_offset += pcmd->ElemCount;
			}
		}

		// Restore modified GL state
		glUseProgram(last_program);
		glBindTexture(GL_TEXTURE_2D, last_texture);
		glBindSampler(0, last_sampler);
		glActiveTexture(last_active_texture);
		glBindVertexArray(last_vertex_array);
		glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
		glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
		glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
		if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
		if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
		if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
		if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, last_polygon_mode[0]);
		glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
		glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
	}

	//--------------------------------------------------------------
	void EngineGLFW::fixedDrawData(ImDrawData * draw_data)
	{
		// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		ImGuiIO& io = ImGui::GetIO();
		int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
		int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
		if (fb_width == 0 || fb_height == 0)
			return;
		draw_data->ScaleClipRects(io.DisplayFramebufferScale);

		// We are using the OpenGL fixed pipeline to make the example code simpler to read!
		// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers, polygon fill.
		GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
		GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
		GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
		glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound

		// Setup viewport, orthographic projection matrix
		glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		// Render command lists
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
			const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
			glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, pos)));
			glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, uv)));
			glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, col)));

			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
					glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
					glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
				}
				idx_buffer += pcmd->ElemCount;
			}
		}

		// Restore modified state
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopAttrib();
		glPolygonMode(GL_FRONT, last_polygon_mode[0]); glPolygonMode(GL_BACK, last_polygon_mode[1]);
		glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
		glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
	}

	//--------------------------------------------------------------
	void EngineGLFW::onKeyReleased(ofKeyEventArgs& event)
	{
		int key = event.keycode;
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = false;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	}

	//--------------------------------------------------------------
	void EngineGLFW::onKeyPressed(ofKeyEventArgs& event)
	{
		int key = event.keycode;
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		bool isNumericalKey = (key >= GLFW_KEY_KP_0) && (key <= GLFW_KEY_KP_EQUAL);
		if (key < GLFW_KEY_ESCAPE || isNumericalKey)
		{
			io.AddInputCharacter((unsigned short)event.codepoint);
		}
	}

	//--------------------------------------------------------------
	bool EngineGLFW::createDeviceObjects()
	{
		if (ofIsGLProgrammableRenderer())
		{
			// Backup GL state
			GLint last_texture, last_array_buffer, last_vertex_array;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

			const GLchar *vertex_shader =
			"#version 150\n"
			"uniform mat4 ProjMtx;\n"
			"in vec2 Position;\n"
			"in vec2 UV;\n"
			"in vec4 Color;\n"
			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"	Frag_UV = UV;\n"
			"	Frag_Color = Color;\n"
			"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

			const GLchar* fragment_shader =
			"#version 150\n"
			"uniform sampler2D Texture;\n"
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Color;\n"
			"out vec4 Out_Color;\n"
			"void main()\n"
			"{\n"
			"	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
			"}\n";

			g_ShaderHandle = glCreateProgram();
			g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
			g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(g_VertHandle, 1, &vertex_shader, 0);
			glShaderSource(g_FragHandle, 1, &fragment_shader, 0);
			glCompileShader(g_VertHandle);
			glCompileShader(g_FragHandle);
			glAttachShader(g_ShaderHandle, g_VertHandle);
			glAttachShader(g_ShaderHandle, g_FragHandle);
			glLinkProgram(g_ShaderHandle);

			g_UniformLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
			g_UniformLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
			g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
			g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
			g_AttribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");

			glGenBuffers(1, &g_VboHandle);
			glGenBuffers(1, &g_ElementsHandle);

			glGenVertexArrays(1, &g_VaoHandle);
			glBindVertexArray(g_VaoHandle);
			glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
			glEnableVertexAttribArray(g_AttribLocationPosition);
			glEnableVertexAttribArray(g_AttribLocationUV);
			glEnableVertexAttribArray(g_AttribLocationColor);

			glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
			glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
			glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

			createFontsTexture();

			// Restore modified GL state
			glBindTexture(GL_TEXTURE_2D, last_texture);
			glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
			glBindVertexArray(last_vertex_array);

			return true;
		}
		else
		{
			createFontsTexture();

			return true;
		}
	}


	//--------------------------------------------------------------
	void EngineGLFW::invalidateDeviceObjects()
	{
		if (ofIsGLProgrammableRenderer())
		{
			if (g_VaoHandle) glDeleteVertexArrays(1, &g_VaoHandle);
			if (g_VboHandle) glDeleteBuffers(1, &g_VboHandle);
			if (g_ElementsHandle) glDeleteBuffers(1, &g_ElementsHandle);
			g_VaoHandle = g_VboHandle = g_ElementsHandle = 0;

			if (g_ShaderHandle && g_VertHandle) glDetachShader(g_ShaderHandle, g_VertHandle);
			if (g_VertHandle) glDeleteShader(g_VertHandle);
			g_VertHandle = 0;

			if (g_ShaderHandle && g_FragHandle) glDetachShader(g_ShaderHandle, g_FragHandle);
			if (g_FragHandle) glDeleteShader(g_FragHandle);
			g_FragHandle = 0;

			if (g_ShaderHandle) glDeleteProgram(g_ShaderHandle);
			g_ShaderHandle = 0;
		}

		if (g_FontTexture)
		{
			glDeleteTextures(1, &g_FontTexture);
            //JVC: This is causing an error
			ImGui::GetIO().Fonts->TexID = 0;
			g_FontTexture = 0;
		}
	}   
#endif // OFXIMGUI_ENABLE_OF_BINDINGS

    bool EngineGLFW::updateFontsTexture(){
#ifdef OFXIMGUI_ENABLE_OF_BINDINGS
        return createFontsTexture();
#else
        if (ofIsGLProgrammableRenderer()) {
            return ImGui_ImplOpenGL3_CreateFontsTexture();
        }
        else {
            return ImGui_ImplOpenGL2_CreateFontsTexture();
        }
#endif
    }
}

#endif

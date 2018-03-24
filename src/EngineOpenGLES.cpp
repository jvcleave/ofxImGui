#include "EngineOpenGLES.h"

#if defined(TARGET_OPENGLES) && (!defined (OF_TARGET_API_VULKAN) )

#include "ofAppRunner.h"
#include "ofGLProgrammableRenderer.h"

namespace ofxImGui
{
	ofShader EngineOpenGLES::g_Shader;

	//--------------------------------------------------------------
	void EngineOpenGLES::setup(bool autoDraw)
	{
		if (isSetup) return;

		ImGuiIO& io = ImGui::GetIO();

		io.KeyMap[ImGuiKey_Tab] = OF_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = OF_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = OF_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = OF_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = OF_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = OF_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = OF_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = OF_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = OF_KEY_END;
		io.KeyMap[ImGuiKey_Delete] = OF_KEY_DEL;
		io.KeyMap[ImGuiKey_Backspace] = OF_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Enter] = OF_KEY_RETURN;
		io.KeyMap[ImGuiKey_Escape] = OF_KEY_ESC;

		if (autoDraw)
		{
			io.RenderDrawListsFn = rendererDrawData;
		}
		
		io.SetClipboardTextFn = &BaseEngine::setClipboardString;
		io.GetClipboardTextFn = &BaseEngine::getClipboardString;

		createDeviceObjects();

		// Override listeners
		ofAddListener(ofEvents().keyReleased, this, &EngineOpenGLES::onKeyReleased);

		// BaseEngine listeners
		ofAddListener(ofEvents().keyPressed, (BaseEngine*)this, &BaseEngine::onKeyPressed);
		ofAddListener(ofEvents().mouseDragged, (BaseEngine*)this, &BaseEngine::onMouseDragged);
		ofAddListener(ofEvents().mousePressed, (BaseEngine*)this, &BaseEngine::onMousePressed);
		ofAddListener(ofEvents().mouseReleased, (BaseEngine*)this, &BaseEngine::onMouseReleased);
		ofAddListener(ofEvents().mouseScrolled, (BaseEngine*)this, &BaseEngine::onMouseScrolled);
		ofAddListener(ofEvents().windowResized, (BaseEngine*)this, &BaseEngine::onWindowResized);

		isSetup = true;
	}

	//--------------------------------------------------------------
	void EngineOpenGLES::exit()
	{
		if (!isSetup) return;

		// Override listeners
		ofRemoveListener(ofEvents().keyReleased, this, &EngineOpenGLES::onKeyReleased);

		// BaseEngine listeners
		ofRemoveListener(ofEvents().keyPressed, (BaseEngine*)this, &BaseEngine::onKeyPressed);
		ofRemoveListener(ofEvents().mouseDragged, (BaseEngine*)this, &BaseEngine::onMouseDragged);
		ofRemoveListener(ofEvents().mousePressed, (BaseEngine*)this, &BaseEngine::onMousePressed);
		ofRemoveListener(ofEvents().mouseReleased, (BaseEngine*)this, &BaseEngine::onMouseReleased);
		ofRemoveListener(ofEvents().mouseScrolled, (BaseEngine*)this, &BaseEngine::onMouseScrolled);
		ofRemoveListener(ofEvents().windowResized, (BaseEngine*)this, &BaseEngine::onWindowResized);

		invalidateDeviceObjects();

		isSetup = false;
	}

	bool EngineOpenGLES::createDeviceObjects()
	{
//#if defined(TARGET_RASPBERRY_PI)
//		std::string header = "";
//#else
		std::string header = "precision highp float; \n";
//#endif
		std::string vertex_shader = header + R"(
    
    uniform mat4 ProjMat;
    
    attribute vec2 Position;
    attribute vec2 UV;
    attribute vec4 Color;
    
    varying vec2 Frag_UV;
    varying vec4 Frag_Color;
    
    void main()
    {
        Frag_UV = UV;
        Frag_Color = Color;
        gl_Position = ProjMat * vec4(Position.xy, 0, 1);
    }
    
    )";

		std::string fragment_shader = header + R"(
    
    uniform sampler2D Texture;
    
    varying vec2 Frag_UV;
    varying vec4 Frag_Color;
    
    void main()
    {
        gl_FragColor = Frag_Color * texture2D( Texture, Frag_UV);
    }
    
    )";


		g_Shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment_shader);
		g_Shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex_shader);

		g_Shader.linkProgram();

		// Backup GL state
		GLint last_texture, last_array_buffer;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);

		g_ShaderHandle = g_Shader.getProgram();

		g_UniformLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
		g_UniformLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMat");
		g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
		g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
		g_AttribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");

		glGenBuffers(1, &g_VboHandle);
		glGenBuffers(1, &g_ElementsHandle);

		ImGuiIO& io = ImGui::GetIO();

		// Build texture
		unsigned char* pixels;
		int width, height;

		if (ofIsGLProgrammableRenderer())
		{
			// Load as RGBA 32-bits for OpenGL3 because it is more likely to be compatible with user's existing shader.
			io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RGBA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				pixels);

		}
		else
		{
			io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_ALPHA,
				width,
				height,
				0,
				GL_ALPHA,
				GL_UNSIGNED_BYTE,
				pixels);
		}

		GLuint textureid = loadTextureImage2D(pixels, width, height);
		io.Fonts->TexID = (void *)(intptr_t)textureid;

		io.Fonts->ClearTexData();

		// Restore modified GL state
		glBindTexture(GL_TEXTURE_2D, last_texture);
		glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);

		return true;
	}

	//--------------------------------------------------------------
	void EngineOpenGLES::invalidateDeviceObjects()
	{
		if (g_VboHandle) glDeleteBuffers(1, &g_VboHandle);
		if (g_ElementsHandle) glDeleteBuffers(1, &g_ElementsHandle);
		g_VboHandle = g_ElementsHandle = 0;

		g_Shader.unload();
		g_ShaderHandle = 0;
	}

	//--------------------------------------------------------------
	void EngineOpenGLES::draw()
	{
		rendererDrawData(ImGui::GetDrawData());
	}

	//--------------------------------------------------------------
	void EngineOpenGLES::rendererDrawData(ImDrawData * draw_data)
	{
		GLint last_program, last_texture, last_array_buffer, last_element_array_buffer;
		glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);

		// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glActiveTexture(GL_TEXTURE0);

		float width = ofGetWidth();
		float height = ofGetHeight();

		// Setup orthographic projection matrix
		const float ortho_projection[4][4] =
		{
			{ 2.0f / width,           0.0f,                   0.0f, 0.0f },
			{ 0.0f,                  2.0f / -height,           0.0f, 0.0f },
			{ 0.0f,                  0.0f,                  -1.0f, 0.0f },
			{-1.0f,                  1.0f,                   0.0f, 1.0f },
		};
		glUseProgram(g_ShaderHandle);
		glUniform1i(g_UniformLocationTex, 0);
		glUniformMatrix4fv(g_UniformLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);

		// Render command lists
		glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
		glEnableVertexAttribArray(g_AttribLocationPosition);
		glEnableVertexAttribArray(g_AttribLocationUV);
		glEnableVertexAttribArray(g_AttribLocationColor);

		glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
		glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
		glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));

		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			const ImDrawIdx* idx_buffer_offset = 0;

			glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

			for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
			{
				if (pcmd->UserCallback)
				{
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
					glScissor((int)pcmd->ClipRect.x, (int)(height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
					glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer_offset);

				}
				idx_buffer_offset += pcmd->ElemCount;
			}
		}

		// Restore modified state
		glDisableVertexAttribArray(g_AttribLocationPosition);
		glDisableVertexAttribArray(g_AttribLocationUV);
		glDisableVertexAttribArray(g_AttribLocationColor);
		glUseProgram(last_program);
		glBindTexture(GL_TEXTURE_2D, last_texture);
		glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
		glDisable(GL_SCISSOR_TEST);
	}

	//--------------------------------------------------------------
	void EngineOpenGLES::onKeyReleased(ofKeyEventArgs& event)
	{
		int key = event.keycode;
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = false;
		io.AddInputCharacter((unsigned short)event.codepoint);
		//TODO modifiers?
	}
}

#endif


#include "EngineVk.h"

#if defined(OF_TARGET_API_VULKAN)

#include "ofAppGLFWWindow.h"
#include "vk/ofVkRenderer.h"
#include "vk/ImageAllocator.h"
#include "vk/Context.h"
#include "vk/Texture.h"
#include "vk/RenderBatch.h"
#include "vk/Shader.h"
#include "vk/DrawCommand.h"
#include <glm/glm.hpp>

// We keep a shared pointer to the renderer so we don't have to 
// fetch it anew every time we need it.


namespace ofxImGui
{

	::of::vk::RenderBatch*                  EngineVk::batch = nullptr; // current renderbatch
	std::unique_ptr<of::vk::ImageAllocator> EngineVk::mImageAllocator;
	std::shared_ptr<of::vk::Texture>        EngineVk::mFontTexture;    // wrapper with sampler around font texture
	std::shared_ptr<::vk::Image>            EngineVk::mFontImage;      // data store for image data
	::vk::Device                            EngineVk::mDevice;         // non-owning reference to vk device

	std::unique_ptr<of::vk::DrawCommand>    EngineVk::mDrawCommand;    // draw command prototype

	void EngineVk::setRenderBatch( of::vk::RenderBatch & batch_ ){
		batch = &batch_;
	}

	//--------------------------------------------------------------
	
	void EngineVk::setup(bool autoDraw)
	{
		if (isSetup) return;

		mRenderer = dynamic_pointer_cast<ofVkRenderer>( ofGetCurrentRenderer() );
		mDevice  = mRenderer->getVkDevice();

		setupImageAllocator();

		ImGuiIO& io = ImGui::GetIO();

		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		io.RenderDrawListsFn = render;

		io.SetClipboardTextFn = &BaseEngine::setClipboardString;
		io.GetClipboardTextFn = &BaseEngine::getClipboardString;

		createDeviceObjects();

		// Override listeners
		ofAddListener(ofEvents().mousePressed, this, &EngineVk::onMousePressed);
		ofAddListener(ofEvents().keyReleased, this, &EngineVk::onKeyReleased);
		ofAddListener(ofEvents().keyPressed, this, &EngineVk::onKeyPressed);

		// BaseEngine listeners
		ofAddListener(ofEvents().mouseDragged, (BaseEngine*)this, &BaseEngine::onMouseDragged);
		ofAddListener(ofEvents().mouseReleased, (BaseEngine*)this, &BaseEngine::onMouseReleased);
		ofAddListener(ofEvents().mouseScrolled, (BaseEngine*)this, &BaseEngine::onMouseScrolled);
		ofAddListener(ofEvents().windowResized, (BaseEngine*)this, &BaseEngine::onWindowResized);

		isSetup = true;

	}
	
	//--------------------------------------------------------------

	void EngineVk::setupImageAllocator(){

		const auto & rendererProperties = mRenderer->getVkRendererProperties();

		{
				of::vk::ImageAllocator::Settings allocatorSettings;
				allocatorSettings.device = mDevice;
				allocatorSettings.size = ( 1 << 24UL ); // 16 MB
				allocatorSettings.memFlags = ::vk::MemoryPropertyFlagBits::eDeviceLocal;
				allocatorSettings.physicalDeviceMemoryProperties = rendererProperties.physicalDeviceMemoryProperties;
				allocatorSettings.physicalDeviceProperties = rendererProperties.physicalDeviceProperties;

				mImageAllocator = std::make_unique<of::vk::ImageAllocator>( allocatorSettings );
				mImageAllocator->setup();
		}
	}

	//--------------------------------------------------------------

	void EngineVk::exit()
	{
		if (!isSetup) return;

		// Override listeners
		ofRemoveListener(ofEvents().mousePressed, this, &EngineVk::onMousePressed);
		ofRemoveListener(ofEvents().keyReleased, this, &EngineVk::onKeyReleased);
		ofRemoveListener(ofEvents().keyPressed, this, &EngineVk::onKeyPressed);

		// Base class listeners
		ofRemoveListener(ofEvents().mouseDragged, (BaseEngine*)this, &BaseEngine::onMouseDragged);
		ofRemoveListener(ofEvents().mouseReleased, (BaseEngine*)this, &BaseEngine::onMouseReleased);
		ofRemoveListener(ofEvents().mouseScrolled, (BaseEngine*)this, &BaseEngine::onMouseScrolled);
		ofRemoveListener(ofEvents().windowResized, (BaseEngine*)this, &BaseEngine::onWindowResized);

		invalidateDeviceObjects();

		isSetup = false;

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

	void EngineVk::onMousePressed(ofMouseEventArgs& event)
	{
		int button = event.button;
		if (button >= 0 && button < 5)
		{
			remapToGLFWConvention(button);
			mousePressed[button] = true;
			mouseReleased = false;
		}
	}

	//--------------------------------------------------------------

	void EngineVk::render(ImDrawData * draw_data)
	{
		// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		ImGuiIO& io = ImGui::GetIO();
		int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
		int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
		if (fb_width == 0 || fb_height == 0)
			return;
		draw_data->ScaleClipRects(io.DisplayFramebufferScale);

		auto & alloc = batch->getContext()->getTransientAllocator();

		::vk::DeviceSize offset = 0;
		void * dataP = nullptr;

		const glm::mat4 ortho_projection =
		{
			{ 2.0f / io.DisplaySize.x, 0.0f,                    0.0f, 0.0f },
			{ 0.0f,                    2.0f / io.DisplaySize.y, 0.0f, 0.0f },
			{ 0.0f,                    0.0f,                    1.0f, 0.0f },
			{ -1.f,                    -1.f,                    0.0f, 1.0f },
		};

		// create a 
		auto dc = *mDrawCommand;

		dc.setUniform( "modelViewProjectionMatrix", ortho_projection );

		for ( int n = 0; n < draw_data->CmdListsCount; n++ ){
			const ImDrawList* cmd_list = draw_data->CmdLists[n];

			dc.allocAndSetAttribute( 0, (void*)cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof( ImDrawVert ), alloc );
			dc.allocAndSetIndices(cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof( ImDrawIdx ), alloc );
			
			for ( int cmd_i = 0, idx_offset = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++ ){
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if ( pcmd->UserCallback ){
					pcmd->UserCallback( cmd_list, pcmd );
				} else{

					// Set texture from texture id stored with ImGui draw command
					
					dc.setTexture( "tex_unit_0", *( static_cast<of::vk::Texture*>( pcmd->TextureId ) ) );

					//VkRect2D scissor;
					//scissor.offset.x = (int32_t)( pcmd->ClipRect.x ) > 0 ? (int32_t)( pcmd->ClipRect.x ) : 0;
					//scissor.offset.y = (int32_t)( pcmd->ClipRect.y ) > 0 ? (int32_t)( pcmd->ClipRect.y ) : 0;
					//scissor.extent.width = (uint32_t)( pcmd->ClipRect.z - pcmd->ClipRect.x );
					//scissor.extent.height = (uint32_t)( pcmd->ClipRect.w - pcmd->ClipRect.y + 1 ); // FIXME: Why +1 here?
					//vkCmdSetScissor( g_CommandBuffer, 0, 1, &scissor );

					batch->draw( dc, pcmd->ElemCount, 1, idx_offset, 0, 0 );
				}

				idx_offset += pcmd->ElemCount;
			}
		}

	}

	//--------------------------------------------------------------

	void EngineVk::onKeyReleased(ofKeyEventArgs& event)
	{
		int key = event.keycode;
		
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = false;

		io.KeyCtrl  = event.modifiers & GLFW_MOD_CONTROL;
		io.KeyShift = event.modifiers & GLFW_MOD_SHIFT;
		io.KeyAlt   = event.modifiers & GLFW_MOD_ALT;
		io.KeySuper = event.modifiers & GLFW_MOD_SUPER;
	}

	//--------------------------------------------------------------
	
	void EngineVk::onKeyPressed(ofKeyEventArgs& event) {
		int key = event.keycode;
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = true;

		io.KeyCtrl  = event.modifiers & GLFW_MOD_CONTROL;
		io.KeyShift = event.modifiers & GLFW_MOD_SHIFT;
		io.KeyAlt   = event.modifiers & GLFW_MOD_ALT;
		io.KeySuper = event.modifiers & GLFW_MOD_SUPER;

		if (key < GLFW_KEY_ESCAPE)
		{
			io.AddInputCharacter((unsigned short)event.codepoint);
		}
	}

	//--------------------------------------------------------------
	
	void EngineVk::createDrawCommands(){

		of::vk::Shader::Settings shaderSettings;

		shaderSettings.device = mDevice;
		shaderSettings.printDebugInfo = true;
		shaderSettings.sources[::vk::ShaderStageFlagBits::eVertex]   = "imgui.vert";
		shaderSettings.sources[::vk::ShaderStageFlagBits::eFragment] = "imgui.frag";

		auto vertexInfo = std::make_shared<of::vk::Shader::VertexInfo>();

		vertexInfo->attributeNames = { "inPos", "inTexCoord", "inColor" };
		vertexInfo->attribute = std::vector<::vk::VertexInputAttributeDescription>( 3 );
		
		vertexInfo->attribute[0]
			.setLocation( 0 )
			.setBinding( 0 )
			.setOffset( offsetof(ImDrawVert, pos) )
			.setFormat( ::vk::Format::eR32G32Sfloat )
			;
		
		vertexInfo->attribute[1]
			.setLocation( 1 )
			.setBinding( 0 )
			.setOffset( offsetof( ImDrawVert, uv ) )
			.setFormat(::vk::Format::eR32G32Sfloat)
			;

		vertexInfo->attribute[2]
			.setLocation( 2 )
			.setBinding( 0 )
			.setOffset( offsetof( ImDrawVert, col ) )
			.setFormat( ::vk::Format::eR8G8B8A8Unorm )
			;
		
		// only one binding description, as we're only using one binding.
		vertexInfo->bindingDescription = { { 0, sizeof( ImDrawVert ), ::vk::VertexInputRate::eVertex } };

		// by setting vertexInfo like this we prevent the shader from reflecting
		shaderSettings.vertexInfo = vertexInfo;

		auto imGuiShader = std::make_shared<of::vk::Shader>( shaderSettings );

		of::vk::GraphicsPipelineState pipeline;

		pipeline.setShader( imGuiShader );

		pipeline.rasterizationState
			.setCullMode( ::vk::CullModeFlagBits::eNone )
			.setFrontFace( ::vk::FrontFace::eCounterClockwise )
			.setPolygonMode( ::vk::PolygonMode::eFill )
			.setLineWidth( 1.f )
			;

		pipeline.depthStencilState
			.setDepthTestEnable( VK_FALSE )
			.setDepthWriteEnable( VK_FALSE )
			;

		pipeline.blendAttachmentStates[0]
			.setBlendEnable( VK_TRUE )
			.setSrcColorBlendFactor( ::vk::BlendFactor::eSrcAlpha )
			.setDstColorBlendFactor( ::vk::BlendFactor::eOneMinusSrcAlpha )
			.setColorBlendOp( ::vk::BlendOp::eAdd )
			.setSrcAlphaBlendFactor( ::vk::BlendFactor::eOneMinusSrcAlpha )
			.setDstAlphaBlendFactor( ::vk::BlendFactor::eZero )
			.setAlphaBlendOp( ::vk::BlendOp::eAdd )
			.setColorWriteMask(::vk::ColorComponentFlagBits::eR | ::vk::ColorComponentFlagBits::eG | ::vk::ColorComponentFlagBits::eB | ::vk::ColorComponentFlagBits::eA )
			;

		mDrawCommand = std::make_unique<of::vk::DrawCommand>();
		mDrawCommand->setup( pipeline );

	}

	//--------------------------------------------------------------
	
	bool EngineVk::createDeviceObjects()
	{
		// create draw command prototype
		createFontsTexture();
		createDrawCommands();
		// attach font texture to draw command
		mDrawCommand->setTexture( "tex_unit_0", *mFontTexture );

		return true;
	}

	//--------------------------------------------------------------
	
	bool EngineVk::createFontsTexture()
	{
		// Build texture atlas
		ImGuiIO& io = ImGui::GetIO();

		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32( &pixels, &width, &height );
		size_t upload_size = width*height * 4 * sizeof( char );

		of::vk::ImageTransferSrcData imgData;
		imgData.pData = pixels;
		imgData.numBytes = upload_size;
		imgData.extent.width = width;
		imgData.extent.height = height;

		mFontImage = mRenderer->getStagingContext()->storeImageCmd( imgData, mImageAllocator );

		::vk::SamplerCreateInfo samplerInfo = of::vk::Texture::getDefaultSamplerCreateInfo();
		
		samplerInfo
			.setMagFilter( ::vk::Filter::eLinear )
			.setMinFilter( ::vk::Filter::eLinear )
			.setMipmapMode( ::vk::SamplerMipmapMode::eLinear )
			.setMinLod( -1000 )
			.setMaxLod( 1000 )
			.setMaxAnisotropy( 1.0f )
			.setAddressModeU( ::vk::SamplerAddressMode::eRepeat )
			.setAddressModeV( ::vk::SamplerAddressMode::eRepeat )
			.setAddressModeW( ::vk::SamplerAddressMode::eRepeat )
			;

		auto imageViewCreateInfo = of::vk::Texture::getDefaultImageViewCreateInfo(*mFontImage);

		mFontTexture = std::make_shared<of::vk::Texture>( mRenderer->getVkDevice(), samplerInfo , imageViewCreateInfo);

		// Store our identifier
		io.Fonts->TexID = (void *)( mFontTexture.get());

		return true;
	}

	//--------------------------------------------------------------
	void EngineVk::invalidateDeviceObjects()
	{
		mDrawCommand.reset();
		mImageAllocator.reset();
		mFontTexture.reset();    // wrapper with sampler around font texture
		mFontImage.reset();      // data store for image data
	}

} // end namespace ofxImGui

#endif

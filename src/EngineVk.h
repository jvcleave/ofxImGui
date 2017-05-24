#pragma once

#include "ofConstants.h"
#if defined(OF_TARGET_API_VULKAN)

#include "BaseEngine.h"

#include "ofEvents.h"
#include "imgui.h"
#include <vulkan/vulkan.hpp>

#include "vk/ImageAllocator.h"
#include "vk/Context.h"
#include "vk/Texture.h"
#include "vk/DrawCommand.h"
#include "vk/RenderBatch.h"

class ofVkRenderer; // ffdecl.


namespace ofxImGui
{
	class EngineVk
		: public BaseEngine
	{
	public:
		~EngineVk()
		{
			exit();
		}

		// BaseEngine required
		void setup() override;
		void exit() override;
		bool createDeviceObjects() override;
		void invalidateDeviceObjects() override;

		bool createFontsTexture();

		void onKeyReleased(ofKeyEventArgs& event) override;
		void onKeyPressed(ofKeyEventArgs& event) override;
		void onMousePressed(ofMouseEventArgs& event) override;

		// Custom 
		static void render(ImDrawData * draw_data);
		static void setRenderBatch( of::vk::RenderBatch& batch );

	private:

		static ::of::vk::RenderBatch* batch; // current batch used for drawing

		std::shared_ptr<ofVkRenderer> mRenderer;  
		
		static ::vk::Device mDevice;                           // non-owning reference to vk device

		static std::unique_ptr<of::vk::ImageAllocator>  mImageAllocator;

		static std::shared_ptr<of::vk::Texture>  mFontTexture; // wrapper with sampler around font texture
		static std::shared_ptr<::vk::Image>      mFontImage;   // data store for image data

		static std::unique_ptr<of::vk::DrawCommand> mDrawCommand; // prototype draw command
		
		void createDrawCommands();
		void setupImageAllocator();
		
	};

}

#endif
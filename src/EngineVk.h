#ifndef GUARD_ENGINE_VK_H
#define GUARD_ENGINE_VK_H

// We include this to check for #define OF_TARGET_API_VULKAN
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
		void setup(bool autoDraw) override;
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

		static ::vk::Device                             mDevice;         // Non-owning reference to current VK device
		std::shared_ptr<ofVkRenderer>                   mRenderer;       // Reference to current renderer
		static ::of::vk::RenderBatch*                   batch;           // Current batch used for drawing
		static std::unique_ptr<of::vk::ImageAllocator>  mImageAllocator; // Allocator used for font texture
		static std::shared_ptr<::vk::Image>             mFontImage;      // Data store for image data
		static std::shared_ptr<of::vk::Texture>         mFontTexture;    // Wrapper with sampler around font texture
		static std::unique_ptr<of::vk::DrawCommand>     mDrawCommand;    // Used to draw ImGui components
		
		void createDrawCommands();
		void setupImageAllocator();
		
	};

}

#endif // OF_TARGET_API_VULKAN
#endif // GUARD_ENGINE_VK_H

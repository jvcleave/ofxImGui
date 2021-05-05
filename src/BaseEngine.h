#pragma once

#include "ofEvents.h"
#include "ofAppBaseWindow.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

namespace ofxImGui
{
	class BaseEngine
	{
	public:
		BaseEngine()
			: isSetup(false)
		{}

		virtual ~BaseEngine()
		{}

		virtual void setup(bool autoDraw) = 0;
		virtual void exit() = 0;

        virtual void newFrame() = 0;
        virtual void endFrame() = 0; // todo: remove ?
        virtual void render() = 0;

        virtual bool updateFontsTexture() = 0;

		virtual GLuint loadTextureImage2D(unsigned char * pixels, int width, int height);

	protected:
		bool isSetup;
	};
}


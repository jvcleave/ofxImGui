#ifndef GUARD_IM_GUI_LOGGER_CHANNEL
#define GUARD_IM_GUI_LOGGER_CHANNEL
#include "imgui.h"
#include "ofLog.h"

namespace ofxImGui {

/// \brief A logger channel that logs its messages to the console.
class LoggerChannel : public ofBaseLoggerChannel
{
public:

	static ImGuiTextBuffer & getBuffer();

	/// \brief Destroy the console logger channel.
	virtual ~LoggerChannel(){};
	void log( ofLogLevel level, const std::string & module, const std::string & message );
	void log( ofLogLevel level, const std::string & module, const char* format, ... ) OF_PRINTF_ATTR( 4, 5 );
	void log( ofLogLevel level, const std::string & module, const char* format, va_list args );
};

} // end namespace ofxImGui

#endif // IM_GUI_LOGGER_CHANNEL
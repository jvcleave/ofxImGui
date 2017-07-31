#include "ofxImGuiLoggerChannel.h"

//--------------------------------------------------------------

ImGuiTextBuffer& ofxImGui::LoggerChannel::getBuffer(){
	static ImGuiTextBuffer sLogBuffer; // static log buffer for logger channel
	return sLogBuffer;
};

//--------------------------------------------------------------
void ofxImGui::LoggerChannel::log( ofLogLevel level, const string & module, const string & message ){
	getBuffer().append( "[%s] %s: %s\n", ofGetLogLevelName( level, true ).c_str(), module.c_str(), message.c_str() );
}

void ofxImGui::LoggerChannel::log( ofLogLevel level, const string & module, const char* format, ... ){
	va_list args;
	va_start( args, format );
	log( level, module, format, args );
	va_end( args );
}

void ofxImGui::LoggerChannel::log( ofLogLevel level, const string & module, const char* format, va_list args ){
	getBuffer().append( "[%s] %s: %s", ofGetLogLevelName( level, true ).c_str(), module.c_str() );
	getBuffer().appendv( format, args );
}


//--------------------------------------------------------------

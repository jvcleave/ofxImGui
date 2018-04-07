#pragma once

#include "ofMain.h"
#include "ofParameter.h"
#include "ofxImGui.h"

class ofApp 
	: public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();

	void saveToFile(const std::string& filename, ofAbstractParameter & parameter);
	void loadFromFile(const std::string& filename, ofAbstractParameter & parameter);
	void SaveAsSettings();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void ShowAppMainMenuBar();
	void ShowAppMenuFile();

protected:
	enum class ServoSelect
	{
		Servo0,
		Servo1,
		Servo2,
		Servo3,
		Servo4,
		Servo5,
		Servo6,
		Servo_all,
	};

	// Render
	bool loadImage(const string & filePath);
	ofTexture texture;

	// Gui
	bool imGui();

	ofxImGui::Gui gui;
	bool guiVisible;
	bool mouseOverGui;

	// Parameters
	ofParameter<int> servoSelect{ "##Servo Select", static_cast<int>(ServoSelect::Servo0) };
	ofParameter<bool> servo_statusled{ "Status LED", true };
	ofParameter<int> servo_min_voltage{ "##servo min voltage", 5000, 5000, 65535 };
	ofParameter<int> servo_out_voltage{ "##servo out voltage", 5500, 2000, 9000 };
	ofParameter<bool> allservos_enabled{ "Enabled", false };
	ofParameterGroup servo_main_parameters{ "Servos_main_parameters", servoSelect, servo_statusled, servo_min_voltage, servo_out_voltage, allservos_enabled };

	ofParameter<bool> servo0_enabled{ "Servo0 Enabled", false };
	ofParameter<int> servo0_Pulse_Width_min{ "##Servo0 Pulse Width min", 1000 };
	ofParameter<int> servo0_Pulse_Width_max{ "##Servo0 Pulse Width max", 2000 };
	ofParameter<int> servo0_Degree_min{ "##Servo0 Degree min", -9000 };
	ofParameter<int> servo0_Degree_max{ "##Servo0 Degree max", 9000 };
	ofParameter<int> servo0_Position{ "Servo0 Position (°/100)", 0, -9000, 9000 };
	ofParameter<int> servo0_Velocity{ "Servo0 Velocity (°/100s)", 65535, 0, 65535 };
	ofParameter<int> servo0_Acceleration{ "Servo0 Acceleration (°/100s²)", 65535, 0, 65535 };
	ofParameter<int> servo0_Period{ "Servo0 Period (µs)", 19500, 1, 65535 };
	ofParameterGroup servo0_Parameters{ "Servo0_parameters", servo0_enabled, servo0_Pulse_Width_min, servo0_Pulse_Width_max, servo0_Degree_min, servo0_Degree_max, servo0_Position, servo0_Velocity, servo0_Acceleration, servo0_Period };

	ofParameter<bool> servo1_enabled{ "Enabled", false };
	ofParameter<int> servo1_Pulse_Width_min{ "##Servo1 Pulse Width min", 1000 };
	ofParameter<int> servo1_Pulse_Width_max{ "##Servo1 Pulse Width max", 2000 };
	ofParameter<int> servo1_Degree_min{ "##Servo1 Degree min", -9000 };
	ofParameter<int> servo1_Degree_max{ "##Servo1 Degree max", 9000 };
	ofParameter<int> servo1_Position{ "Position (°/100)", 0, -9000, 9000 };
	ofParameter<int> servo1_Velocity{ "Velocity (°/100s)", 65535, 0, 65535 };
	ofParameter<int> servo1_Acceleration{ "Acceleration (°/100s²)", 65535, 0, 65535 };
	ofParameter<int> servo1_Period{ "Period (µs)", 19500, 1, 65535 };
	ofParameterGroup servo1_Parameters{ "Servo1_parameters", servo1_enabled, servo1_Pulse_Width_min, servo1_Pulse_Width_max, servo1_Degree_min, servo1_Degree_max, servo1_Position, servo1_Velocity, servo1_Acceleration, servo1_Period };

	ofParameter<bool> servo2_enabled{ "Enabled", false };
	ofParameter<int> servo2_Pulse_Width_min{ "##Servo2 Pulse Width min", 1000 };
	ofParameter<int> servo2_Pulse_Width_max{ "##Servo2 Pulse Width max", 2000 };
	ofParameter<int> servo2_Degree_min{ "##Servo2 Degree min", -9000 };
	ofParameter<int> servo2_Degree_max{ "##Servo2 Degree max", 9000 };
	ofParameter<int> servo2_Position{ "Position (°/100)", 0, -9000, 9000 };
	ofParameter<int> servo2_Velocity{ "Velocity (°/100s)", 65535, 0, 65535 };
	ofParameter<int> servo2_Acceleration{ "Acceleration (°/100s²)", 65535, 0, 65535 };
	ofParameter<int> servo2_Period{ "Period (µs)", 19500, 1, 65535 };
	ofParameterGroup servo2_Parameters{ "Servo2_parameters", servo2_enabled, servo2_Pulse_Width_min, servo2_Pulse_Width_max, servo2_Degree_min, servo2_Degree_max, servo2_Position, servo2_Velocity, servo2_Acceleration, servo2_Period };

	ofParameter<bool> servo3_enabled{ "Enabled", false };
	ofParameter<int> servo3_Pulse_Width_min{ "##Servo3 Pulse Width min", 1000 };
	ofParameter<int> servo3_Pulse_Width_max{ "##Servo3 Pulse Width max", 2000 };
	ofParameter<int> servo3_Degree_min{ "##Servo3 Degree min", -9000 };
	ofParameter<int> servo3_Degree_max{ "##Servo3 Degree max", 9000 };
	ofParameter<int> servo3_Position{ "Position (°/100)", 0, -9000, 9000 };
	ofParameter<int> servo3_Velocity{ "Velocity (°/100s)", 65535, 0, 65535 };
	ofParameter<int> servo3_Acceleration{ "Acceleration (°/100s²)", 65535, 0, 65535 };
	ofParameter<int> servo3_Period{ "Period (µs)", 19500, 1, 65535 };
	ofParameterGroup servo3_Parameters{ "Servo3_parameters", servo3_enabled, servo3_Pulse_Width_min, servo3_Pulse_Width_max, servo3_Degree_min, servo3_Degree_max, servo3_Position, servo3_Velocity, servo3_Acceleration, servo3_Period };

	ofParameter<bool> servo4_enabled{ "Enabled", false };
	ofParameter<int> servo4_Pulse_Width_min{ "##Servo4 Pulse Width min", 1000 };
	ofParameter<int> servo4_Pulse_Width_max{ "##Servo4 Pulse Width max", 2000 };
	ofParameter<int> servo4_Degree_min{ "##Servo4 Degree min", -9000 };
	ofParameter<int> servo4_Degree_max{ "##Servo4 Degree max", 9000 };
	ofParameter<int> servo4_Position{ "Position (°/100)", 0, -9000, 9000 };
	ofParameter<int> servo4_Velocity{ "Velocity (°/100s)", 65535, 0, 65535 };
	ofParameter<int> servo4_Acceleration{ "Acceleration (°/100s²)", 65535, 0, 65535 };
	ofParameter<int> servo4_Period{ "Period (µs)", 19500, 1, 65535 };
	ofParameterGroup servo4_Parameters{ "Servo4_parameters", servo4_enabled, servo4_Pulse_Width_min, servo4_Pulse_Width_max, servo4_Degree_min, servo4_Degree_max, servo4_Position, servo4_Velocity, servo4_Acceleration, servo4_Period };

	ofParameter<bool> servo5_enabled{ "Enabled", false };
	ofParameter<int> servo5_Pulse_Width_min{ "##Servo5 Pulse Width min", 1000 };
	ofParameter<int> servo5_Pulse_Width_max{ "##Servo5 Pulse Width max", 2000 };
	ofParameter<int> servo5_Degree_min{ "##Servo5 Degree min", -9000 };
	ofParameter<int> servo5_Degree_max{ "##Servo5 Degree max", 9000 };
	ofParameter<int> servo5_Position{ "Position (°/100)", 0, -9000, 9000 };
	ofParameter<int> servo5_Velocity{ "Velocity (°/100s)", 65535, 0, 65535 };
	ofParameter<int> servo5_Acceleration{ "Acceleration (°/100s²)", 65535, 0, 65535 };
	ofParameter<int> servo5_Period{ "Period (µs)", 19500, 1, 65535 };
	ofParameterGroup servo5_Parameters{ "Servo5_parameters", servo5_enabled, servo5_Pulse_Width_min, servo5_Pulse_Width_max, servo5_Degree_min, servo5_Degree_max, servo5_Position, servo5_Velocity, servo5_Acceleration, servo5_Period };

	ofParameter<bool> servo6_enabled{ "Enabled", false };
	ofParameter<int> servo6_Pulse_Width_min{ "##Servo6 Pulse Width min", 1000 };
	ofParameter<int> servo6_Pulse_Width_max{ "##Servo6 Pulse Width max", 2000 };
	ofParameter<int> servo6_Degree_min{ "##Servo6 Degree min", -9000 };
	ofParameter<int> servo6_Degree_max{ "##Servo6 Degree max", 9000 };
	ofParameter<int> servo6_Position{ "Position (°/100)", 0, -9000, 9000 };
	ofParameter<int> servo6_Velocity{ "Velocity (°/100s)", 65535, 0, 65535 };
	ofParameter<int> servo6_Acceleration{ "Acceleration (°/100s²)", 65535, 0, 65535 };
	ofParameter<int> servo6_Period{ "Period (µs)", 19500, 1, 65535 };
	ofParameterGroup servo6_Parameters{ "Servo6_parameters", servo6_enabled, servo6_Pulse_Width_min, servo6_Pulse_Width_max, servo6_Degree_min, servo6_Degree_max, servo6_Position, servo6_Velocity, servo6_Acceleration, servo6_Period };

	ofParameterGroup servos_parameters{ "Servo_parameters",servo_main_parameters ,servo0_Parameters ,servo1_Parameters ,servo2_Parameters ,servo3_Parameters , servo4_Parameters, servo5_Parameters, servo6_Parameters };


	ofParameter<ofFloatColor> background{ "Background", ofFloatColor::black };
	ofParameter<ofFloatColor> foreground{ "Foreground", ofFloatColor::crimson };
	ofParameterGroup colors{ "Colors", background, foreground };
};

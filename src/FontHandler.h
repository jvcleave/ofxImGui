// created by 
// on 27

#pragma once
#include <map>
#include <string>
#include <vector>

#include "imgui.h"

#include "ofJson.h"
#include "ofPath.h"

struct Font
{
	int imFontIndex_m;

public:
	Font()
	{
	}


	int getImFontIndex()
	{
		return imFontIndex_m;
	}

	bool setup(const std::string& path_p, const float size_p)
	{
		if (ofFile::doesFileExist(path_p))
		{
			ImGuiIO* io = &ImGui::GetIO();
			io->Fonts->AddFontFromFileTTF(path_p.c_str(), size_p);
			imFontIndex_m = io->Fonts->Fonts.size() - 1;
			return true;
		}
		ofLogError(__FUNCTION__, "Couldn't load font %s. File doesn't exist", path_p.c_str());
		return false;
	}
};

class FontHandler
{
	static std::map<std::string, std::map<int, Font>> fontNameImFontMap_m;
	static std::map<std::string, std::string> fontNameFontPathMap_m;
	static int stepSize_m;
	static bool noBegin_m;

	static bool hasFont(const std::string& fontName_p)
	{
		return fontNameFontPathMap_m.find(fontName_p) != fontNameFontPathMap_m.end();
	}


	static bool findClosestFont(const std::string& fontName_p, const float lineHeight_p, int* font_p)
	{
		if (!hasFont(fontName_p))
		{
			ofLogError(__FUNCTION__, "Font %s is not setup", fontName_p.c_str());
			return false;
		}
		int minDistance = 10000000;
		ImGuiIO* io = &ImGui::GetIO();

		for (int i = 0; i < io->Fonts->Fonts.size(); i++)
		{
			const int currentDistance = abs(static_cast<int>(lineHeight_p - io->Fonts->Fonts[i]->FontSize));
			if (currentDistance < minDistance)
			{
				*font_p = i;
				minDistance = currentDistance;
			}
		}
		return true;
	}


	static int createFont(const std::string& fontName_p, const std::string& fontPath_p, const int fontSize_p)
	{
		//addFont
		if (!hasFont(fontName_p))
			return 0;

		if (fontNameImFontMap_m[fontName_p].find(fontSize_p) == fontNameImFontMap_m[fontName_p].end())
		{
			Font myFont;
			myFont.setup(fontPath_p, fontSize_p);
			fontNameImFontMap_m[fontName_p][fontSize_p] = myFont;
			return myFont.getImFontIndex();
		}
		return fontNameImFontMap_m[fontName_p][fontSize_p].getImFontIndex();
	}

	static void addFont(const std::string& fontPath_p, const int min_p, const int max_p)
	{
		const ofFile font(fontPath_p);
		fontNameFontPathMap_m[font.getBaseName()] = fontPath_p;
		for (int fontSize = min_p; fontSize <= max_p; fontSize += stepSize_m)
		{
			createFont(font.getBaseName(), fontPath_p, fontSize);
		}
	}


public:
	~FontHandler()
	{
		//TODO cleanup;
	}

	static void setup(const ofJson& json_p)
	{
		if (json_p.find("fonts") != json_p.end())
		{
			ofJson fontJson = json_p["fonts"];

			for (int i = 0; i < fontJson.size(); i++)
			{
				int min = 8;
				int max = 80;
				ofFile font;
				if (fontJson.at(i).find("min") != fontJson.at(i).end())
				{
					min = fontJson.at(i)["min"].get<int>();
				}
				if (fontJson.at(i).find("max") != fontJson.at(i).end())
				{
					max = fontJson.at(i)["max"].get<int>();
				}
				if (fontJson.at(i).find("src") != fontJson.at(i).end())
				{
					font = ofFile(ofToDataPath(fontJson.at(i)["src"].get<std::string>(), true));
					loadFont(font.getAbsolutePath(), min, max);
				}
				if (fontJson.at(i).find("isDefault") != fontJson.at(i).end() && fontJson.at(i).find("defaultSize") !=
					fontJson.at(i).end())
				{
					setDefault(font.getBaseName(), fontJson.at(i)["defaultSize"].get<int>());
				}
			}
		}
	}

	static void setDefault(const std::string& fontname_p, const int size_p)
	{
		if (!hasFont(fontname_p))
			return;

		ImGuiIO& io = ImGui::GetIO();
		int fontIndex = createFont(fontname_p, fontNameFontPathMap_m[fontname_p], size_p);
		io.FontDefault = io.Fonts->Fonts[fontIndex];
	}

	static void loadFont(const std::string& fontPath_p, const int min_p, const int max_p)
	{
		const ofFile font(fontPath_p);
		if (!hasFont(font.getBaseName()))
		{
			addFont(fontPath_p, min_p, max_p);
		}
	}

	static void BeginFontSize(const std::string& fontName_p, const float desiredLineHeight_p)
	{
		int font;

		if (!findClosestFont(fontName_p, desiredLineHeight_p, &font))
		{
			noBegin_m = true;
			return;
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGui::PushFont(io.Fonts->Fonts[font]);
		ImGui::SetWindowFontScale(desiredLineHeight_p / io.Fonts->Fonts[font]->FontSize);
		noBegin_m = false;
	}

	static void EndFontSize()
	{
		if (noBegin_m)
			return;
		ImGui::PopFont();
		ImGui::SetWindowFontScale(1.0f);
	}
};

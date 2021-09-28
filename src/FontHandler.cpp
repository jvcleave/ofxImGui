#include "FontHandler.h"


std::map<std::string, std::map<int, Font>> FontHandler::fontNameImFontMap_m;
std::map<std::string, std::string> FontHandler::fontNameFontPathMap_m;
bool FontHandler::noBegin_m = true;
int FontHandler::stepSize_m = 4;

#pragma once

#include "pch.h"

struct GlobalResources {

	std::string applicationName;
	std::string appdataPath;
	std::string optionsFileName;
	std::string optionsFilePath;
	std::string lockfilePath;

	sf::Image ArduinoIconImage;
	sf::Texture ArduinoIconTexture;
	int ArduinoIconSize;

};

extern std::unique_ptr<GlobalResources> RES;

void loadResources();
void releaseResources();

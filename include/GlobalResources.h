#pragma once

#include "pch.h"

struct GlobalResources {

	sf::Image ArduinoIconImage;
	sf::Texture ArduinoIconTexture;

};

extern std::unique_ptr<GlobalResources> RES;

void loadResources();
void releaseResources();

#pragma once

#include "pch.h"

struct GlobalResources {

	std::string applicationName;
	std::string rawAppdataPath;
	std::string appdataPath;
	std::string optionsFileName;
	std::string optionsFilePath;
	std::string lockfilePath;
	std::string autostartShortcutDir;

	sf::Image ArduinoIconImage;
	sf::Texture ArduinoIconTexture;
	int ArduinoIconSize;

	std::string TelegramApiToken;
	int feedbackChatID;

	std::string githubIssueUrl;
	std::string versionString;

};

extern std::unique_ptr<GlobalResources> RES;

void loadResources();
void releaseResources();

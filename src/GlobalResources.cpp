
#include "GlobalResources.h"
#include "resource.h"
#include "cpplocate/cpplocate.h"

std::unique_ptr<GlobalResources> RES;

void loadResources() {

    RES = std::make_unique<GlobalResources>();

    auto code = Battery::LoadEmbeddedResource(DB_ICON1, "PNG");
    if (!RES->ArduinoIconImage.loadFromMemory(&code[0], code.size()))
        throw Battery::Exception("Failed to load tray icon");

    if (!RES->ArduinoIconTexture.loadFromImage(RES->ArduinoIconImage))
        throw Battery::Exception("Failed to load icon texture");

    RES->ArduinoIconSize = 64;
    RES->applicationName = "ArduinoByteReverser";
    RES->appdataPath = cpplocate::localDir(RES->applicationName);
    RES->rawAppdataPath = Battery::GetParentDirectory(RES->appdataPath);
    RES->optionsFileName = "options.xml";
    RES->optionsFilePath = RES->appdataPath + cpplocate::pathSeparator() + RES->optionsFileName;
    RES->lockfilePath = RES->appdataPath + cpplocate::pathSeparator() + "lock";

    RES->autostartShortcutDir = RES->rawAppdataPath + "/Microsoft/Windows/Start Menu/Programs/Startup";

    RES->TelegramApiToken = "5468665543:AAHgLsdbf1lSboxxrtcPAbtPFimjqQryVSc";
    RES->feedbackChatID = 729235222;
}

void releaseResources() {
    RES.reset();
}

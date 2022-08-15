
#include "GlobalResources.h"
#include "resource.h"
#include "version.h"
#include "cpplocate/cpplocate.h"

std::unique_ptr<GlobalResources> RES;

void loadResources() {

    RES = std::make_unique<GlobalResources>();

    auto bytes24 = Battery::LoadEmbeddedResource(DB_ARDUINOICON24, "PNG");
    if (!RES->ArduinoIconImage24.loadFromMemory(&bytes24[0], bytes24.size()))
        throw Battery::Exception("Failed to load Arduino icon");

    if (!RES->ArduinoIconTexture24.loadFromImage(RES->ArduinoIconImage24))
        throw Battery::Exception("Failed to load Arduino icon texture");

    auto bytes64 = Battery::LoadEmbeddedResource(DB_ARDUINOICON64, "PNG");
    if (!RES->ArduinoIconImage64.loadFromMemory(&bytes64[0], bytes64.size()))
        throw Battery::Exception("Failed to load tray icon");

    if (!RES->ArduinoIconTexture64.loadFromImage(RES->ArduinoIconImage64))
        throw Battery::Exception("Failed to load tray icon texture");

    RES->applicationName = "ArduinoByteReverser";
    RES->appdataPath = cpplocate::localDir(RES->applicationName);
    RES->rawAppdataPath = Battery::GetParentDirectory(RES->appdataPath);
    RES->optionsFileName = "options.xml";
    RES->optionsFilePath = RES->appdataPath + cpplocate::pathSeparator() + RES->optionsFileName;
    RES->lockfilePath = RES->appdataPath + cpplocate::pathSeparator() + "lock";

    RES->autostartShortcutDir = RES->rawAppdataPath + "/Microsoft/Windows/Start Menu/Programs/Startup";

    RES->TelegramApiToken = "5468665543:AAHgLsdbf1lSboxxrtcPAbtPFimjqQryVSc";
    RES->feedbackChatID = 729235222;

    RES->githubIssueUrl = "https://github.com/HerrNamenlos123/bytereverse/issues/new";
    RES->versionString = std::to_string(BYTEREVERSE_VERSION_MAJOR) + "." + std::to_string(BYTEREVERSE_VERSION_MINOR) + "." + std::to_string(BYTEREVERSE_VERSION_PATCH);
}

void releaseResources() {
    RES.reset();
}

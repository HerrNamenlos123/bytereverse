
#include "GlobalResources.h"
#include "resource.h"

std::unique_ptr<GlobalResources> RES;

void loadResources() {

    RES = std::make_unique<GlobalResources>();

    auto code = Battery::LoadEmbeddedResource(DB_ICON1, "PNG");
    if (!RES->ArduinoIconImage.loadFromMemory(&code[0], code.size()))
        throw Battery::Exception("Failed to load tray icon");

    if (!RES->ArduinoIconTexture.loadFromImage(RES->ArduinoIconImage))
        throw Battery::Exception("Failed to load icon texture");
}

void releaseResources() {
    RES.reset();
}

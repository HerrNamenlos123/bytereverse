#pragma once

#include "pch.h"

#include "Battery/Fonts/RobotoMedium.h"
#include "Battery/Fonts/FontAwesomeWebfont.h"

struct Fonts {

    inline static ImFont* robotoOptions = nullptr;
    inline static ImFont* robotoOptionsPopups = nullptr;
    inline static ImFont* robotoWorker = nullptr;
    inline static ImFont* robotoSignature = nullptr;
    inline static ImFont* fontAwesomeHugeCheckMark = nullptr;
    inline static ImFont* fontAwesomeSignature = nullptr;
    inline static ImFont* fontAwesomeOptions = nullptr;

    static void load() {

        int optionsSize = 21;
        robotoOptions = ADD_FONT(RobotoMedium, optionsSize);
        fontAwesomeOptions = ADD_ICON_FONT(FontAwesomeWebfont, optionsSize);

        robotoOptionsPopups = ADD_FONT(RobotoMedium, 18);
        
        int signatureSize = 15;
        robotoSignature = ADD_FONT(RobotoMedium, 15);
        fontAwesomeSignature = ADD_ICON_FONT(FontAwesomeWebfont, 15);

        robotoWorker = ADD_FONT(RobotoMedium, 17);
        fontAwesomeHugeCheckMark = ADD_ICON_FONT(FontAwesomeWebfont, 32);
    }

};

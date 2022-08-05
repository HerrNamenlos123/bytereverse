#pragma once

#include "pch.h"

#include "Battery/Fonts/RobotoMedium.h"
#include "Battery/Fonts/FontAwesomeWebfont.h"

struct Fonts {

    inline static ImFont* robotoMedium = nullptr;
    inline static ImFont* robotoSmall = nullptr;
    inline static ImFont* fontAwesome = nullptr;

    static void load() {
        robotoMedium = ADD_FONT(RobotoMedium, 25);
        robotoSmall = ADD_FONT(RobotoMedium, 17);
        fontAwesome = ADD_ICON_FONT(FontAwesomeWebfont, 32);
    }

};

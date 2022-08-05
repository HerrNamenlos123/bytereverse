
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

class BatteryApp : public Battery::Application {
public:
    BatteryApp() : panel([&] { OnUIUpdate(); }, [&] { OnUIRender(); }) {}

    std::unique_ptr<Battery::TrayIcon> tray;
    Battery::ImGuiPanel panel;
    sf::Texture iconTX;

    void OnUIUpdate();
    void OnUIRender();

    void OnStartup() override;
    void OnUpdate() override;
    void OnRender() override;
    void OnShutdown() override;
    void OnEvent(sf::Event event, bool& handled) override;

    void MoveWindow(int dx, int dy);
    void SetupWorkWindow();
    void SetupHomeWindow();
    void OnLeftClick();
    void OnRightClick();
};

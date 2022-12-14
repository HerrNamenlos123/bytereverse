
#include "pch.h"
#include "Fonts.h"

#include "WorkerUI.h"
#include "OptionsUI.h"
#include "TelegramBot.h"

class BatteryApp : public Battery::Application {
public:
    BatteryApp() {}

    std::unique_ptr<Battery::Lockfile> lockfile;
    std::unique_ptr<Battery::TrayIcon> tray;
    sf::Texture iconTX;

    std::unique_ptr<_BaseUI> activeUI;

    void OnUIUpdate();
    void OnUIRender();

    void OnStartup() override;
    void OnUpdate() override;
    void OnRender() override;
    void OnShutdown() override;
    void OnEvent(sf::Event event, bool& handled) override;

    bool ReadyForNewUI();
    void OnLeftClick();
    void OnRightClick();
};

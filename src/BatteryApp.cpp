
#include "pch.h"
#include "BatteryApp.h"
#include "GlobalResources.h"
#include "Shortcut.h"

bool installApplication() {
	// Return true if the application has to be restarted
	
    auto installPath = Battery::GetLocalInstallDir() + "/" + Battery::GetExecutableName();
	auto currentPath = Battery::GetExecutablePath();
	
    if (std::filesystem::path(currentPath) == std::filesystem::path(installPath)) {
        return false;   // Already installed
    }
	
    LOG_WARN("Executable is not in the local app directory! Installing application now");
    LOG_DEBUG("currentPath = {}", std::filesystem::path(currentPath).string());
	LOG_DEBUG("installPath = {}", std::filesystem::path(installPath).string());

    Battery::PrepareDirectory(Battery::GetParentDirectory(installPath));
    Battery::RemoveFile(installPath);       // If it exists
    if (!Battery::CopyFile(currentPath, installPath)) {
        LOG_ERROR("Failed to copy executable to local install directory");
        Battery::MessageBoxError("Failed to install the application. Please try again.");
        return true;
    }

	// Start application again from install location
    Battery::ExecuteShellCommand("start " + installPath);
    Battery::MessageBoxInfo("Successfully installed application! Please click the tray icon in your taskbar to start converting.");
    return true;
}

void BatteryApp::OnStartup() {

    window.setVisible(false);
    window.setTitle("ByteReverser utility");
    window.setFramerateLimit(60);
    Battery::GetApp().applicationName = "ArduinoByteReverser";

    if (installApplication()) {
        CloseApplication();
        return;
    }

    loadResources();
    OptionsFile::loadOptions();
    Battery::PrepareDirectory(RES->appdataPath);
    std::filesystem::current_path(RES->appdataPath);

    Fonts::load();

    try {
        lockfile = std::make_unique<Battery::Lockfile>(RES->lockfilePath);
    }
    catch (const Battery::LockfileUnavailableException&) {
        Battery::MessageBoxError("Another instance of ArduinoByteReverser is already running");
        CloseApplication();
        return;
    }

    Battery::SetWindowIcon(window, RES->ArduinoIconImage64.getSize().x, RES->ArduinoIconImage64);
    
    tray = std::make_unique<Battery::TrayIcon>(RES->ArduinoIconImage24, "ByteReverser: Quick-convert");
    tray->attachLeftClickCallback([&] { OnLeftClick(); });
    tray->attachRightClickCallback([&] { OnRightClick(); });

    registerNewInstance();
}

void BatteryApp::OnUpdate() {
    tray->update();

    if (activeUI) {
        activeUI->update();
        if (activeUI->shouldBeClosed) {
            LOG_DEBUG("activeUI requested to be closed, hiding window");
            activeUI.reset();
            window.setVisible(false);
            Battery::SetWindowTransparent(window, false);
        }
        SetWindowStandby(false);
    }
    else {
        SetWindowStandby(true);
    }

}

void BatteryApp::OnRender() {
    if (activeUI) activeUI->render();
}

void BatteryApp::OnShutdown() {
    activeUI.reset();
    releaseResources();
}

void BatteryApp::OnEvent(sf::Event event, bool& handled) {

    static int oldx = -1;
    static int oldy = -1;
    int dx = 0;
    int dy = 0;

    if (event.type == sf::Event::MouseMoved) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (oldx != -1) {
                if (event.mouseMove.x != oldx) {
                    dx = event.mouseMove.x - oldx;
                }
            }
            else {
                oldx = event.mouseMove.x;
            }
            if (oldy != -1) {
                if (event.mouseMove.y != oldy) {
                    dy = event.mouseMove.y - oldy;
                }
            }
            else {
                oldy = event.mouseMove.y;
            }
            if (dx != 0 || dy != 0) {
                if (activeUI) {
                    activeUI->moveWindowDelta(dx, dy);
                }
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        oldx = -1;
        oldy = -1;
    }

    if (activeUI) {
        activeUI->onEvent(event);
    }
}

bool BatteryApp::ReadyForNewUI() {

    if (activeUI) {
        if (activeUI->canBeClosed)      // If there is another one which can be closed
            return true;
    }
    else {
        return true;                    // If there is no UI
    }

    return false;
}

void BatteryApp::OnLeftClick() {

    if (ReadyForNewUI()) {

        if (optionsFile.profiles.size() == 0) {
            Battery::MessageBoxError("There is no profile active. Please right-click the tray icon and create a new one.");
            return;
        }

        if (optionsFile.activeProfile >= optionsFile.profiles.size()) {
            optionsFile.activeProfile = optionsFile.profiles.size() - 1;
            LOG_WARN("The profile index was invalid, choosing to the last profile");
        }

        auto& profile = optionsFile.profiles[optionsFile.activeProfile];

        LOG_DEBUG("Instantiating WorkerUI");
        activeUI.reset();
        activeUI = std::make_unique<WorkerUI>(profile.name, profile.sourcePath, profile.targetPath);
        activeUI->setupWindow();
    }
    else {
        LOG_WARN("WorkerUI is blocked by another UI");
    }
}

void BatteryApp::OnRightClick() {

    if (ReadyForNewUI()) {
        LOG_DEBUG("Instantiating OptionsUI");
        activeUI.reset();
        activeUI = std::make_unique<OptionsUI>();
        activeUI->setupWindow();
    }
    else {
        LOG_WARN("OptionsUI is blocked by another UI");
    }
}

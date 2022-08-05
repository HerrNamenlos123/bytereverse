
#include "pch.h"
#include "BatteryApp.h"
#include "GlobalResources.h"

#define DEFAULT_WORK_WINDOW_WIDTH 300
#define DEFAULT_WORK_WINDOW_HEIGHT 80

void BatteryApp::OnStartup() {

    //window.setFramerateLimit(30);

    window.setVisible(false);
    loadResources();

    tray = std::make_unique<Battery::TrayIcon>(RES->ArduinoIconImage, "Quick-convert for the current profile");
    tray->attachLeftClickCallback([&] { OnLeftClick(); });
    tray->attachRightClickCallback([&] { OnRightClick(); });

    Fonts::load();
}

void BatteryApp::OnUpdate() {
    tray->update();

    if (activeUI) {
        activeUI->update();
        if (activeUI->shouldBeClosed) {
            activeUI.reset();
        }
    }

}

void BatteryApp::OnRender() {
    if (activeUI) activeUI->render();
}

void BatteryApp::OnShutdown() {
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
                //if (ImGui::GetIO().WantCaptureMouse) {
                    //MoveWindow(dx, dy);
                //}
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        oldx = -1;
        oldy = -1;
    }
}

void BatteryApp::OnLeftClick() {

    bool instantiate = false;
    if (activeUI) if (activeUI->canBeClosed) instantiate = true;
    if (!activeUI) instantiate = true;

    if (instantiate) {
        activeUI.reset();
        activeUI = std::make_unique<WorkerUI>();
        activeUI->setupWindow(DEFAULT_WORK_WINDOW_WIDTH, DEFAULT_WORK_WINDOW_HEIGHT);
    }
}

void BatteryApp::OnRightClick() {

    bool instantiate = false;
    if (activeUI) if (activeUI->canBeClosed) instantiate = true;
    if (!activeUI) instantiate = true;

    if (instantiate) {
        /*activeUI.reset();
        activeUI = std::make_unique<OptionsUI>();
        activeUI->setupWindow(DEFAULT_WORK_WINDOW_WIDTH, DEFAULT_WORK_WINDOW_HEIGHT);*/
    }
    CloseApplication();
}

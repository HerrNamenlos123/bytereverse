
#include "pch.h"
#include "BatteryApp.h"
#include "resource.h"

#include "windows.h"

std::atomic<bool> visible = false;
std::atomic<bool> fadeout = false;
std::atomic<bool> workDone = true;
float fadeStartTime = 0;
float fadeTime = 1.f;
float alpha = 0;
std::thread workerThread;

#define DEFAULT_WORK_WINDOW_WIDTH 300
#define DEFAULT_WORK_WINDOW_HEIGHT 80

bool doWork() {

    Battery::Sleep(2);

    fadeout = true;
    fadeStartTime = Battery::GetRuntime().asSeconds();
    alpha = 255;
    workDone = true;
}

static void Spinner(const char* label, double radius, int thickness, const ImU32& color) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return;

    // Render
    window->DrawList->PathClear();

    int num_segments = 30;
    int start = abs(std::sin(Battery::GetRuntime().asSeconds() * 1.8) * (num_segments - 5.0));

    double a_min = IM_PI * 2.0 * ((double)start) / (float)num_segments;
    double a_max = IM_PI * 2.0 * ((double)num_segments - 3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

    for (int i = 0; i < num_segments; i++) {
        double a = a_min + ((double)i / (double)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(ImVec2(
            (double)centre.x + std::cos(a + Battery::GetRuntime().asSeconds() * 14.0) * radius,
            (double)centre.y + std::sin(a + Battery::GetRuntime().asSeconds() * 14.0) * radius));
    }

    window->DrawList->PathStroke(color, false, thickness);
}

void BatteryApp::OnUIUpdate() {
    panel.position = { 0, 0 };
    panel.size = { window.getSize().x, window.getSize().y };

    if (workDone) {
        float timeSinceFadeStart = Battery::GetRuntime().asSeconds() - fadeStartTime;
        float al = Battery::Map(timeSinceFadeStart, 0.f, fadeTime, 255.f, 0.f);
        if (timeSinceFadeStart < fadeTime) {
            Battery::SetWindowAlpha(window, al);
        }
        else {
            Battery::SetWindowAlpha(window, 0);
        }
    }
    else {
        Battery::SetWindowAlpha(window, 255);
    }
}

void BatteryApp::OnUIRender() {

    std::string profile = "<ProFiLeNaMe>";
    std::string source = "<SoUrCeFiLe>";
    std::string target = "<TaRgEtFiLe>";

    int offset = ImGui::GetStyle().WindowPadding.y;
    ImGui::Columns(2, 0, false);
    ImGui::SetColumnWidth(0, ImGui::GetWindowHeight() - offset);
    ImGui::Image((ImTextureID)iconTX.getNativeHandle(), ImVec2(ImGui::GetWindowHeight() - offset * 2, ImGui::GetWindowHeight() - offset * 2));
    ImGui::NextColumn();
    ImGui::PushFont(Fonts::robotoSmall);
    ImGui::Text("Profile: %s", profile.c_str());
    ImGui::Text("Source: %s", source.c_str());
    ImGui::Text("Target: %s", target.c_str());
    ImGui::EndColumns();

    int max = 0;
    ImVec2 p = ImGui::CalcTextSize(profile.c_str());
    max = std::max<int>(max, p.x);
    p = ImGui::CalcTextSize(source.c_str());
    max = std::max<int>(max, p.x);
    p = ImGui::CalcTextSize(target.c_str());
    max = std::max<int>(max, p.x);

    int width = std::max<int>(DEFAULT_WORK_WINDOW_WIDTH, max + ImGui::GetWindowHeight() * 2 + 50);
    if (width != window.getSize().x)
        window.setSize({ (uint16_t)width, window.getSize().y });

    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 58);
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - 10 - offset);

    if (!workDone) {
        Spinner("Loading", 16, 6, 0xFF00FF00);
    }
    else {
        ImGui::PushFont(Fonts::fontAwesome);
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "\xef\x80\x8c");	// Green check mark
        ImGui::PopFont();
    }

    ImGui::PopFont();
}

void BatteryApp::OnStartup() {
    using namespace Battery;

    window.setVisible(false);
    Battery::SetWindowTransparent(window, true);

    // Load tray icon
    sf::Image icon;
    auto code = Battery::LoadEmbeddedResource(DB_ICON1, "PNG");
    if (!icon.loadFromMemory(&code[0], code.size()))
        throw Battery::Exception("Failed to load tray icon");

    tray = std::make_unique<Battery::TrayIcon>(icon, "Quick-convert for the current profile");

    tray->attachLeftClickCallback([&] { OnLeftClick(); });
    tray->attachRightClickCallback([&] { OnRightClick(); });

    if (!iconTX.loadFromImage(icon)) throw Battery::Exception("Failed to load icon texture");

    Fonts::load();

}

void BatteryApp::OnUpdate() {
    using namespace Battery;

    tray->update();
    panel.update();

    //LOG_ERROR("X={} Y={}", sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
}

void BatteryApp::OnRender() {
    using namespace Battery;

    panel.render();
    window.clear();
    //window.clear(sf::Color::Transparent);
}

void BatteryApp::OnShutdown() {
    using namespace Battery;

    if (workerThread.joinable())
        workerThread.join();
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
                    MoveWindow(dx, dy);
                //}
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        oldx = -1;
        oldy = -1;
    }
}

void BatteryApp::MoveWindow(int dx, int dy) {

    auto pos = window.getPosition() + sf::Vector2i(dx, dy);

    auto desktop = Battery::GetUsableDesktopArea();
    pos.x = std::max<int>(0, pos.x);
    pos.x = std::min<int>(pos.x, desktop.x - window.getSize().x);
    pos.y = std::max<int>(0, pos.y);
    pos.y = std::min<int>(pos.y, desktop.y - window.getSize().y);

    window.setPosition(pos);
}

void BatteryApp::SetupWorkWindow() {

    if (!workDone)
        return;

    if (workerThread.joinable())
        workerThread.join();

    window.setSize({ DEFAULT_WORK_WINDOW_WIDTH, DEFAULT_WORK_WINDOW_HEIGHT });

    // First center the window
    auto pos = sf::Mouse::getPosition();
    pos.x -= window.getSize().x / 2;
    pos.y -= window.getSize().y / 2;

    // And now cap it to the desktop size
    auto desktop = Battery::GetUsableDesktopArea();
    pos.x = std::max<int>(0, pos.x);
    pos.x = std::min<int>(pos.x, desktop.x - window.getSize().x);
    pos.y = std::max<int>(0, pos.y);
    pos.y = std::min<int>(pos.y, desktop.y - window.getSize().y);

    window.setPosition(pos);
    window.setVisible(true);

    visible = true;
    workerThread = std::thread(doWork);
    workDone = false;
}

void BatteryApp::SetupHomeWindow() {

}

void BatteryApp::OnLeftClick() {
    SetupWorkWindow();
}

void BatteryApp::OnRightClick() {
    SetupHomeWindow();
    CloseApplication();
}

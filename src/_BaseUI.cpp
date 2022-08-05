
#include "pch.h"
#include "_BaseUI.h"

void _BaseUI::setupWindow(uint16_t width, uint16_t height, uint8_t alpha) {
    auto& window = Battery::GetApp().window;
    originalSize = { width, height };

    // Center the window
    auto pos = sf::Mouse::getPosition();
    pos.x -= width / 2;
    pos.y -= height / 2;

    // Cap the position within the usable desktop size (above taskbar)
    auto desktop = Battery::GetUsableDesktopArea();
    pos.x = std::max<int>(0, pos.x);
    pos.x = std::min<int>(pos.x, desktop.x - width);
    pos.y = std::max<int>(0, pos.y);
    pos.y = std::min<int>(pos.y, desktop.y - height);

    window.setSize({ width, height });
    window.setPosition(pos);
    window.setVisible(true);

    if (alpha == 255) {
        Battery::SetWindowTransparent(window, false);
    }
    else {
        Battery::SetWindowTransparent(window, true);
        Battery::SetWindowAlpha(window, alpha);
    }
}

void _BaseUI::moveWindowDelta(int dx, int dy) {
    auto& window = Battery::GetApp().window;

    auto pos = window.getPosition() + sf::Vector2i(dx, dy);

    auto desktop = Battery::GetUsableDesktopArea();
    pos.x = std::max<int>(0, pos.x);
    pos.x = std::min<int>(pos.x, desktop.x - window.getSize().x);
    pos.y = std::max<int>(0, pos.y);
    pos.y = std::min<int>(pos.y, desktop.y - window.getSize().y);

    window.setPosition(pos);
}

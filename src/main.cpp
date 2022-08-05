
#include "pch.h"
#include "BatteryApp.h"

int main(int argc, const char** argv) {
    BatteryApp app;
    app.Run(0, 0, argc, argv, sf::Style::None);
}

/*
#include "windows.h"

int main()
{
    // First, I load the logo and create an sprite
    sf::Texture logo;

    if (!logo.loadFromFile("monopoly.png")) {
        exit(1);
    }

    sf::Sprite sp;
    sp.setTexture(logo);
    sp.scale({ 0.2f, 0.2f }); // My logo is quite big in fact (1st google result btw)

    int logoWidth = sp.getGlobalBounds().width;
    int logoHeight = sp.getGlobalBounds().height;

    // With the logo loaded, I know its size, so i create the window accordingly
    sf::RenderWindow window(sf::VideoMode({ (uint16_t)logoWidth, (uint16_t)logoHeight }), "SFML", sf::Style::None); // <- Important!! Style=None removes title

    // To close splash window by timeout, I just suppose you want something like this, don't you?
    sf::Clock timer;
    sf::Time time = timer.restart();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // event loop is always needed
        }
        // Window closed by time
        time += timer.restart();
        if (time >= sf::seconds(0.5f)) {
            window.close();
        }

        window.clear();
        window.draw(sp);
        window.display();
    }


    // Then, reuse the window and do things again
    window.create(sf::VideoMode({ 600, 400 }), "SFML", sf::Style::None);

    SetWindowLongPtr(window.getSystemHandle(), GWL_EXSTYLE, GetWindowLongPtr(window.getSystemHandle(), GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(window.getSystemHandle(), 0, 255, LWA_ALPHA);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

        }

        window.clear(sf::Color(0, 0, 0, 100));
        window.draw(sp);
        window.display();
    }

    return 0;
}
/*

#include <SFML/Graphics.hpp>
#include <Windows.h>

// include those functions here
int main()
{
    const sf::Color windowColor{ sf::Color(255, 0, 0, 192) };
    const sf::Color windowColorGrabbed{ sf::Color(255, 255, 0, 64) };

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "", sf::Style::None);
    makeWindowTransparent(window); // allow window to be transparent

    sf::Vector2i grabbedOffset;
    bool grabbedWindow = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    grabbedOffset = window.getPosition() - sf::Mouse::getPosition();
                    grabbedWindow = true;
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    grabbedWindow = false;
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                if (grabbedWindow)
                    window.setPosition(sf::Mouse::getPosition() + grabbedOffset);
            }
        }
        sf::Color clearColor{ windowColor };
        if (grabbedWindow)
            clearColor = windowColorGrabbed;
        setWindowAlpha(window, clearColor.a); // change transparency of window
        window.clear(clearColor);
        window.display();
    }
}*/
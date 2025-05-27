#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

std::string const WINDOW_TITLE = "Con-graduate-lations!";
constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;
constexpr int TARGET_FPS = 60;
constexpr int ANTIALIAS_LEVEL = 8;

// Window Settings
inline void setupWindow(sf::RenderWindow& window)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIALIAS_LEVEL;

    window.create(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Default, settings);

    auto const desktop = sf::VideoMode::getDesktopMode();
    auto windowSize = window.getSize();
    window.setPosition({ static_cast<int>(desktop.width / 2 - windowSize.x / 2),
        static_cast<int>(desktop.height / 2 - windowSize.y / 2) });

    window.setFramerateLimit(TARGET_FPS);

    auto applied = window.getSettings();
    std::cout << "Anti-Aliasing: " << applied.antialiasingLevel << "x\n";
}

// Fonts
std::string const FONT_FILE = "assets/fonts/Celsius Flower.ttf";

// Music
std::string const FILE_MUSIC = "assets/music/Remember_Summer_Days.flac";

// Sprites
std::string const IMAGE_SUN = "assets/images/terrasun.png";
std::string const IMAGE_CLOUD = "assets/images/cloud.png";
std::string const TWILIGHT_SPARKLE = "assets/images/twilight_sparkle_graduate.png";

#ifndef CS110_CAT_MOUSE_GAME_H
#define CS110_CAT_MOUSE_GAME_H

#include <SFML/Graphics.hpp>
#include "GL.h"

namespace Game {
    struct Background {
        sf::Sprite image;
        sf::Texture texture;
        void setBG(const std::string& path, sf::RenderWindow& window){
            texture.setSmooth(true);
            texture.setSrgb(true);
            texture.loadFromFile(path);
            image.setTexture(texture);
            image.setScale((float) window.getSize().x / (float) texture.getSize().x,
                           (float) window.getSize().y / (float) texture.getSize().y);
        }
    };
    struct State {
        sf::VideoMode windowMode = sf::VideoMode().getDesktopMode();
        std::string windowTitle = "Mouse and Cat meow meow!";
        sf::Uint32 windowStyle = sf::Style::Default;
        bool verticalSync = false;
        unsigned int frameLimit = 90;
        sf::ContextSettings contextSettings;
        bool active = true;
        State() {
            setContextSettings(24, 16, true, 8);
        }
        void setContextSettings(unsigned int depthBits,unsigned int AL, bool sRgb,unsigned int stencilBits) {
            contextSettings.depthBits = depthBits;
            contextSettings.antialiasingLevel = AL;
            contextSettings.sRgbCapable = sRgb;
            contextSettings.stencilBits = stencilBits;
        }
        State(const State &) = delete;
        State &operator=(const State &) = delete;
    };
    void Init();
    void RenderWindow();
    void RenderFrame();
    void Run();
    extern State gameState;
    extern sf::Clock clock;
    extern sf::Time timePerFrame;
    extern sf::RenderWindow window;
    extern Background background;
    extern int gameMode;
    extern int currentEntity;
};


#endif //CS110_CAT_MOUSE_GAME_H

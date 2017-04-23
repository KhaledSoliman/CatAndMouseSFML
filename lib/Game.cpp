#include "Game.h"
#include "GUI.h"

Game::State Game::gameState;
sf::RenderWindow Game::window;
sf::Clock Game::clock;
sf::Time Game::timePerFrame;
Game::Background Game::background;
int Game::gameMode;
int Game::currentEntity;

void Game::Init() {
    background.setBG("../assets/common/bg-1.jpg", window);
}

void Game::RenderWindow() {
    window.create(gameState.windowMode, gameState.windowTitle, gameState.windowStyle,
                  gameState.contextSettings);
    window.setVerticalSyncEnabled(gameState.verticalSync);
    window.setFramerateLimit(gameState.frameLimit);
    window.setActive(gameState.active);
}

void Game::RenderFrame() {
    clock.restart();
    window.clear();
    window.draw(background.image);
    GUI::Render(window);
    GL::Render(window);
    window.display();
    timePerFrame = clock.getElapsedTime();
}

void Game::Run() {
    RenderWindow();
    Init();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                bool found = false;
                for (auto element: GUI::menus)
                    if (element.second->isActive()){
                        found = element.second->clickScan(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
                        if(found) break;
                    }
                while (window.pollEvent(event));
            } else if (event.type == sf::Event::MouseMoved) {
                for (auto element: GUI::menus)
                    if (element.second->isActive())
                        element.second->hoveringScan(
                                window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
            } else if (event.type == sf::Event::KeyPressed) {
                bool flag;
                    switch (event.key.code) {
                        case sf::Keyboard::W:
                            GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::North);
                            flag = true;
                            break;
                        case sf::Keyboard::D:
                            GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::East);
                            flag = true;
                            break;
                        case sf::Keyboard::A:
                            GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::West);
                            flag = true;
                            break;
                        case sf::Keyboard::S:
                            GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::South);
                            flag = true;
                            break;
                        default:
                            flag = false;
                            break;
                    }
                if(flag){
                    if(gameMode){
                        currentEntity  = 1 - currentEntity;
                    }else{
                        GL::world.moveEntity(GL::EntityType::Cat, static_cast<GL::Direction>(rand() % 4));
                    }
                }
            }
            RenderFrame();
        }
    }
}
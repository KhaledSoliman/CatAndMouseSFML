#include "triggers.h"
#include "GUI.h"
#include "Game.h"

void triggers::quit(){
   Game::window.close();

}

void triggers::startGame() {
    GUI::menus[GUI::Menus::Main]->setActive(false);
    GUI::menus[GUI::Menus::Modes]->setActive(true);
}

void triggers::back() {
    GUI::menus[GUI::Menus::Modes]->setActive(false);
    GUI::menus[GUI::Menus::Main]->setActive(true);
}

void triggers::soloPlay() {
    GUI::menus[GUI::Menus::Modes]->setActive(false);
    Game::gameMode = 0;
    Game::currentEntity = 0;
    GL::world.setActive(true);
}

void triggers::multiPlay(){
    GUI::menus[GUI::Menus::Modes]->setActive(false);
    Game::gameMode = 1;
    Game::currentEntity = 0;
    GL::world.setActive(true);
}

void triggers::restartGame() {
    GUI::menus[GUI::Menus::EndGame]->setActive(false);
    GL::world.populateWorld();
    GL::world.setActive(true);
}

void triggers::backToMain() {
    GUI::menus[GUI::Menus::EndGame]->setActive(false);
    GUI::menus[GUI::Menus::Main]->setActive(true);
}
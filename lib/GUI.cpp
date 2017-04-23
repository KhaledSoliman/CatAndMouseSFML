#include "GUI.h"
#include "triggers.h"
#include "Game.h"

std::map<GUI::Menus, GUI::Menu*> GUI::menus;
std::map<std::string, std::function<void()>> GUI::triggers;
std::map<std::string, sf::Font*> GUI::fonts;
std::vector<sf::Text*> GUI::texts;

void GUI::Init() {
    sf::Font* pFont = new sf::Font;
    pFont->loadFromFile("../assets/fonts/PoseiAOE.ttf");
    fonts["PoseiAOE"] = pFont;
    triggers["Quit"] = triggers::quit;
    triggers["New Game"] = triggers::startGame;
    triggers["Back"] = triggers::back;
    triggers["1 Player"] = triggers::soloPlay;
    triggers["2 Players"] = triggers::multiPlay;
    triggers["Restart"] = triggers::restartGame;
    triggers["Back to Main Menu"] = triggers::backToMain;
    createMainMenu();
    createModeMenu();
    createEndGameMenu();
}

void GUI::Render(sf::RenderWindow &window) {
    for (auto element: texts) {
        window.draw(*element);
    }
    for (auto element: menus) {
        element.second->draw(window);
    }
}

void GUI::addText(const std::string &text, const std::string &font, unsigned int charSize, const sf::Color &color,
                  const sf::Vector2f &pos) {
    sf::Text* pText = new sf::Text(text, *fonts[font], charSize);
    pText->setFillColor(color);
    pText->setPosition(pos);
    texts.push_back(pText);
}

void GUI::createMainMenu() {
    Menu* pMenu = new Menu;
    pMenu->setActive(true);
    pMenu->setText("sansation.ttf", 50, 0xFFFFFFFF);
    pMenu->setHover("Main_Menu_Hover_Effect.wav", 0xFF0000FF);
    pMenu->setClick("Main_Menu_Click_Effect.wav", 0xFFFF00FF);
    pMenu->setInc(sf::Vector2f(0.f, 100.f));
    pMenu->setPosition(sf::Vector2f(200.f, 250.f));
    pMenu->addButton("New Game");
    pMenu->addButton("Quit");
    pMenu->createBackground();
    menus[Menus::Main] = pMenu;
}

void GUI::createModeMenu(){
    Menu* pMenu = new Menu;
    pMenu->setActive(false);
    pMenu->setText("sansation.ttf", 50, 0xFFFFFFFF);
    pMenu->setHover("Main_Menu_Hover_Effect.wav", 0xFF0000FF);
    pMenu->setClick("Main_Menu_Click_Effect.wav", 0xFFFF00FF);
    pMenu->setInc(sf::Vector2f(0.f, 100.f));
    pMenu->setPosition(sf::Vector2f(-1800.f, 150.f));
    pMenu->addButton("1 Player");
    pMenu->addButton("2 Players");
    pMenu->addButton("Back");
    pMenu->createBackground();
    menus[Menus::Modes] = pMenu;
}

void GUI::createEndGameMenu() {
    Menu* pMenu = new Menu;
    pMenu->setActive(false);
    pMenu->setText("sansation.ttf", 50, 0xFFFFFFFF);
    pMenu->setHover("Main_Menu_Hover_Effect.wav", 0xFF0000FF);
    pMenu->setClick("Main_Menu_Click_Effect.wav", 0xFFFF00FF);
    pMenu->setInc(sf::Vector2f(0.f, 100.f));
    pMenu->setPosition(sf::Vector2f(-1600.f, 300.f));
    pMenu->addButton("Restart");
    pMenu->addButton("Back to Main Menu");
    pMenu->addButton("Quit");
    pMenu->createBackground();
    menus[Menus::EndGame] = pMenu;
}

void GUI::Destroy() {
    for(auto element: menus)
        delete element.second;
    for(auto element: texts)
        delete element;
    for(auto element: fonts)
        delete element.second;
}

GUI::Button::Button() : Text(), state(disabled), clickAble(false) {}

GUI::Button::Button(const std::string &value, const sf::Font &font, const sf::Color &color, unsigned int charSize)
        : Text(value, font, charSize), state(enabled), clickAble(false) {
    setFillColor(color);
};

void GUI::Button::setState(int newState) {
    state = newState;
}

int GUI::Button::getState() const {
    return state;
}

bool GUI::Button::isMouseOver(const sf::Vector2f &pos) const {
    return getGlobalBounds().contains(pos);
}

void GUI::Button::bindFunction(std::function<void()> trigger) {
    callBack = trigger;
    clickAble = true;
}

void GUI::Button::trigger() {
    if(clickAble)
        callBack();
}

void GUI::Menu::addButton(const std::string &value) {
    Button *pButton = new Button(value, menuFont, defaultColor, charSize);
    pButton->setPosition(currentPosition);
    buttons.insert(pButton);
    if (triggers.find(value) != triggers.end())
        pButton->bindFunction(triggers[value]);
    currentPosition += incVector;
}

void GUI::Menu::addText(const std::string &value) {
    sf::Text *pText = new sf::Text(value,menuFont,charSize);
    pText->setFillColor(defaultColor);
    pText->setPosition(currentPosition);
    texts.insert(pText);
    currentPosition += incVector;
}

void GUI::Menu::draw(sf::RenderWindow &window) const {
    if (active){
        window.draw(background);
        for (auto button: buttons) {
            window.draw(*button);
        }
        for (auto text: texts) {
            window.draw(*text);
        }
    }

}

void GUI::Menu::setActive(bool flag) {
    if(active){
        sf::Time totalTime = sf::seconds(0.0f);
        sf::Time time = sf::seconds(0.5f);
        sf::Vector2f offsetVector = sf::Vector2f(-2000.f,0.f);
        while(totalTime < time){
            for(auto element: texts)
                element->move((Game::timePerFrame.asSeconds()/time.asSeconds()) * offsetVector.x, 0);
            for(auto element: buttons)
                element->move((Game::timePerFrame.asSeconds()/time.asSeconds()) * offsetVector.x, 0);
            background.move((Game::timePerFrame.asSeconds()/time.asSeconds()) * offsetVector.x, 0);
            Game::RenderFrame();
            totalTime += Game::timePerFrame;
        }
        active = flag;
    } else {
        active = flag;
        sf::Time totalTime = sf::seconds(0.0f);
        sf::Time time = sf::seconds(0.5f);
        sf::Vector2f offsetVector = sf::Vector2f(2000.f,0.f);
        while(totalTime < time){
            for(auto element: texts)
                element->move((Game::timePerFrame.asSeconds()/time.asSeconds()) * offsetVector.x, 0);
            for(auto element: buttons)
                element->move((Game::timePerFrame.asSeconds()/time.asSeconds()) * offsetVector.x, 0);
            background.move((Game::timePerFrame.asSeconds()/time.asSeconds()) * offsetVector.x, 0);
            Game::RenderFrame();
            totalTime += Game::timePerFrame;
        }
    }
}

bool GUI::Menu::isActive() {
    return active;
}

void GUI::Menu::setPosition(const sf::Vector2f &pos) {
    currentPosition = startingPosition = pos;
}

void GUI::Menu::setInc(const sf::Vector2f &inc) {
    incVector = inc;
}

void GUI::Menu::emptyText(){
    for(auto element: texts){
        delete element;
        currentPosition -= incVector;
    }
    texts.clear();
    createBackground();
}

void GUI::Menu::createBackground() {
    sf::Vector2f size(50.f,50.f);
    float maxWidth = 0;
    for(auto element: texts){
        size.y += element->getLocalBounds().height + incVector.y;
        maxWidth = element->getLocalBounds().width > maxWidth ? element->getLocalBounds().width : maxWidth;
    }
    for(auto element: buttons){
        size.y += element->getLocalBounds().height + incVector.y;
        maxWidth = element->getLocalBounds().width > maxWidth ? element->getLocalBounds().width : maxWidth;
    }
    size.x += maxWidth;
    background.setSize(size);
    background.setFillColor(sf::Color(0x000000B3));
    background.setPosition(startingPosition-sf::Vector2f(25.f,25.f));
}

bool GUI::Menu::clickScan(const sf::Vector2f &mousePos) {
    for (auto element : buttons) {
        if (element->isMouseOver(mousePos)) {
            element->setState(disabled);
            element->setFillColor(clickColor);
            buttonClickSound.sound.play();
            element->trigger();
            element->setState(enabled);
            element->setFillColor(defaultColor);
            return true;
        }
    }
}

void GUI::Menu::hoveringScan(const sf::Vector2f &mousePos) {
    for (auto element : buttons) {
        if (element->isMouseOver(mousePos) && element->getState() != hovered) {
            buttonHoverSound.sound.play();
            element->setState(hovered);
            element->setFillColor(hoverColor);
            break;
        } else if (!element->isMouseOver(mousePos) && element->getState() == hovered) {
            element->setState(enabled);
            element->setFillColor(defaultColor);
        }
    }
}

void GUI::Menu::setText(const std::string &fontPath, unsigned int characterSize, const sf::Uint32 &color) {
    menuFont.loadFromFile("../assets/fonts/" + fontPath);
    charSize = characterSize;
    defaultColor = sf::Color(color);
}

void GUI::Menu::setClick(const std::string &name, const sf::Uint32 &color) {
    buttonClickSound.setSound("../assets/sound/" + name);
    clickColor = sf::Color(color);
}

void GUI::Menu::setHover(const std::string &name, const sf::Uint32 &color) {
    buttonHoverSound.setSound("../assets/sound/" + name);
    hoverColor = sf::Color(color);
}
#include "GUI.h"
#include "triggers.h"
#include "Game.h"

std::map<GUI::Menus, std::shared_ptr<GUI::Menu>> GUI::menus;
std::map<const std::string, std::function<void()>> GUI::triggers;
std::map<const std::string, std::shared_ptr<sf::Font>> GUI::fonts;
std::vector<std::shared_ptr<sf::Text>> GUI::texts;

void GUI::Init() {
    std::shared_ptr<sf::Font> pFont(new sf::Font);
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
    std::shared_ptr<sf::Text> pText(new sf::Text(text, *fonts[font], charSize));
    pText->setFillColor(color);
    pText->setPosition(pos);
    texts.push_back(pText);
}

void GUI::createMainMenu() {
    std::shared_ptr<Menu> pMenu(new Menu);
    pMenu->setActive(true);
    pMenu->setText("sansation.ttf", 50, 0xFFFFFFFF);
    pMenu->setHover("Main_Menu_Hover_Effect.wav", 0xFF0000FF);
    pMenu->setClick("Main_Menu_Click_Effect.wav", 0xFFFF00FF);
    pMenu->setInc(sf::Vector2f(0.f, 100.f));
    pMenu->setPosition(sf::Vector2f(200.f, 250.f));
    pMenu->addButton("New Game");
    pMenu->addButton("Quit");
    menus[Menus::Main] = pMenu;
}

void GUI::createModeMenu(){
    std::shared_ptr<Menu> pMenu(new Menu);
    pMenu->setActive(false);
    pMenu->setText("sansation.ttf", 50, 0xFFFFFFFF);
    pMenu->setHover("Main_Menu_Hover_Effect.wav", 0xFF0000FF);
    pMenu->setClick("Main_Menu_Click_Effect.wav", 0xFFFF00FF);
    pMenu->setInc(sf::Vector2f(0.f, 100.f));
    pMenu->setPosition(sf::Vector2f(200.f, 150.f));
    pMenu->addButton("1 Player");
    pMenu->addButton("2 Players");
    pMenu->addButton("Back");
    menus[Menus::Modes] = pMenu;
}

void GUI::createEndGameMenu(const std::string &message) {
    std::shared_ptr<Menu> pMenu(new Menu);
    pMenu->setActive(false);
    pMenu->setText("sansation.ttf", 50, 0xFFFFFFFF);
    pMenu->setHover("Main_Menu_Hover_Effect.wav", 0xFF0000FF);
    pMenu->setClick("Main_Menu_Click_Effect.wav", 0xFFFF00FF);
    pMenu->setInc(sf::Vector2f(0.f, 100.f));
    pMenu->setPosition(sf::Vector2f(400.f, 300.f));
    pMenu->addText(message);
    pMenu->addButton("Restart");
    pMenu->addButton("Back to Main Menu");
    pMenu->addButton("Quit");
    menus[Menus::EndGame] = pMenu;
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
        sf::Time time = sf::seconds(0.75f);
        sf::Vector2f offsetVector = sf::Vector2f(-1000.f,0.f);
        while(totalTime < time){
            for(auto element: buttons)
                element->move((Game::timePerFrame.asSeconds()/time.asSeconds()) * offsetVector.x, 0);
            Game::RenderFrame();
            totalTime += Game::timePerFrame;
        }
    }
    active = flag;
}

bool GUI::Menu::isActive() {
    return active;
}

void GUI::Menu::setPosition(const sf::Vector2f &pos) {
    currentPosition = pos;
}

void GUI::Menu::setInc(const sf::Vector2f &inc) {
    incVector = inc;
}

void GUI::Menu::clickScan(const sf::Vector2f &mousePos) {
    for (auto element : buttons) {
        if (element->isMouseOver(mousePos)) {
            element->setState(disabled);
            element->setFillColor(clickColor);
            buttonClickSound.sound.play();
            element->trigger();
            element->setState(enabled);
            break;
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
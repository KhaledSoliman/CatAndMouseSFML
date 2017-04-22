#include "GL.h"
#include "GUI.h"

GL::Point GL::mouseLocation(4, 4);
GL::Point GL::catLocation(2, 5);
GL::World GL::world(9, 9, sf::Vector2f(600.f, 50.f), sf::Vector2f(80.f, 80.f));

/*
 * Functions Native to the Namespace.
 */

void GL::Render(sf::RenderWindow &window) {
    world.draw(window);
}

/*
 * Tile Class Definition
 */

void GL::Tile::setEntity(GL::Entity *pEntity) {
    if (pEntity != nullptr)
        pEntity->setPosition((this->getPosition().x + (this->getSize().x / 2)) - pEntity->getRadius(),
                             (this->getPosition().y + (this->getSize().y / 2)) - pEntity->getRadius());
    occupant = pEntity;
}

GL::TileType GL::Tile::getType() const {
    return type;
}

GL::Entity *GL::Tile::getEntity() {
    return occupant;
}

GL::Point GL::Tile::getWorldPosition() const {
    return coords;
}

void GL::Tile::setTile(const sf::Color &color, const sf::Vector2f &pos, const sf::Vector2f &size, TileType tileType,
                       Point location) {
    setFillColor(color);
    setSize(size);
    setPosition(pos);
    coords = location;
    type = tileType;
}

void GL::Tile::draw(sf::RenderWindow &window) const {
    window.draw(*this);
    if (occupant != nullptr)
        occupant->draw(window);
}

/*
 * Entity Class Definition
 */

GL::Entity::Entity(EntityType entityType, Point position) : coords(position) {
    type = entityType;
}

GL::EntityType GL::Entity::getType() const {
    return type;
}

void GL::Entity::draw(sf::RenderWindow &window) const {
    window.draw(*this);
};

/*
 * World Class Definition
 */

void GL::World::setActive(bool boolean) {
    active = boolean;
}

void GL::World::populateWorld() {
    sf::Vector2f currentPosition = startingPosition;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            currentPosition.x += tileSize.x + 0.f;
            if ((j == ((height - 1) / 2)) && (i == (width - 1)))
                content[j][i].setTile(sf::Color(0xFFFFFFFF), currentPosition, tileSize, TileType::bridge, Point(i, j));
            else if (j % (height - 1) == 0 || i % (width - 1) == 0)
                content[j][i].setTile(sf::Color(0x0000FFFF), currentPosition, tileSize, TileType::water, Point(i, j));
            else
                content[j][i].setTile(sf::Color(0x006400FF), currentPosition, tileSize, TileType::land, Point(i, j));
            content[j][i].setEntity(nullptr);
        }
        currentPosition.x = startingPosition.x;
        currentPosition.y += tileSize.y + 0.f;
    }
    mouseLocation = Point(4,4);
    catLocation = Point(2,5);
    Entity *pEntity = new Entity(EntityType::Mouse, mouseLocation);
    pEntity->setFillColor(sf::Color::Red);
    pEntity->setRadius(25);
    content[mouseLocation.y][mouseLocation.x].setEntity(pEntity);
    pEntity = new Entity(EntityType::Cat, catLocation);
    pEntity->setFillColor(sf::Color::Black);
    pEntity->setRadius(25);
    content[catLocation.y][catLocation.x].setEntity(pEntity);
}

void GL::World::moveEntity(EntityType type, Direction direction) {
    Point *newLocation;
    Point previousLocation;
    Entity *pEntity;
    switch (type) {
        case EntityType::Mouse:
            previousLocation = mouseLocation;
            newLocation = &mouseLocation;
            break;
        case EntityType::Cat:
            previousLocation = catLocation;
            newLocation = &catLocation;
            break;
    }
    switch (direction) {
        case Direction::North:
            newLocation->y = previousLocation.y - 1;
            newLocation->x = previousLocation.x;
            break;
        case Direction::East:
            newLocation->x = previousLocation.x + 1;
            newLocation->y = previousLocation.y;
            break;
        case Direction::West:
            newLocation->x = previousLocation.x - 1;
            newLocation->y = previousLocation.y;
            break;
        case Direction::South:
            newLocation->y = previousLocation.y + 1;
            newLocation->x = previousLocation.x;
            break;
    }
    pEntity = content[previousLocation.y][previousLocation.x].getEntity();
    content[previousLocation.y][previousLocation.x].setEntity(nullptr);
    examineLocal(content[newLocation->y][newLocation->x], pEntity->getType());
    content[newLocation->y][newLocation->x].setEntity(pEntity);
}

void GL::World::examineLocal(Tile location, EntityType Entity){
    if(Entity == EntityType::Cat){
        if(location.getType() == TileType::water){
            endGame("The cat has drowned :(");
        }else if(location.getWorldPosition() == mouseLocation){
            endGame("The cat has eaten the mouse :(");
        }
    } else {
        if(location.getWorldPosition() == catLocation){
            endGame("The mouse has fed it self to the cat??");
        }else if(location.getType() == TileType::water){
            endGame("The mouse has jumped in the water lol!");
        }else if (location.getType() == TileType::bridge){
            endGame("The mouse has escaped to god knows where!");
        }
    }
}

void GL::World::endGame(const std::string& message) {
    setActive(false);
    GUI::createEndGameMenu(message);
    GUI::menus[GUI::Menus::EndGame]->setActive(true);
}

GL::World::World(unsigned int newHeight, unsigned int newWidth, const sf::Vector2f &pos, const sf::Vector2f &size)
        : height(newHeight), width(newWidth) {
    content = new Tile *[height];
    for (int i = 0; i < height; i++)
        content[i] = new Tile[width];
    startingPosition = pos;
    tileSize = size;
    populateWorld();
}

void GL::World::draw(sf::RenderWindow &window) {
    if (active)
        for (int j = 0; j < height; j++)
            for (int i = 0; i < width; i++)
                content[j][i].draw(window);
}
#ifndef CS110_CAT_MOUSE_GL_H
#define CS110_CAT_MOUSE_GL_H

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "Game.h"

namespace GL {

    struct Point {
        int x, y;

        Point() : x(0), y(0) {}

        Point(int i, int j) : x(abs(i)), y(abs(j)) {};

        bool operator==(const Point &RHS) {
            return (RHS.x == x && RHS.y == y);
        };

        Point& operator=(const Point& RHS){
            x = RHS.x;
            y = RHS.y;
            return *this;
        }
    };


    enum class EntityType {
        Mouse,
        Cat
    };


    enum class TileType {
        water,
        land,
        bridge
    };
    enum class Direction {
        North,
        East,
        West,
        South
    };

    class Entity : public sf::CircleShape {
    public:
        Entity(EntityType entityType, Point position);

        EntityType getType() const;

        void draw(sf::RenderWindow &) const;

    private:
        Point coords;
        EntityType type;
    };


    class Tile : public sf::RectangleShape {
    public:
        Tile() {}

        void setTile(const sf::Color &, const sf::Vector2f &,const sf::Vector2f&, TileType,Point);

        Entity* getEntity();

        void setEntity(Entity *);

        TileType getType() const;

        Point getWorldPosition() const;

        void draw(sf::RenderWindow &) const;

        ~Tile(){
            delete occupant;
        }

    private:
        Entity *occupant;
        TileType type;
        Point coords;
    };

    class World {
    public:
        World() : active(false) {}

        World(const unsigned int, const unsigned int, const sf::Vector2f&, const sf::Vector2f&);

        void populateWorld();

        void moveEntity(EntityType, Direction);

        void examineLocal(Tile, EntityType);

        void setActive(bool);

        void endGame(const std::string&);

        void draw(sf::RenderWindow &);

        ~World() {
            for (int j = 0; j < height; j++) {
                delete[] content[height];
            }
        }

    private:
        unsigned int height;
        unsigned int width;
        sf::Vector2f startingPosition;
        sf::Vector2f tileSize;
        bool active;
        Tile **content;
    };

    void Render(sf::RenderWindow &);

    extern World world;
    extern Point mouseLocation;
    extern Point catLocation;
};


#endif //CS110_CAT_MOUSE_GL_H
//LevelSystem.h
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <maths.h>

#define ls LevelSystem

class LevelSystem {
public:
    enum TILE { EMPTY, START, END, WALL, ENEMY, WAYPOINT };

    static void loadLevelFile(const std::string&, float tileSize = 100.f);
    static void Render(sf::RenderWindow& window);
    static sf::Color getColor(TILE t);
    static void setColor(TILE t, sf::Color c);
    //Get Tile at grid coordinate
    static TILE getTile(sf::Vector2ul);
    //Get Screenspace coordinate of tile
    static sf::Vector2f getTilePosition(sf::Vector2ul);
    //get the tile at screenspace pos
    static TILE getTileAt(sf::Vector2f);

    static std::vector<sf::Vector2ul> findTiles(TILE);

    static size_t getWidth();

    static size_t getHeight();

protected:
    static std::unique_ptr<TILE[]> _tiles; //Internal array of tiles
    static size_t _width; //how many tiles wide is level
    static size_t _height; //how many tile high is level
    static sf::Vector2f _offset; //Screenspace offset of level, when rendered.
    static float _tileSize; //Screenspace size of each tile, when rendered.
    static std::map<TILE, sf::Color> _colours; //color to render each tile type

    //array of sfml sprites of each tile
    static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;
    //generate the _sprites array
    static void buildSprites();

private:
    LevelSystem() = delete;
    ~LevelSystem() = delete;
};

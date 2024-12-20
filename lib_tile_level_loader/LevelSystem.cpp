//LevelSystem.cpp
#include "LevelSystem.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace sf;

std::unique_ptr<LevelSystem::TILE[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;
Vector2f LevelSystem::_offset(0.0f, 0.0f);

float LevelSystem::_tileSize(100.f);
vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

std::map<LevelSystem::TILE, sf::Color> LevelSystem::_colours{ {WALL, Color::White}, {END, Color::Magenta} };

sf::Color LevelSystem::getColor(LevelSystem::TILE t) {
    auto it = _colours.find(t);
    if (it == _colours.end()) {
        _colours[t] = Color::Transparent;
    }
    return _colours[t];
}

void LevelSystem::setColor(LevelSystem::TILE t, sf::Color c) {
    _colours[t] = c;
}

Vector2f LevelSystem::getTilePosition(Vector2ul p) {
    return (Vector2f(p.x, p.y) * _tileSize);
}

void LevelSystem::loadLevelFile(const std::string& path, float tileSize) {
    _tileSize = tileSize;
    size_t w = 0, h = 0;
    string buffer;

    // Load in file to buffer
    ifstream f(path);
    if (f.good()) {
        f.seekg(0, std::ios::end);
        buffer.resize(f.tellg());
        f.seekg(0);
        f.read(&buffer[0], buffer.size());
        f.close();
    }
    else {
        throw string("Couldn't open level file: ") + path;
    }

    std::vector<TILE> temp_tiles;
    for (int i = 0; i < buffer.size(); ++i) {
        const char c = buffer[i];
        switch (c) {
        case 'w':
            temp_tiles.push_back(WALL);
            break;
        case 's':
            temp_tiles.push_back(START);
            break;
        case 'e':
            temp_tiles.push_back(END);
            break;
        case ' ':
            temp_tiles.push_back(EMPTY);
            break;
        case '+':
            temp_tiles.push_back(WAYPOINT);
            break;
        case 'n':
            temp_tiles.push_back(ENEMY);
            break;
        case '\n':      // end of line
            if (w == 0) { // if we haven't written width yet
                w = i;      // set width
            }
            h++; // increment height
            break;
        default:
            cout << c << endl; // Don't know what this tile type is
        }
    }
    if (temp_tiles.size() != (w * h)) {
        throw string("Can't parse level file") + path;
    }
    _tiles = std::make_unique<TILE[]>(w * h);
    _width = w; //set static class vars
    _height = h;
    std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
    cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
    buildSprites();
}

size_t LevelSystem::getWidth() { return _width; }

size_t LevelSystem::getHeight() { return _height; }

void LevelSystem::buildSprites() {
    _sprites.clear();
    for (size_t y = 0; y < LevelSystem::getHeight(); ++y) {
        for (size_t x = 0; x < LevelSystem::getWidth(); ++x) {
            auto s = make_unique<RectangleShape>();
            s->setPosition(getTilePosition({ x, y }));
            s->setSize(Vector2f(_tileSize, _tileSize));
            s->setFillColor(getColor(getTile({ x, y })));
            _sprites.push_back(move(s));
        }
    }
}

LevelSystem::TILE LevelSystem::getTile(Vector2ul p) {
    if (p.x > _width || p.y > _height) {
        throw string("Tile out of range: ") + to_string(p.x) + "," + to_string(p.y) + ")";
    }
    return _tiles[(p.y * _width) + p.x];
}

LevelSystem::TILE LevelSystem::getTileAt(Vector2f v) {
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) {
        throw string("Tile out of range ");
    }
    return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

std::vector<sf::Vector2ul> LevelSystem::findTiles(LevelSystem::TILE type) {
    auto v = vector<sf::Vector2ul>();
    for (size_t i = 0; i < _width * _height; ++i) {
        if (_tiles[i] == type) {
            v.push_back({ i % _width, i / _width });
        }
    }

    return v;
}

void LevelSystem::Render(RenderWindow& window) {
    for (size_t i = 0; i < _width * _height; ++i) {
        window.draw(*_sprites[i]);
    }
}

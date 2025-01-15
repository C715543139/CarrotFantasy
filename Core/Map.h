#ifndef MAP_H
#define MAP_H

#include <QSettings>
#include <vector>
#include <iostream>
#include <sstream>
#include "Unit.h"
using std::vector, std::stringstream, std::cout, std::endl;

class Tile {
public:
    Tile();
    friend class Map;

private:
    vector<Monster> monsters;
    vector<Tower> towers;
    vector<Item> items;

    enum TileType {
        EMPTY, CARROT, NEST, PATH, CLOUD
    } tileType;

    enum TileDirection {
        UP, DOWN, LEFT, RIGHT, NONE
    } tileDirection;
};

class Map {
public:
    explicit Map(int map);
    int width() const;
    int height() const;
    Tile &getTile(int y, int x); // 获取Tile

private:
    const int WIDTH = 12, HEIGHT = 7, TILESIZE = 90;
    vector<vector<Tile>> tiles;
};

#endif // MAP_H

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
    friend class GameManager;

private:
    vector<Monster> monsters;
    vector<Tower> towers;
    // vector<Item> items;

    enum TileType {
        EMPTY, CARROT, NEST, PATH, CLOUD
    } tileType;

    Direction tileDirection;
};

#endif // MAP_H

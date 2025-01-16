#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Map.h"

class GameManager {
public:
    GameManager();
    void update();
    void init(int mapIndex);

private:
    int waveIndex, waveMax;
    const int WIDTH = 12, HEIGHT = 7, TILESIZE = 90;
    vector<vector<Tile>> tiles;
};

#endif // GAMEMANAGER_H

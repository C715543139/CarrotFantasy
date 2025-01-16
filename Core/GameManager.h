#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QPainter>
#include <QRandomGenerator>
#include "Map.h"

class GameManager: public QObject {
    Q_OBJECT

public:
    GameManager();
    ~GameManager();
    void update();
    void init(int mapIndex);
    QPixmap getTileImage(int y, int x);
    const int width = 12, height = 7, tileSize = 90;

private:
    vector<vector<Tile>> tiles;
    Nest nest;
    Carrot carrot;
    int waveIndex, waveMax, waveTimer;
    int monsterCounter, monsterKilled, monsterTimer;

    void monsterMove();

signals:
    void waveChange(int wave);
};

#endif // GAMEMANAGER_H

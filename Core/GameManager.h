#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QPainter>
#include <QRandomGenerator>
#include <map>
#include "Map.h"
using std::map;

class GameManager: public QObject {
    Q_OBJECT

public:
    const int width = 12, height = 7, tileSize = 90;

    GameManager();
    ~GameManager();
    void update();
    void init(int mapIndex);
    QPixmap getTileImage(int y, int x);
    int canPlaceTower(int posY, int posX);
    void addTower(int posY, int posX, const QString &name);
    bool enoughCoin(const QString &name);

private:
    int monsterCounter, monsterKilled, monsterTimer;
    int waveIndex, waveMax, waveTimer;
    int coin;
    vector<vector<Tile>> tiles;
    Nest nest;
    Carrot carrot;
    map<std::pair<int, int>, std::pair<int, int>> starTiles; // type, index
    vector<vector<QPixmap>> starAnime;

    void monsterMove();
    void towerAttack();
    void towerDamage(int y, int x);

signals:
    void waveChange(int wave);
    void coinChange(int coin);
};

#endif // GAMEMANAGER_H

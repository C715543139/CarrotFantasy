#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QPainter>
#include <QRandomGenerator>
#include <QMultiMap>
#include "Map.h"
using std::map;

class GameManager: public QObject {
    Q_OBJECT

public:
    const int width = 12, height = 7, tileSize = 90;

    GameManager();
    ~GameManager();
    bool update();
    void init(int mapIndex);
    QPixmap getTileImage(int y, int x);
    int canPlaceTower(int posY, int posX) const;
    void addTower(int posY, int posX, const QString &name);
    void upgradeTower(int posY, int posX);
    void removeTower(int posY, int posX);
    std::pair<int, int> getCost(int posY, int posX) const; // 升级, 出售
    bool enoughToUpgrade(int posY, int posX) const;
    bool enoughToPlace(const QString &name) const;

private:
    int coin;
    int waveIndex, waveMax, waveTimer;
    int monsterCounter, monsterKilled, monsterTimer;
    Nest nest;
    Carrot carrot;
    vector<vector<Tile>> tiles;
    vector<vector<QPixmap>> specialAnime;
    QMultiMap<std::pair<int, int>, std::pair<int, int>> specialTiles; // type, index
    bool monsterMove();
    void towerAttack();
    void towerDamage(int y, int x);

signals:
    void waveChange(int wave);
    void coinChange(int coin);
    void lose();
    void win(int hp);
};

#endif // GAMEMANAGER_H

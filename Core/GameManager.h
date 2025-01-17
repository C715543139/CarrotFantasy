#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QPainter>
#include <QRandomGenerator>
#include "Map.h"

class GameManager: public QObject {
    Q_OBJECT

public:
    const int width = 12, height = 7, tileSize = 90;

    GameManager();
    ~GameManager();
    void update();
    void init(int mapIndex);
    QPixmap getTileImage(int y, int x);
    void addTower(int posY, int posX, const QString &name);

private:
    vector<vector<Tile>> tiles;
    Nest nest;
    Carrot carrot;
    int waveIndex, waveMax, waveTimer;
    int monsterCounter, monsterKilled, monsterTimer;

    void monsterMove();
    void towerAttack();
    void towerDamage(int y, int x);

signals:
    void waveChange(int wave);
};

#endif // GAMEMANAGER_H

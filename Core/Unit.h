#ifndef UNIT_H
#define UNIT_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QPainter>
#include <vector>
using std::vector;


enum Direction {
    UP, DOWN, LEFT, RIGHT, NONE
};

class Unit {
public:
    explicit Unit(int y, int x);
    friend class GameManager;

protected:
    int y, x;
};

class Monster : public Unit {
public:
    Monster(int y, int x, const QString &name, Direction dir);
    QPixmap getImage();
    friend class GameManager;

private:
    int speed, value, hp, damage;
    int animeIndex, animeTimer;
    int moveTimer;
    int frozen, fired, slowed;
    QPixmap anime1, anime2;
    Direction direction;
    QPixmap frozenImage, firedImage, slowedImage;
};

class Tower : public Unit {
public:
    Tower(int y, int x, const QString &name);
    QPixmap getImage();
    friend class GameManager;

private:
    int level, createCost, CDTimer;
    vector<int> atkDamage, atkRange, sellValue, upgradeCost;
    vector<double> atkSpace;
    QString name;
    // 动画相关
    vector<QPixmap> normalImage;
    vector<vector<QPixmap>> atkAnime;
};

class Carrot : public Unit {
public:
    Carrot(int y, int x);
    QPixmap getImage();
    friend class GameManager;

private:
    int hp;
    vector<QPixmap> anime;
};

class Nest : public Unit {
public:
    Nest(int y, int x);
    QPixmap getImage();
    friend class GameManager;

private:
    QPixmap anime;
};

// class Item : public Unit {};


#endif // UNIT_H

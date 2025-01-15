#ifndef UNIT_H
#define UNIT_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <vector>
using std::vector;

class Unit {
public:
    explicit Unit(int y, int x);

protected:
    int y, x;
};

class Monster : public Unit {
public:
    Monster(int y, int x, const QString &name);

private:
    int speed, value, hp, damage;
    QPixmap anime1, anime2;
};

class Tower : public Unit {
public:
    Tower(int y, int x, const QString &name);

private:
    int level, createCost;
    vector<int> atkDamage, atkRange, sellValue, upgradeCost;
    vector<double> atkSpace;
    const QString name;
    // 动画相关
};

class Item : public Unit {};


#endif // UNIT_H

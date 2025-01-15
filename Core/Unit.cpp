#include "Unit.h"


Unit::Unit(int y, int x) : y(y), x(x) {}

Monster::Monster(int y, int x, const QString &name) : Unit(y, x) {
    // 读取文件内容
    QFile file(":/res/Game/JSON/monsters.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonData = file.readAll();
    file.close();

    // 解析 JSON 数据
    QJsonObject targetObj = QJsonDocument::fromJson(jsonData).object()[name].toObject();
    speed = targetObj["Speed"].toInt();
    value = targetObj["Value"].toInt();
    hp = targetObj["Hp"].toInt();
    damage = targetObj["Damage"].toInt();
    anime1 = QPixmap(":/res/Game/Monsters/" + name + "01.png");
    anime2 = QPixmap(":/res/Game/Monsters/" + name + "02.png");
}

Tower::Tower(int y, int x, const QString &name) : Unit(y, x), name(name) {
    QFile file(":/res/Game/JSON/towers.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonObject rootObj = QJsonDocument::fromJson(jsonData).object();

    // 解析 JSON 数据
    level = 0;
    for (int i = 0; i < 3; i++) {
        QJsonObject targetObj = rootObj[name + QString::number(i)].toObject();
        if (i == 0) createCost = targetObj["CreateCost"].toInt();
        atkSpace.push_back(targetObj["AttackSpace"].toDouble());
        atkDamage.push_back(targetObj["AttackDamage"].toInt());
        atkRange.push_back(targetObj["AttackRange"].toInt());
        sellValue.push_back(targetObj["SellValue"].toInt());
        upgradeCost.push_back(targetObj["UpgradeCost"].toInt());
    }

    // 动画相关
}

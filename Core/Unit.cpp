#include "Unit.h"


Unit::Unit(int y, int x) : y(y), x(x) {}


Monster::Monster(int y, int x, const QString &name, Direction dir)
    : Unit(y, x), animeIndex(1), animeTimer(0), moveTimer(0), direction(dir) {
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

QPixmap Monster::getImage() {
    animeTimer += 16;
    if (animeTimer > 200) {
        animeTimer = 0;
        if (animeIndex == 1) {
            animeIndex = 2;
        } else {
            animeIndex = 1;
        }
    }
    return (animeIndex == 1) ? anime1 : anime2;
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

QPixmap Tower::getImage() {
    return QPixmap(":/res/Game/Monsters/land_star02.png");
}

Carrot::Carrot(int y, int x): Unit(y, x), hp(10) {
    for (int i = 1; i <= 10; i++)
        anime.push_back(QPixmap(QString(":/res/Game/Carrot/hlb%1.png").arg(i)));
}

QPixmap Carrot::getImage() {
    return anime[hp - 1];
}

Nest::Nest(int y, int x) : Unit(y, x), anime(QPixmap(":/res/Game/Monsters/nest.png")) {}

QPixmap Nest::getImage() {
    return anime;
}

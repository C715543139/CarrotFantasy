#include "Unit.h"


Unit::Unit(int y, int x) : y(y), x(x) {}

Monster::Monster(int y, int x, const QString &name, Direction dir)
    : Unit(y, x), animeIndex(1), animeTimer(0), moveTimer(0), direction(dir),
      frozen(0), fired(0), slowed(0),
      frozenImage{":/res/Game/Monsters/Effect/frozen.png"},
      firedImage{":/res/Game/Monsters/Effect/fired.png"},
      slowedImage{":/res/Game/Monsters/Effect/slowed.png"} {
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
    if (animeTimer > 48) {
        animeTimer = 0;
        if (animeIndex == 1) {
            animeIndex = 2;
        } else {
            animeIndex = 1;
        }
    }

    QPixmap pixmap(200, 200), target(animeIndex == 1 ? anime1 : anime2);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPoint offset = pixmap.rect().center() - QPoint(target.width() / 2, target.height() / 2);
    painter.drawPixmap(offset, target);

    if (frozen > 0) {
        offset = QPoint((pixmap.width() - frozenImage.width()) / 2, 100);
        painter.drawPixmap(offset, frozenImage);
    }
    if (fired > 0) {
        offset = QPoint((pixmap.width() - firedImage.width()) / 2, 100);
        painter.drawPixmap(offset, firedImage);
    }
    if (slowed > 0) {
        offset = QPoint((pixmap.width() - slowedImage.width()) / 2, 100);
        painter.drawPixmap(offset, slowedImage);
    }
    return pixmap;
}

Tower::Tower(int y, int x, const QString &name) : Unit(y, x), name(name), CDTimer(0) {
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

        // 动画
        normalImage.push_back(QPixmap(
            QString(":/res/Game/Tower/" + name + "/N" + name + "%1.png").arg(i)));
        atkAnime.push_back(vector<QPixmap>());
        if (name == "Star" || name == "BStar") {
            for (int j = 0; j < 3; j++) {
                atkAnime[i].push_back(QPixmap(
                    QString(":/res/Game/Tower/" + name + "/A" + name + "%1%2.png").arg(i).arg(j)));
            }
        } else {
            for (int j = 0; j < 5; j++) {
                atkAnime[i].push_back(QPixmap(
                    QString(":/res/Game/Tower/" + name + "/A" + name + "%1%2.png").arg(i).arg(j)));
            }
        }
    }
}

QPixmap Tower::getImage() {
    if (CDTimer > 0 && CDTimer < atkAnime[level].size() * 64) return atkAnime[level][CDTimer / 64];
    return normalImage[level];
}

Carrot::Carrot(int y, int x): Unit(y, x), hp(10) {
    for (int i = 1; i <= 10; i++)
        anime.push_back(QPixmap(QString(":/res/Game/Carrot/hlb%1.png").arg(i)));
}

QPixmap Carrot::getImage() {
    return anime[hp - 1];
}

Nest::Nest(int y, int x) : Unit(y, x), bornTimer(0),
                           nest(":/res/Game/Monsters/nest.png"),
                           born(":/res/Game/Monsters/born.png") {}

QPixmap Nest::getImage() {
    if (bornTimer > 0) {
        bornTimer -= 16;
        return born;
    }
    return nest;
}

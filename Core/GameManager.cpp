#include "GameManager.h"


GameManager::GameManager()
    : tiles(vector(height, vector<Tile>(width))),
      nest(0, 0), carrot(0, 0),
      waveIndex(1), waveMax(15), waveTimer(0),
      monsterCounter(0), monsterKilled(0), monsterTimer(0)
{}

GameManager::~GameManager() {}

const QList<QString>
        monsterNames = {
            "land_star", "land_pink", "land_nima", "fly_yellow", "fly_blue", "fat_green"
        },
        bossNames = {
            "land_boss_star", "land_boss_pink", "land_boss_nima", "fly_boss_yellow", "fly_boss_blue", "fat_boss_green"
        };

void GameManager::update() {
    // 波次刷新
    waveTimer += 16;
    if (waveTimer > 10000 && monsterCounter == monsterKilled) {
        waveTimer = 0;
        waveIndex++;
        waveChange(waveIndex);
    }

    // 怪物刷新
    if (monsterCounter < waveIndex * 10) {
        monsterTimer += 16;
        if (monsterTimer > 500) {
            monsterTimer = 0;
            monsterCounter++;

            // 随机选择生成的怪物
            QString name;
            if (monsterCounter == waveMax * 10) name = bossNames[QRandomGenerator::global()->bounded(0, 6)];
            else name = monsterNames[QRandomGenerator::global()->bounded(0, 6)];

            tiles[nest.y][nest.x].monsters.push_back(
                Monster(nest.y, nest.x, name, tiles[nest.y][nest.x].tileDirection));
        }
    }

    monsterMove();
}

void GameManager::monsterMove() {
    // 创建新地图格
    vector newTiles(tiles);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            newTiles[y][x].monsters.clear();

    // 更新Monster所在格子
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (auto monster: tiles[y][x].monsters) {
                if (monster.moveTimer >= tileSize) {
                    monster.moveTimer = 0;
                    int Y = (monster.y += (monster.direction == DOWN) - (monster.direction == UP)),
                            X = (monster.x += (monster.direction == RIGHT) - (monster.direction == LEFT));
                    auto &newTile = newTiles[Y][X];

                    if (newTile.tileType == Tile::CARROT) carrot.hp -= monster.damage;
                    else {
                        if (tiles[Y][X].tileDirection != NONE) monster.direction = tiles[Y][X].tileDirection;
                        monster.moveTimer += monster.speed;
                        newTiles[Y][X].monsters.push_back(monster);
                    }
                } else {
                    monster.moveTimer += monster.speed;
                    newTiles[monster.y][monster.x].monsters.push_back(monster);
                }
            }
        }
    }
    tiles = newTiles;
}


void GameManager::init(int mapIndex) {
    waveIndex = 1, waveTimer = 0;
    waveChange(waveIndex);
    monsterCounter = 0, monsterKilled = 0, monsterTimer = 0;
    tiles = vector(height, vector<Tile>(width));

    // 加载ini
    QSettings settings(QString(":/res/Game/Path/p%1.ini").arg(mapIndex), QSettings::IniFormat);

    auto setTile = [&](const QString &key, Tile::TileType type, Direction dir) {
        int amount = settings.value(key + "/amount").toInt();
        for (int i = 0, x = 0, y = 0; i < amount; i++) {
            stringstream ss(settings.value(key + "/" + QString::number(i)).toString().toStdString());
            ss >> y >> x;
            (dir == NONE) ? (tiles[y][x].tileType = type) : (tiles[y][x].tileDirection = dir);
        }
    };

    // 设置Path，Cloud
    setTile("Path", Tile::PATH, NONE);
    setTile("Cloud", Tile::CLOUD, NONE);

    // 设置Carrot
    int Y = settings.value("Carrot/y").toInt();
    int X = settings.value("Carrot/x").toInt();
    tiles[Y][X].tileType = Tile::CARROT;
    carrot.y = Y, carrot.x = X, carrot.hp = 10;

    // 设置Nest
    Y = settings.value("Nest/y").toInt();
    X = settings.value("Nest/x").toInt();
    tiles[Y][X].tileType = Tile::NEST;
    nest.y = Y, nest.x = X;

    // 设置方向
    setTile("Left", Tile::EMPTY, LEFT);
    setTile("Right", Tile::EMPTY, RIGHT);
    setTile("Up", Tile::EMPTY, UP);
    setTile("Down", Tile::EMPTY, DOWN);
}

QPixmap GameManager::getTileImage(int y, int x) {
    QPixmap pixmap(180, 180);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    Tile &tile = tiles[y][x];

    if (tile.tileType == Tile::NEST) {
        painter.drawPixmap(45, 45, nest.getImage());
    }

    if (tile.tileType == Tile::CARROT) {
        painter.drawPixmap(65, 10, carrot.getImage());
    }

    if (tile.tileType == Tile::EMPTY) {
        for (auto &tower: tile.towers) {
            painter.drawPixmap(45, 45, tower.getImage());
        }
    } else {
        for (auto &monster: tile.monsters) {
            painter.drawPixmap(55, 45, monster.getImage());
        }
    }

    return pixmap;
}

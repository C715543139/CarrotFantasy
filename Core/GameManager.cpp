#include "GameManager.h"


GameManager::GameManager(Sound *sound)
    : coin(0), waveIndex(1), waveMax(15), waveTimer(0),
      monsterCounter(0), monsterKilled(0), monsterTimer(0),
      nest(0, 0), carrot(0, 0), sound(sound),
      tiles(vector(height, vector<Tile>(width))) {
    // 初始化动画
    specialAnime.resize(3);
    for (int i = 0; i < 6; i++) {
        specialAnime[0].push_back(QPixmap(QString(":/res/Game/Tower/Star/PStar%1.png").arg(i)));
        specialAnime[1].push_back(QPixmap(QString(":/res/Game/Tower/BStar/PBStar%1.png").arg(i)));
        specialAnime[2].push_back(QPixmap(QString(":/res/Game/Monsters/Death/air%1.png").arg((i))));
    }
}

GameManager::~GameManager() {}

const QList<QString>
monsterNames = {
    "land_star", "land_pink", "land_nima", "fly_yellow", "fly_blue", "fat_green",
    "land_boss_star", "land_boss_pink", "land_boss_nima", "fly_boss_yellow", "fly_boss_blue", "fat_boss_green"
};

bool GameManager::update() {
    // 胜利判定
    if (waveIndex >= waveMax && monsterCounter == monsterKilled) {
        win(carrot.hp);
        return false;
    }

    // 波次刷新
    waveTimer += 16;
    if (waveTimer > 5000 && monsterCounter == monsterKilled) {
        waveTimer = 0;
        waveIndex++;
        waveChange(waveIndex);
    }

    // 怪物刷新
    if (monsterCounter < waveIndex * 10 && waveIndex <= waveMax) {
        monsterTimer += 16;
        if (monsterTimer > 500) {
            monsterTimer = 0;
            monsterCounter++;
            nest.bornTimer = 256;

            // 随机选择生成的怪物
            QString name;
            name = monsterNames[rand() % monsterNames.size()];
            tiles[nest.y][nest.x].monsters.push_back(
                Monster(nest.y, nest.x, name, tiles[nest.y][nest.x].tileDirection));
            sound->born();
        }
    }

    towerAttack();
    return monsterMove();
}

bool GameManager::monsterMove() {
    // 创建新地图格
    vector newTiles(tiles);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            newTiles[y][x].monsters.clear();

    // 更新Monster所在格子
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (auto monster: tiles[y][x].monsters) {
                // 死亡判定
                if (monster.hp <= 0) {
                    coinChange(coin += monster.value);
                    monsterKilled++;
                    specialTiles.insert({y, x}, {2, 5 * 16 * 2});
                    sound->monster();
                    continue;
                }

                // 状态更新
                if (monster.frozen > 0) {
                    monster.frozen -= 16;
                }
                if (monster.fired > 0) {
                    monster.fired -= 16;
                }
                if (monster.slowed > 0) {
                    monster.slowed -= 16;
                }

                // 更新位置
                if (monster.moveTimer >= tileSize && monster.frozen <= 0) {
                    monster.moveTimer = 0;
                    int Y = (monster.y += (monster.direction == DOWN) - (monster.direction == UP)),
                            X = (monster.x += (monster.direction == RIGHT) - (monster.direction == LEFT));
                    auto &newTile = newTiles[Y][X];

                    if (newTile.tileType == Tile::CARROT) {
                        monsterKilled++;
                        carrot.hp -= monster.damage;
                        sound->crash();

                        if (carrot.hp <= 0) {
                            lose();
                            return false;
                        }
                    } else {
                        if (tiles[Y][X].tileDirection != NONE) monster.direction = tiles[Y][X].tileDirection;
                        monster.moveTimer += (monster.speed / ((monster.slowed > 0) + 1));
                        newTiles[Y][X].monsters.push_back(monster);
                    }
                } else {
                    monster.moveTimer += (monster.speed / ((monster.slowed > 0) + 1));
                    newTiles[monster.y][monster.x].monsters.push_back(monster);
                }
            }
        }
    }
    tiles = newTiles;
    return true;
}

void GameManager::towerAttack() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (tiles[y][x].tileType == Tile::EMPTY && !tiles[y][x].towers.empty()) {
                towerDamage(y, x);
            }
        }
    }
}

void GameManager::towerDamage(int y, int x) {
    auto &tower = tiles[y][x].towers[0];
    int level = tower.level;

    // 判断是否处于CD
    if (tower.CDTimer != 0) {
        tower.CDTimer += 16;
        if (tower.CDTimer > 500 * tower.atkSpace[level]) {
            tower.CDTimer = 0;
        } else return;
    }

    bool attacked = false;
    if (tower.name == "Sun" || tower.name == "Snow") {
        for (int i = y - tower.atkRange[level]; i <= y + tower.atkRange[level]; i++) {
            for (int j = x - tower.atkRange[level]; j <= x + tower.atkRange[level]; j++) {
                if (i < 0 || i >= height || j < 0 || j >= width) continue;
                for (auto &monster: tiles[i][j].monsters) {
                    tower.CDTimer = 1; // 进入CD
                    monster.hp -= tower.atkDamage[level];
                    attacked = true;

                    if (tower.name == "Sun") monster.fired = 500;
                    if (tower.name == "Snow") monster.frozen = 1500;
                }
            }
        }
    } else {
        vector<std::pair<int, int>> monsterTiles;
        for (int i = y - tower.atkRange[level]; i <= y + tower.atkRange[level]; i++) {
            for (int j = x - tower.atkRange[level]; j <= x + tower.atkRange[level]; j++) {
                if (i < 0 || i >= height || j < 0 || j >= width) continue;
                if (!tiles[i][j].monsters.empty()) monsterTiles.push_back({i, j});
            }
        }

        if (!monsterTiles.empty()) {
            int randomIndex = rand() % monsterTiles.size();
            int Y = monsterTiles[randomIndex].first, X = monsterTiles[randomIndex].second;
            for (auto &monster: tiles[Y][X].monsters) {
                monster.hp -= tower.atkDamage[level];
                attacked = true;

                if (tower.name == "BStar") {
                    specialTiles.insert({Y, X}, {1, 5 * 16 * 2});
                    monster.slowed = 3000;
                } else {
                    specialTiles.insert({Y, X}, {0, 5 * 16 * 2});
                }
            }
            tower.CDTimer = 1;
        }
    }
    if (attacked) sound->tower(tower.name);
}

void GameManager::init(int mapIndex) {
    waveIndex = 1, waveTimer = 0;
    waveChange(waveIndex);
    monsterCounter = 0, monsterKilled = 0, monsterTimer = 0;
    tiles = vector(height, vector<Tile>(width));
    specialTiles.clear();

    // 加载ini
    QSettings settings(QString("Resource/Game/Path/p%1.ini").arg(mapIndex), QSettings::IniFormat);

    auto setTile = [&](const QString &key, Tile::TileType type, Direction dir) {
        int amount = settings.value(key + "/amount").toInt();
        for (int i = 0, x = 0, y = 0; i < amount; i++) {
            stringstream ss(settings.value(key + "/" + QString::number(i)).toString().toStdString());
            ss >> y >> x;
            if (dir == NONE) tiles[y][x].tileType = type;
            else tiles[y][x].tileDirection = dir;
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

    // 设置金币
    coinChange(coin = settings.value("Coin/amount").toInt());
}

QPixmap GameManager::getTileImage(int y, int x) {
    Tile &tile = tiles[y][x];
    QPixmap pixmap(400, 400), target;
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPoint center = pixmap.rect().center(), offset;

    // 中心对齐
    if (tile.tileType == Tile::NEST) {
        target = nest.getImage();
        offset = center - QPoint(target.width() / 2, target.height() / 2);
        painter.drawPixmap(offset, target);
    }

    if (tile.tileType == Tile::CARROT) {
        target = carrot.getImage();
        offset = center - QPoint(target.width() / 2, target.height() / 2);
        painter.drawPixmap(offset, target);
    }

    if (tile.tileType == Tile::EMPTY) {
        for (auto &tower: tile.towers) {
            target = tower.getImage();
            offset = center - QPoint(target.width() / 2, target.height() / 2);
            painter.drawPixmap(offset, target);
        }
    } else {
        for (auto &monster: tile.monsters) {
            target = monster.getImage();
            offset = center - QPoint(target.width() / 2, target.height() / 2);
            painter.drawPixmap(offset, target);
        }

        if (specialTiles.find({y, x}) != specialTiles.end()) {
            auto items = specialTiles.values({y, x});
            specialTiles.remove({y, x});
            for (auto it: items) {
                target = specialAnime[it.first][it.second / 32];
                offset = center - QPoint(target.width() / 2, target.height() / 2);
                painter.drawPixmap(offset, target);

                it.second -= 16;
                if (it.second > 0) {
                    specialTiles.insert({y, x}, it);
                }
            }
        }
    }
    return pixmap;
}

// 0为无反应, 1升级, 2放置
int GameManager::canPlaceTower(int posY, int posX) const {
    int y = posY / tileSize, x = posX / tileSize;
    auto &tile = tiles[y][x];
    if (tile.tileType == Tile::EMPTY) return tile.towers.empty() + 1;
    return 0;
}

void GameManager::addTower(int posY, int posX, const QString &name) {
    int y = posY / tileSize, x = posX / tileSize;
    auto &tile = tiles[y][x];
    auto tower = Tower(y, x, name);
    tile.towers.push_back(tower);
    coinChange(coin -= tower.createCost);
}

void GameManager::upgradeTower(int posY, int posX) {
    int y = posY / tileSize, x = posX / tileSize;
    auto &tower = tiles[y][x].towers[0];
    coinChange(coin -= tower.upgradeCost[tower.level]);
    tower.level++;
}

void GameManager::removeTower(int posY, int posX) {
    int y = posY / tileSize, x = posX / tileSize;
    auto &tower = tiles[y][x].towers[0];
    coinChange(coin += tower.sellValue[tower.level]);
    tiles[y][x].towers.clear();
}

std::pair<int, int> GameManager::getCost(int posY, int posX) const {
    int y = posY / tileSize, x = posX / tileSize;
    auto &tower = tiles[y][x].towers[0];
    return {tower.upgradeCost[tower.level], tower.sellValue[tower.level]};
}

bool GameManager::enoughToUpgrade(int posY, int posX) const {
    int y = posY / tileSize, x = posX / tileSize;
    auto &tower = tiles[y][x].towers[0];
    return coin >= tower.upgradeCost[tower.level];
}

bool GameManager::enoughToPlace(const QString &name) const {
    return coin >= Tower(0, 0, name).createCost;
}

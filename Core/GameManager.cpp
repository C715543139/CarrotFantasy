#include "GameManager.h"


GameManager::GameManager(): tiles(vector(HEIGHT, vector<Tile>(WIDTH))), waveIndex(1), waveMax(15) {}

void GameManager::update() {
}

void GameManager::init(int mapIndex) {
    waveIndex = 1;

    // 加载ini
    QSettings settings(QString(":/res/Game/Path/p%1.ini").arg(mapIndex), QSettings::IniFormat);

    auto setTile = [&](const QString &key, Tile::TileType type, Tile::TileDirection dir) {
        int amount = settings.value(key + "/amount").toInt();
        for (int i = 0, x = 0, y = 0; i < amount; i++) {
            stringstream ss(settings.value(key + "/" + QString::number(i)).toString().toStdString());
            ss >> y >> x;
            (dir == Tile::NONE) ? (tiles[y][x].tileType = type) : (tiles[y][x].tileDirection = dir);
        }
    };

    // 设置Path，Cloud
    setTile("Path", Tile::PATH, Tile::NONE);
    setTile("Cloud", Tile::CLOUD, Tile::NONE);

    // 设置Carrot
    int Y = settings.value("Carrot/y").toInt();
    int X = settings.value("Carrot/x").toInt();
    tiles[Y][X].tileType = Tile::CARROT;

    // 设置Nest
    Y = settings.value("Nest/y").toInt();
    X = settings.value("Nest/x").toInt();
    tiles[Y][X].tileType = Tile::NEST;

    // 设置方向
    setTile("Left", Tile::EMPTY, Tile::LEFT);
    setTile("Right", Tile::EMPTY, Tile::RIGHT);
    setTile("Up", Tile::EMPTY, Tile::UP);
    setTile("Down", Tile::EMPTY, Tile::DOWN);

    // 测试
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << tiles[i][j].tileType << " ";
        }
        cout << endl;
    }
    cout << endl;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << tiles[i][j].tileDirection << " ";
        }
        cout << endl;
    }
}

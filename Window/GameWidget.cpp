#include "GameWidget.h"
#include "ui_GameWidget.h"

GameWidget::GameWidget(int &mapUnlock, Sound *sound, QWidget *parent)
    : QWidget(parent), ui(new Ui::GameWidget), isPause(false), isCountDown(false),
      sound(sound), gameManager(sound) {
    ui->setupUi(this);
    ui->gameView->gameManager = &gameManager;
    ui->gameView->isCountDown = &isCountDown;
    ui->gameView->sound = sound;

    // 游戏背景
    ui->background->setStyleSheet("background-image: url(:/res/Game/Framework/gameBg.png);");

    // 暂停按钮背景
    setBtnBackground(ui->pauseBtn,
                     ":/res/Game/Framework/pauseBtn.png",
                     ":/res/Game/Framework/pauseBtnP.png");

    // 菜单按钮背景
    setBtnBackground(ui->menuBtn,
                     ":/res/Game/Framework/menuBtn.png",
                     ":/res/Game/Framework/menuBtnP.png");

    // 菜单栏背景
    ui->menuBar->setObjectName("menuBar");
    ui->menuBar->setStyleSheet(
        "#menuBar {"
        "background-image: url(:/res/Game/Framework/menuBg.png);"
        "}");

    // 波次计数器背景
    ui->menuCtr->setObjectName("menuCtr");
    ui->menuCtr->setStyleSheet(
        "#menuCtr {"
        "background-image: url(:/res/Game/Framework/menuCenter.png);"
        "}");

    connect(&gameManager, &GameManager::waveChange, [this](int wave) {
        ui->waveLb->setText(QString::number(wave / 10) + "  " + QString::number(wave % 10));
    });
    connect(&gameManager, &GameManager::coinChange, [this](int coin) {
        ui->coinsLb->setText(QString::number(coin));
    });
    connect(&gameManager, &GameManager::win, [&](int hp) {
        mapUnlock++;
        win(hp);
    });
    connect(&gameManager, &GameManager::lose, this, &GameWidget::lose);
}

GameWidget::~GameWidget() { delete ui; }

// 暂停按钮
void GameWidget::on_pauseBtn_clicked() {
    if (isCountDown) return;

    sound->menuBtn();
    isPause = !isPause;
    if (isPause) {
        timer.stop();
        setBtnBackground(ui->pauseBtn,
                         ":/res/Game/Framework/continueBtn.png",
                         ":/res/Game/Framework/continueBtnP.png");
    } else {
        timer.start();
        setBtnBackground(ui->pauseBtn,
                         ":/res/Game/Framework/pauseBtn.png",
                         ":/res/Game/Framework/pauseBtnP.png");
    }
}

// 菜单按钮
void GameWidget::on_menuBtn_clicked() {
    if (isCountDown) return;

    sound->menuBtn();
    timer.stop();
    QMessageBox msgBox;
    msgBox.setWindowTitle("菜单");
    msgBox.setText("你想要做什么？");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.addButton("继续", QMessageBox::ActionRole);
    QPushButton *restartBtn = msgBox.addButton("重来", QMessageBox::ActionRole);
    QPushButton *backBtn = msgBox.addButton("选关", QMessageBox::ActionRole);
    msgBox.exec();

    if (msgBox.clickedButton() == restartBtn) {
        // 发送信号，重新加载游戏
        stopGame();
        restart();
    } else if (msgBox.clickedButton() == backBtn) {
        stopGame();
        toSelectPage();
    } else if (!isPause) {
        timer.start();
    }
}

void GameWidget::loadGame(int mapIndex) {
    // 加载地图
    auto gameView = ui->gameView;
    gameView->setMap(mapIndex);

    // 加载初始单位
    gameManager.init(mapIndex);

    // 更新画面
    gameView->updateView();

    // 倒数动画
    count = 0, isCountDown = true;
    countDown();
    connect(&countTimer, &QTimer::timeout, this, &GameWidget::countDown);
    countTimer.start(1000);

    // 游戏循环
    QTimer::singleShot(4000, [this]() {
        connect(&timer, &QTimer::timeout, this, &GameWidget::updateGame);
        timer.start(16);
    });
}

void GameWidget::stopGame() {
    timer.stop();
    disconnect(&timer, &QTimer::timeout, this, &GameWidget::updateGame);
    isPause = false;
    setBtnBackground(ui->pauseBtn,
                     ":/res/Game/Framework/pauseBtn.png",
                     ":/res/Game/Framework/pauseBtnP.png");
}


void GameWidget::updateGame() {
    if (gameManager.update()) ui->gameView->updateView();
}

void GameWidget::countDown() {
    QPixmap countImg(QString(":/res/Game/Framework/countDown%1.png").arg(count++));
    auto countItem = ui->gameView->scene->addPixmap(countImg);
    countItem->setPos(441, 173);
    if (count == 4) {
        sound->GO();
        countTimer.stop();
        disconnect(&countTimer, &QTimer::timeout, this, &GameWidget::countDown);
        QTimer::singleShot(1000, [this] {
            isCountDown = false;
        });
    } else {
        sound->countDown();
    }
}

void GameWidget::win(int hp) {
    stopGame();

    sound->win();
    QMessageBox msgBox;
    msgBox.setWindowFlags(Qt::FramelessWindowHint);

    int honor = (hp == 10 ? 2 : (hp <= 3 ? 0 : 1));
    QPixmap pixmap(QString(":/res/Game/Framework/honor%1.png").arg(honor));
    msgBox.setIconPixmap(pixmap);
    msgBox.setText("获得了" + QString(honor == 0 ? "木" : (honor == 1 ? "银" : "金")) + "萝卜！");

    auto button = msgBox.addButton("非常简单！", QMessageBox::ActionRole);
    msgBox.exec();

    if (msgBox.clickedButton() == button) {
        toSelectPage();
    }
}

void GameWidget::lose() {
    stopGame();

    sound->lose();
    QMessageBox msgBox;
    msgBox.setWindowTitle(" ");
    msgBox.setText("胡萝卜被吃掉了😭");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowFlags(Qt::FramelessWindowHint);

    auto button = msgBox.addButton("再试一次！", QMessageBox::ActionRole);
    msgBox.exec();

    if (msgBox.clickedButton() == button) {
        restart();
    }
}

GameView::GameView(QWidget *parent) : QGraphicsView(parent), scene(new QGraphicsScene) {
    setScene(scene);
    scene->setSceneRect(0, 0, 1080, 630);
}

GameView::~GameView() { delete scene; }

void GameView::setMap(int mapIndex) {
    this->mapIndex = mapIndex;
    // 路径图片
    QPixmap path(QString("Resource/Game/Path/p%1.png").arg(mapIndex));
    auto pathItem = scene->addPixmap(path);
    pathItem->setPos(0, 90);
}

// 画面更新
void GameView::updateView() {
    scene->clear();
    setMap(mapIndex);

    for (int i = 0; i < gameManager->height; i++) {
        for (int j = 0; j < gameManager->width; j++) {
            auto item = scene->addPixmap(gameManager->getTileImage(i, j));
            item->setPos(j * gameManager->tileSize - 155, i * gameManager->tileSize - 155);
        }
    }
}

const QList<QString> towerNames{"Sun", "Snow", "Star", "BStar"};

void GameView::placeMsg(int posY, int posX) {
    sound->select();

    QMessageBox msgBox;
    msgBox.setWindowTitle("放置");
    msgBox.setText("你想要放什么？");
    msgBox.setIcon(QMessageBox::Question);

    QString styleSheet = "QPushButton {"
            "border: none;"
            "background-image: url(:/res/Game/Tower/%1/I%1%2.png);"
            "}";

    // 设置图标
    vector<QPushButton *> buttons;
    vector<bool> valid;
    for (int i = 0; i < towerNames.size(); i++) {
        auto button = msgBox.addButton(" ", QMessageBox::ActionRole);
        buttons.push_back(button);
        button->setFixedSize(76, 78);
        valid.push_back(gameManager->enoughToPlace(towerNames[i]));
        button->setStyleSheet(styleSheet.arg(towerNames[i]).arg(valid[i]));
    }
    auto button = msgBox.addButton("手滑了", QMessageBox::ActionRole);
    button->setFixedSize(76, 78);
    button->setStyleSheet("QPushButton {"
        "border: none;"
        "background: transparent;"
        "}");
    msgBox.exec();

    for (int i = 0; i < towerNames.size(); i++) {
        if (msgBox.clickedButton() == buttons[i] && valid[i]) {
            gameManager->addTower(posY, posX, towerNames[i]);
            sound->place();
            return;
        }
    }
    sound->cancel();
}

void GameView::manageMsg(int posY, int posX) {
    sound->select();

    QMessageBox msgBox;
    msgBox.setWindowTitle("管理");
    msgBox.setText("你想要这个塔如何？");
    msgBox.setIcon(QMessageBox::Question);

    auto upgrade = msgBox.addButton(" ", QMessageBox::ActionRole),
            sell = msgBox.addButton(" ", QMessageBox::ActionRole),
            nothing = msgBox.addButton("手滑了", QMessageBox::ActionRole);

    auto cost = gameManager->getCost(posY, posX);
    upgrade->setText("升级 -" + QString::number(cost.first));
    sell->setText("出售 +" + QString::number(cost.second));
    bool invalid = !gameManager->enoughToUpgrade(posY, posX) || cost.first == -1;
    if (!gameManager->enoughToUpgrade(posY, posX)) {
        upgrade->setStyleSheet("text-decoration: line-through;");
    } else if (cost.first == -1) {
        upgrade->setText("顶级");
    }
    msgBox.exec();

    if (msgBox.clickedButton() == upgrade && !invalid) {
        gameManager->upgradeTower(posY, posX);
        sound->upgrade();
    } else if (msgBox.clickedButton() == sell) {
        gameManager->removeTower(posY, posX);
        sound->sell();
    } else {
        sound->cancel();
    }
}


void GameView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && !*isCountDown) {
        int posY = event->pos().y(), posX = event->pos().x();
        int doWhat = gameManager->canPlaceTower(posY, posX);
        if (doWhat == 1) {
            manageMsg(posY, posX);
        } else if (doWhat == 2) {
            placeMsg(posY, posX);
        } else {
            sound->selectFault();
        }
    }
    QGraphicsView::mousePressEvent(event);
}

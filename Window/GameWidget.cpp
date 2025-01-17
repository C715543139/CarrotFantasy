#include "GameWidget.h"
#include "ui_GameWidget.h"

GameWidget::GameWidget(QWidget *parent) : QWidget(parent), ui(new Ui::GameWidget), isPause(false), isCountDown(false) {
    ui->setupUi(this);
    ui->gameView->gameManager = &gameManager;
    ui->gameView->isCountDown = &isCountDown;

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
}

GameWidget::~GameWidget() { delete ui; }

// 暂停按钮
void GameWidget::on_pauseBtn_clicked() {
    if (isCountDown) return;

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

    timer.stop();
    auto msgBox = new QMessageBox;
    msgBox->setWindowTitle("菜单");
    msgBox->setText("你想要做什么？");
    msgBox->setIcon(QMessageBox::Question);
    msgBox->addButton("继续", QMessageBox::ActionRole);
    QPushButton *restartBtn = msgBox->addButton("重来", QMessageBox::ActionRole);
    QPushButton *backBtn = msgBox->addButton("选关", QMessageBox::ActionRole);
    msgBox->exec();

    if (msgBox->clickedButton() == restartBtn) {
        // 发送信号，重新加载游戏
        stopGame();
        restart();
    } else if (msgBox->clickedButton() == backBtn) {
        stopGame();
        toSelectPage();
    } else if (!isPause) {
        timer.start();
    }
    delete msgBox;
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

    // TODO:gameManager

    // gameView
}


void GameWidget::updateGame() {
    gameManager.update();
    ui->gameView->updateView();
}

void GameWidget::countDown() {
    QPixmap countImg(QString(":/res/Game/Framework/countDown%1.png").arg(count++));
    auto countItem = ui->gameView->scene->addPixmap(countImg);
    countItem->setPos(441, 173);
    if (count == 4) {
        countTimer.stop();
        disconnect(&countTimer, &QTimer::timeout, this, &GameWidget::countDown);
        QTimer::singleShot(1000, [this] {
            isCountDown = false;
        });
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
    QPixmap path(QString(":/res/Game/Path/p%1.png").arg(mapIndex));
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
            item->setPos(j * gameManager->tileSize - 105, i * gameManager->tileSize - 105);
        }
    }
}

void GameView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && !*isCountDown) {
        QPoint pos = event->pos();
        gameManager->addTower(pos.y(), pos.x(), "Star");
    }
    QGraphicsView::mousePressEvent(event);
}

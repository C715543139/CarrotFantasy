#include "GameWidget.h"
#include "ui_GameWidget.h"

GameWidget::GameWidget(QWidget *parent) : QWidget(parent), ui(new Ui::GameWidget), isPause(false) {
    ui->setupUi(this);

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
}

GameWidget::~GameWidget() { delete ui; }

// 暂停按钮
void GameWidget::on_pauseBtn_clicked() {
    isPause = !isPause;
    if (isPause) {
        timer.stop();
        setBtnBackground(ui->pauseBtn,
                         ":/res/Game/Framework/continueBtn.png",
                         ":/res/Game/Framework/continueBtnP.png");
    } else {
        timer.start(16);
        setBtnBackground(ui->pauseBtn,
                         ":/res/Game/Framework/pauseBtn.png",
                         ":/res/Game/Framework/pauseBtnP.png");
    }
}

// 菜单按钮
void GameWidget::on_menuBtn_clicked() {
    timer.stop();
    auto msgBox = new QMessageBox;
    msgBox->setWindowTitle("菜单");
    msgBox->setText("你想要做什么？");
    msgBox->setIcon(QMessageBox::Question);
    QPushButton *continueBtn = msgBox->addButton("继续", QMessageBox::ActionRole);
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
        timer.start(16);
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
    gameView->update(gameManager);

    // TODO:倒数动画

    // 游戏循环
    connect(&timer, &QTimer::timeout, this, &GameWidget::updateGame);
    timer.start(16);
}

void GameWidget::stopGame() {
    timer.stop();
    isPause = false;
    setBtnBackground(ui->pauseBtn,
                 ":/res/Game/Framework/pauseBtn.png",
                 ":/res/Game/Framework/pauseBtnP.png");

    // TODO:gameManager

    // gameView

}


void GameWidget::updateGame() {
    gameManager.update();
    ui->gameView->update(gameManager);
    cout << "update game" << endl;
}


GameView::GameView(QWidget *parent) : QGraphicsView(parent), scene(new QGraphicsScene) {
    setScene(scene);
    scene->setSceneRect(0, 0, 1080, 630);
}

GameView::~GameView() { delete scene; }

void GameView::setMap(int mapIndex) {
    // 路径图片
    QPixmap path(QString(":/res/Game/Path/p%1.png").arg(mapIndex));
    scene->clear();
    auto pathItem = scene->addPixmap(path);
    pathItem->setPos(0, 90);
}

// 画面更新
void GameView::update(GameManager &gameManager) {

}

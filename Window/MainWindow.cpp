#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      settings("save.ini", QSettings::IniFormat) {
    ui->setupUi(this);

    // 加载存档
    mapUnlock = settings.value("Map/index").toInt();

    setFixedSize(size()); // 固定窗口大小

    sound = new Sound;
    mainWidget = new MainWidget(sound);
    selectWidget = new SelectWidget(mapUnlock, sound);
    gameWidget = new GameWidget(mapUnlock, sound);
    loadingWidget = new QWidget;
    loadingWidget->setStyleSheet(
        "width: 1080px;"
        "height: 720px;"
        "background-image: url(:/res/SelectScene/loading.jpeg);");

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(mainWidget);
    stackedWidget->addWidget(selectWidget);
    stackedWidget->addWidget(gameWidget);
    stackedWidget->addWidget(loadingWidget);
    setCentralWidget(stackedWidget); // 将页面管理器显示在窗口
    sound->playBGM();

    connect(mainWidget, &MainWidget::toSelectPage, [this] { stackedWidget->setCurrentIndex(1); });
    connect(selectWidget, &SelectWidget::toMainPage, [this] { stackedWidget->setCurrentIndex(0); });
    connect(selectWidget, &SelectWidget::toGamePage, [this] { gameStart(); });
    connect(gameWidget, &GameWidget::restart, [this] { gameStart(); });
    connect(gameWidget, &GameWidget::toSelectPage, [this] {
        sound->playBGM();
        stackedWidget->setCurrentIndex(1);
    });
}

MainWindow::~MainWindow() {
    settings.setValue("Map/index", mapUnlock);
    delete stackedWidget;
    delete ui;
}

void MainWindow::gameStart() {
    sound->stopBGM();
    // 加载动画
    stackedWidget->setCurrentIndex(3);
    QTimer::singleShot(1000, [&] {
        stackedWidget->setCurrentIndex(2);
        // 开始游戏
        gameWidget->loadGame(selectWidget->mapIndex());
    });
}

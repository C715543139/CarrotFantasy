#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setFixedSize(size()); // 固定窗口大小

    mainWidget = new MainWidget;
    gameWidget = new GameWidget;
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(mainWidget);
    stackedWidget->addWidget(gameWidget);
    setCentralWidget(stackedWidget); // 将页面管理器显示在窗口

    connect(mainWidget, &MainWidget::toGamePage, [this](){stackedWidget->setCurrentIndex(1);});
    connect(gameWidget, &GameWidget::toMainPage, [this](){stackedWidget->setCurrentIndex(0);});
}

MainWindow::~MainWindow() {
    delete mainWidget;
    delete gameWidget;
    delete stackedWidget;
    delete ui;
}

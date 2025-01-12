#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setFixedSize(size()); // 固定窗口大小

    mainWidget = new MainWidget;
    selectWidget = new SelectWidget;
    gameWidget = new GameWidget;
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(mainWidget);
    stackedWidget->addWidget(selectWidget);
    stackedWidget->addWidget(gameWidget);
    setCentralWidget(stackedWidget); // 将页面管理器显示在窗口

    connect(mainWidget, &MainWidget::toSelectPage, [this] {stackedWidget->setCurrentIndex(1);});
    connect(selectWidget, &SelectWidget::toMainPage, [this] {stackedWidget->setCurrentIndex(0);});
    connect(selectWidget, &SelectWidget::toGamePage, [this] {stackedWidget->setCurrentIndex(2);});
    connect(gameWidget, &GameWidget::toSelectPage, [this] {stackedWidget->setCurrentIndex(1);});
}

MainWindow::~MainWindow() {
    delete stackedWidget;
    delete ui;
}

#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    mainWidget = new MainWidget;
    gameWidget = new GameWidget;
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(mainWidget);
    stackedWidget->addWidget(gameWidget);
    setCentralWidget(stackedWidget);

    connect(mainWidget, &MainWidget::toGamePage, [this](){stackedWidget->setCurrentIndex(1);});
    connect(gameWidget, &GameWidget::toMainPage, [this](){stackedWidget->setCurrentIndex(0);});
}

MainWindow::~MainWindow() { delete ui; }

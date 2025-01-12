#include "MainWidget.h"
#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    // 主页面背景
    movie = new QMovie(":/res/MainScene/mainBg.gif");
    ui->background->setMovie(movie);
    movie->start();

    // 开始按钮背景
    setBtnBackground(ui->startBtn,
                     ":/res/MainScene/startBtn.png",
                     ":/res/MainScene/startBtnP.png");

    connect(ui->startBtn, SIGNAL(clicked()), this, SIGNAL(toSelectPage()));
}

MainWidget::~MainWidget() {
    delete movie;
    delete ui;
}

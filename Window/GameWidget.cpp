#include "GameWidget.h"
#include "ui_GameWidget.h"

GameWidget::GameWidget(QWidget *parent) : QWidget(parent), ui(new Ui::GameWidget), isPause(false) {
    ui->setupUi(this);

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
        "background-image: url(:/res/Game/Framework/MenuBg.png);"
        "}");
}

GameWidget::~GameWidget() { delete ui; }

// 暂停按钮
void GameWidget::on_pauseBtn_clicked() {
    isPause = !isPause;
    if (isPause) {
        setBtnBackground(ui->pauseBtn,
                         ":/res/Game/Framework/continueBtn.png",
                         ":/res/Game/Framework/continueBtnP.png");
    } else {
        setBtnBackground(ui->pauseBtn,
                         ":/res/Game/Framework/pauseBtn.png",
                         ":/res/Game/Framework/pauseBtnP.png");
    }
}

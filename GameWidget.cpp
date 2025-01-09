#include "GameWidget.h"
#include "ui_GameWidget.h"

GameWidget::GameWidget(QWidget *parent) : QWidget(parent), ui(new Ui::GameWidget) {
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SIGNAL(toMainPage()));
}

GameWidget::~GameWidget() { delete ui; }

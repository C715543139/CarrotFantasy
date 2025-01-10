#include "SelectWidget.h"
#include "ui_SelectWidget.h"

SelectWidget::SelectWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SelectWidget) {
    ui->setupUi(this);

    connect(ui->backBtn, SIGNAL(clicked()), this, SIGNAL(toMainPage()));
    connect(ui->playBtn, SIGNAL(clicked()), this, SIGNAL(toGamePage()));
}

SelectWidget::~SelectWidget() { delete ui; }

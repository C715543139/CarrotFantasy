#include "GameWindow.h"
#include "ui_GameWindow.h"

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GameWindow) { ui->setupUi(this); }

GameWindow::~GameWindow() { delete ui; }

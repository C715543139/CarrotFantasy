#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "GameWidget.h"
#include "MainWidget.h"
#include "SelectWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MainWidget *mainWidget; // 主页面
    SelectWidget *selectWidget;
    GameWidget *gameWidget; // 游戏页面
    QStackedWidget *stackedWidget; // 页面管理器
};
#endif // MAINWINDOW_H

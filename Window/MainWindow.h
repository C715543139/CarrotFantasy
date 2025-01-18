#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QTimer>
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
    void gameStart();

private:
    int mapUnlock;
    Ui::MainWindow *ui;
    MainWidget *mainWidget; // 主页面
    SelectWidget *selectWidget; // 选择页面
    GameWidget *gameWidget; // 游戏页面
    QWidget *loadingWidget;
    QStackedWidget *stackedWidget; // 页面管理器
    Sound *sound;
    QSettings settings;
};
#endif // MAINWINDOW_H

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "MainWindow.h"
#include "GameWindow.h"

// 用于管理不同页面的显示切换
class WindowManager : public QObject {
    Q_OBJECT
public:
    WindowManager();
    void showMainWindow(); // 显示主页面
    void showGameWindow(); // 显示游戏页面

private:
    MainWindow *mainWindow; // 主页面
    GameWindow *gameWindow; // 游戏页面

    void setupConnections(); // 建立信号链接
};

#endif // WINDOWMANAGER_H

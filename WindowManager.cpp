#include "WindowManager.h"


WindowManager::WindowManager(): mainWindow(new MainWindow), gameWindow(new GameWindow) {
    setupConnections();
}

void WindowManager::showMainWindow() {
    mainWindow->show();
}

void WindowManager::showGameWindow() {
    mainWindow->hide();
    gameWindow->show();
}


void WindowManager::setupConnections() {
    connect(mainWindow, &MainWindow::switchToGame, this, &WindowManager::showGameWindow);
}

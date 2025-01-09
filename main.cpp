#include "WindowManager.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    WindowManager manager;
    manager.showMainWindow();
    return a.exec();
}

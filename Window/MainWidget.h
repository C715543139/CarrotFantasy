#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMovie>
#include "../Core/Sound.h"
#include "WindowTool.h"


namespace Ui {
    class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    MainWidget(Sound *sound, QWidget *parent = nullptr);
    ~MainWidget();

signals:
    void toSelectPage();

private:
    Ui::MainWidget *ui;
    QMovie *movie;
    Sound *sound;
};

#endif // MAINWIDGET_H

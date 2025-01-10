#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "WindowTool.h"

namespace Ui {
    class GameWidget;
}

class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

signals:
    void toSelectPage();

private slots:
    void on_pauseBtn_clicked();

private:
    Ui::GameWidget *ui;
    bool isPause;
};

#endif // GAMEWIDGET_H

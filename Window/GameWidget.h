#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QGraphicsView>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "../Core/Map.h"
#include "WindowTool.h"

namespace Ui {
    class GameWidget;
}

class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
    void loadGame(int map);

signals:
    void toSelectPage();
    void restart();

private slots:
    void on_pauseBtn_clicked();
    void on_menuBtn_clicked();

private:
    Ui::GameWidget *ui;
    bool isPause;
};

class GameView : public QGraphicsView {
    Q_OBJECT

    public:
    explicit GameView(QWidget *parent = nullptr);
    ~GameView();
    void setMap(int map);

private:
    QGraphicsScene *scene;
};

#endif // GAMEWIDGET_H

#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QGraphicsView>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMouseEvent>
#include "../Core/GameManager.h"
#include "WindowTool.h"

namespace Ui {
    class GameWidget;
}

class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
    void loadGame(int mapIndex);
    void stopGame();

signals:
    void toSelectPage();
    void restart();

private slots:
    void on_pauseBtn_clicked();
    void on_menuBtn_clicked();
    void updateGame();
    void countDown();

private:
    Ui::GameWidget *ui;
    bool isPause, isCountDown;
    int count;
    QTimer timer, countTimer;
    GameManager gameManager;
};

class GameView : public QGraphicsView {
    Q_OBJECT

public:
    friend class GameWidget;

    explicit GameView(QWidget *parent = nullptr);
    ~GameView();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    int mapIndex;
    bool *isCountDown;
    GameManager *gameManager;
    QGraphicsScene *scene;

    void setMap(int mapIndex);
    void updateView();
    void placeMsg(int posY, int posX);
    void manageMsg(int posY, int posX);
};

#endif // GAMEWIDGET_H

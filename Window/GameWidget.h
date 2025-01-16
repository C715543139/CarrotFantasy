#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QGraphicsView>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
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

private:
    Ui::GameWidget *ui;
    bool isPause;
    QTimer timer;
    GameManager gameManager;
};

class GameView : public QGraphicsView {
    Q_OBJECT

    public:
    explicit GameView(QWidget *parent = nullptr);
    ~GameView();
    void setMap(int mapIndex);
    void update(GameManager &gameManager);

private:
    QGraphicsScene *scene;
};

#endif // GAMEWIDGET_H

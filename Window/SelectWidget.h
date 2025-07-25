#ifndef SELECTWIDGET_H
#define SELECTWIDGET_H

#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QMessageBox>
#include "../Core/Sound.h"
#include "WindowTool.h"

namespace Ui {
    class SelectWidget;
}

class SelectWidget : public QWidget {
    Q_OBJECT

public:
    SelectWidget(int &mapUnlock, Sound *sound, QWidget *parent = nullptr);
    ~SelectWidget();
    int mapIndex();

signals:
    void toMainPage();
    void toGamePage();

private:
    Ui::SelectWidget *ui;
    Sound *sound;

    bool checkMapUnlock(int &mapUnlock);
};

class Slideshow : public QWidget {
    Q_OBJECT

public:
    explicit Slideshow(QWidget *parent = nullptr);
    ~Slideshow();
    void addImage(const QString &path);
    void nextImage();
    void previousImage();
    friend SelectWidget;

private:
    int currentIndex = 0;
    bool isAnimating = false;
    QLabel *currentLabel;
    QLabel *nextLabel;
    QList<QPixmap> images;

    void switchToImage(int newIndex, bool forward);
};

#endif // SELECTWIDGET_H

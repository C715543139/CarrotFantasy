#ifndef SELECTWIDGET_H
#define SELECTWIDGET_H

#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QMessageBox>
#include "WindowTool.h"

namespace Ui {
    class SelectWidget;
}

class SelectWidget : public QWidget {
    Q_OBJECT

public:
    explicit SelectWidget(QWidget *parent = nullptr);
    ~SelectWidget();
    int mapIndex();

signals:
    void toMainPage();
    void toGamePage();

private:
    Ui::SelectWidget *ui;
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
    QLabel *currentLabel;
    QLabel *nextLabel;
    QList<QPixmap> images;
    int currentIndex = 0;
    bool isAnimating = false;

    void switchToImage(int newIndex, bool forward);
};

#endif // SELECTWIDGET_H

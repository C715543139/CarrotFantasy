#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMovie>
#include "WindowTool.h"


namespace Ui {
    class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

signals:
    void toSelectPage();

private:
    Ui::MainWidget *ui;
    QMovie *movie;
};

#endif // MAINWIDGET_H

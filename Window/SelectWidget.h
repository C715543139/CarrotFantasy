#ifndef SELECTWIDGET_H
#define SELECTWIDGET_H

#include <QWidget>

namespace Ui {
    class SelectWidget;
}

class SelectWidget : public QWidget {
    Q_OBJECT

public:
    explicit SelectWidget(QWidget *parent = nullptr);
    ~SelectWidget();

signals:
    void toMainPage();
    void toGamePage();

private:
    Ui::SelectWidget *ui;
};

#endif // SELECTWIDGET_H

#ifndef WINDOWTOOL_H
#define WINDOWTOOL_H

#include <QAbstractButton>

inline void setBtnBackground(QAbstractButton *btn, const QString &imgPath, const QString &imgPathP) {
    btn->setStyleSheet(QString(
        "QPushButton {"
        "background-image: url(%1);"
        "border: none;"
        "}"
        "QPushButton:pressed {"
        "background-image: url(%2);"
        "}").arg(imgPath, imgPathP));
}

#endif // WINDOWTOOL_H

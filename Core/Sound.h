#ifndef SOUND_H
#define SOUND_H

#include <QRandomGenerator>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QHash>
#include <QDir>

class Sound : public QObject {
    Q_OBJECT

public:
    Sound(QObject *parent = nullptr);
    void playBGM() const;
    void stopBGM() const;
    void countDown();
    void GO();
    void lose();
    void win();
    void born();
    void menuBtn();
    void playBtn();
    void place();
    void sell();
    void select();
    void cancel();
    void selectFault();
    void upgrade();
    void monster();
    void crash();
    void tower(const QString &name);

private:
    QMediaPlayer *player; // 背景音乐
    QAudioOutput *audioOutput;
    QHash<QString, QSoundEffect *> game, towers;
    QList<QSoundEffect *> monsters;
};

#endif // SOUND_H

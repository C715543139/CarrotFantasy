#include "Sound.h"

Sound::Sound(QObject *parent) : QObject(parent) {
    QDir directory(":/sound/Music/Game");
    QStringList files = directory.entryList(QDir::Files);
    for (const QString &file: files) {
        QString soundName = QFileInfo(file).baseName();
        QSoundEffect *effect = new QSoundEffect(this);
        effect->setSource(QUrl("qrc:/sound/Music/Game/" + file));
        game[soundName] = effect;
    }

    directory.setPath(":/sound/Music/Towers");
    files = directory.entryList(QDir::Files);
    for (const QString &file: files) {
        QString soundName = QFileInfo(file).baseName();
        QSoundEffect *effect = new QSoundEffect(this);
        effect->setSource(QUrl("qrc:/sound/Music/Towers/" + file));
        towers[soundName] = effect;
    }

    directory.setPath(":/monsterSound/Music/Monsters");
    files = directory.entryList(QDir::Files);
    for (const QString &file: files) {
        QString soundName = QFileInfo(file).baseName();
        QSoundEffect *effect = new QSoundEffect(this);
        effect->setSource(QUrl("qrc:/monsterSound/Music/Monsters/" + file));
        monsters.push_back(effect);
    }

    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl("qrc:/sound/Music/Main/BGMusic.mp3"));
    player->setLoops(QMediaPlayer::Infinite);
}

void Sound::playBGM() const {
    player->play();
}

void Sound::stopBGM() const {
    player->stop();
}

void Sound::countDown() {
    game["CountDown"]->play();
}

void Sound::GO() {
    game["GO"]->play();
}

void Sound::lose() {
    game["Lose"]->play();
}

void Sound::win() {
    game["Perfect"]->play();
}

void Sound::born() {
    game["MC"]->play();
}

void Sound::menuBtn() {
    game["MenuSelect"]->play();
}

void Sound::playBtn() {
    game["Select"]->play();
}

void Sound::place() {
    game["TowerBulid"]->play();
}

void Sound::sell() {
    game["TowerSell"]->play();
}

void Sound::select() {
    game["TowerSelect"]->play();
}

void Sound::cancel() {
    game["TowerDeselect"]->play();
}

void Sound::selectFault() {
    game["SelectFault"]->play();
}


void Sound::upgrade() {
    game["TowerUpdata"]->play();
}

void Sound::monster() {
    monsters[rand() % monsters.size()]->play();
}

void Sound::crash() {
    game["Crash"]->play();
}


void Sound::tower(const QString &name) {
    towers[name]->play();
}

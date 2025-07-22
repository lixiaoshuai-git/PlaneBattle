#ifndef MUSIC_H
#define MUSIC_H
#include <QSoundEffect>

class Music: public QSoundEffect
{
public:
    Music();
    QSoundEffect * bgMusic= new QSoundEffect() ;
    QSoundEffect * bombMusic =new QSoundEffect();
    QSoundEffect * bossMusic =new QSoundEffect();
    QSoundEffect * menuMusic =new QSoundEffect();
    QSoundEffect * shootMusic =new QSoundEffect();
    QSoundEffect * selectMusic =new QSoundEffect();


    QUrl bg = QUrl::fromLocalFile(":/new/res/bg.wav");
    QUrl bomb = QUrl::fromLocalFile(":/new/res/bomb.wav");
    QUrl boss = QUrl::fromLocalFile(":/new/res/boss.wav");
    QUrl menu = QUrl::fromLocalFile(":/new/res/menu.wav");
    QUrl shoot = QUrl::fromLocalFile(":/new/res/shoot.wav");
    QUrl select = QUrl::fromLocalFile(":/new/res/select.wav");

    float bgv=0.5;
    float bossv=0.8;
    float menuv=0.8;

    void playbackMusic(QSoundEffect * music,float volume);
    void stopMusic(QSoundEffect * music);
    void playbombEffect();
    void playshootEffect();
    void playselectEffect();


};

#endif // MUSIC_H

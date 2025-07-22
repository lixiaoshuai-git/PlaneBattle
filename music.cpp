#include "music.h"

Music::Music():QSoundEffect()
{
    bgMusic->setSource(bg);
    bombMusic->setSource(bomb);
    bossMusic->setSource(boss);
    menuMusic->setSource(menu);
    shootMusic->setSource(shoot);
    selectMusic->setSource(select);

}

void Music::playbackMusic(QSoundEffect * music,float volume)
{
    music->setLoopCount(QSoundEffect::Infinite);
    music->setVolume(volume);
    music->play();
}

void Music::stopMusic(QSoundEffect * music)
{
    music->stop();
}
void Music::playbombEffect()
{
    stopMusic(bombMusic);
    bombMusic->setVolume(1);
    bombMusic->play();
}
void Music::playshootEffect()
{
    stopMusic(shootMusic);
    shootMusic->setVolume(1);
    shootMusic->play();
}
void Music::playselectEffect()
{
    stopMusic(selectMusic);
    selectMusic->setVolume(1);
    selectMusic->play();
}


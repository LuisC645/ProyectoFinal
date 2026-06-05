#include "audiomanager.h"

AudioManager::AudioManager()
{
    menuMusic = new QMediaPlayer();

    menuOutput = new QAudioOutput();

    menuMusic->setAudioOutput(
        menuOutput
        );

    menuOutput->setVolume(
        0.5f
        );

    menuMusic->setSource(
        QUrl("qrc:/new/prefix1/resources/MainMenu.mp3")
        );

    QObject::connect(
        menuMusic,
        &QMediaPlayer::mediaStatusChanged,
        [this](QMediaPlayer::MediaStatus status)
        {
            if(status == QMediaPlayer::EndOfMedia)
            {
                menuMusic->setPosition(0);
                menuMusic->play();
            }
        }
        );
}

void AudioManager::playMenuMusic()
{
    menuMusic->setPosition(0);

    menuMusic->play();
}

void AudioManager::stopMenuMusic()
{
    menuMusic->stop();
}

#include "audiomanager.h"

AudioManager::AudioManager()
{
    // Menu
    menuMusic = new QMediaPlayer();
    menuOutput = new QAudioOutput();

    menuMusic->setAudioOutput(menuOutput);

    menuOutput->setVolume(0.5f);

    menuMusic->setSource(QUrl("qrc:/new/prefix1/resources/MainMenu.mp3"));

    QObject::connect(menuMusic, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia){
            menuMusic->setPosition(0);
            menuMusic->play();
        }
    });

    // Juego
    gameMusic = new QMediaPlayer();
    gameOutput = new QAudioOutput();

    gameMusic->setAudioOutput(gameOutput);
    gameOutput->setVolume(0.4f);

    gameMusic->setSource( QUrl("qrc:/new/prefix1/resources/inGame.mp3"));

    QObject::connect(gameMusic, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia)
        {
            gameMusic->setPosition(0);
            gameMusic->play();
        }
    });

    // Fruta
    fruitSound = new QMediaPlayer();
    fruitOutput = new QAudioOutput();

    fruitSound->setAudioOutput(fruitOutput);
    fruitOutput->setVolume(1.0f);
    fruitSound->setSource(QUrl("qrc:/new/prefix1/resources/fruit.mp3"));

    // hit
    hitSound = new QMediaPlayer();
    hitOutput = new QAudioOutput();

    hitSound->setAudioOutput(hitOutput);
    hitOutput->setVolume(1.0f);
    hitSound->setSource(QUrl("qrc:/new/prefix1/resources/hit.mp3"));
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

void AudioManager::playGameMusic()
{
    menuMusic->stop();
    gameMusic->setPosition(0);
    gameMusic->play();
}

void AudioManager::stopGameMusic()
{
    gameMusic->stop();
}

void AudioManager::playFruitSound()
{
    fruitSound->setPosition(0);
    fruitSound->play();
}

void AudioManager::playHitSound()
{
    hitSound->setPosition(0);
    hitSound->play();
}


#include "audiomanager.h"
#include <stdexcept>

// Inicializa recursos de audio
AudioManager::AudioManager()
{
    try
    {
        // Menu
        menuMusic = new QMediaPlayer();
        menuOutput = new QAudioOutput();

        menuMusic->setAudioOutput(menuOutput);

        menuOutput->setVolume(0.5f);

        menuMusic->setSource(QUrl("qrc:/new/prefix1/resources/MainMenu.mp3"));

        if(menuMusic->source().isEmpty()){ throw std::runtime_error("Menu audio"); }

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

        gameMusic->setSource(QUrl("qrc:/new/prefix1/resources/inGame.mp3"));

        if(gameMusic->source().isEmpty())
            throw std::runtime_error("Game audio");

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

        if(fruitSound->source().isEmpty()){ throw std::runtime_error("Fruit audio"); }

        // Hit
        hitSound = new QMediaPlayer();
        hitOutput = new QAudioOutput();

        hitSound->setAudioOutput(hitOutput);
        hitOutput->setVolume(1.0f);
        hitSound->setSource(QUrl("qrc:/new/prefix1/resources/hit.mp3"));

        if(hitSound->source().isEmpty()){ throw std::runtime_error("Hit audio"); }
    }
    catch(...)
    {
        qFatal("Error cargando recursos de audio");
    }
}

// Reproduce musica del menu
void AudioManager::playMenuMusic()
{
    menuMusic->setPosition(0);
    menuMusic->play();
}

// Detiene musica del menu
void AudioManager::stopMenuMusic()
{
    menuMusic->stop();
}

// Reproduce musica del juego
void AudioManager::playGameMusic()
{
    menuMusic->stop();
    gameMusic->setPosition(0);
    gameMusic->play();
}

// Detiene musica del juego
void AudioManager::stopGameMusic()
{
    gameMusic->stop();
}

// Reproduce sonido de fruta
void AudioManager::playFruitSound()
{
    fruitSound->setPosition(0);
    fruitSound->play();
}

// Reproduce sonido de daño
void AudioManager::playHitSound()
{
    hitSound->setPosition(0);
    hitSound->play();
}

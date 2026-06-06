#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QMediaPlayer>
#include <QAudioOutput>

class AudioManager
{
private:

    QMediaPlayer* menuMusic;
    QAudioOutput* menuOutput;

    QMediaPlayer* gameMusic;
    QAudioOutput* gameOutput;

    QMediaPlayer* fruitSound;
    QAudioOutput* fruitOutput;

    QMediaPlayer* hitSound;
    QAudioOutput* hitOutput;

public:

    // Inicializa recursos de audio
    AudioManager();

    // Reproduce musica del menu
    void playMenuMusic();

    // Detiene musica del menu
    void stopMenuMusic();

    // Reproduce musica del juego
    void playGameMusic();

    // Detiene musica del juego
    void stopGameMusic();

    // Reproduce sonido de fruta
    void playFruitSound();

    // Reproduce sonido de daño
    void playHitSound();
};

#endif

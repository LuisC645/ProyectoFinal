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

    AudioManager();

    void playMenuMusic();
    void stopMenuMusic();

    void playGameMusic();
    void stopGameMusic();

    void playFruitSound();
    void playHitSound();
};

#endif

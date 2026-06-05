#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QMediaPlayer>
#include <QAudioOutput>

class AudioManager
{
private:

    QMediaPlayer* menuMusic;
    QAudioOutput* menuOutput;

public:

    AudioManager();

    void playMenuMusic();
    void stopMenuMusic();
};

#endif // AUDIOMANAGER_H

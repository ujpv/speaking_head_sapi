#ifndef SIMPLESPEAKER_H
#define SIMPLESPEAKER_H

#include "abstractspeaker.h"

#include "ofSoundPlayer.h"

class SimpleSpeaker : public AbstractSpeaker
{
public:
    SimpleSpeaker();

    void update();
    void speak(std::wstring);

private:
    ofSoundPlayer player;
};

#endif // SIMPLESPEAKER_H

#ifndef SOUND_CONTROL_H
#define SOUND_CONTROL_H

#include <fmod.hpp>

#define SOUND_ROTATE            0
#define SOUND_MOVE              1
#define SOUND_CLEAR_LINE        2
#define SOUND_LEVEL_UP          3
#define SOUND_MUSIC             4
#define SOUND_ACCEL             5
#define SOUND_SFX               6


class SoundControl {
    public:
        SoundControl();

        void Play(int SoundID);
        void Stop(int SoundID);

        bool IsMusicEnabled();
        bool IsSFXEnabled();
        void SetSFXState(bool State);

        bool Setup();
    private:
        bool m_musicEnabled;
        bool m_sfxEnabled;
        FMOD::Channel *m_rotateChannel;
        FMOD::Channel *m_moveChannel;
        FMOD::Channel *m_clearChannel;
        FMOD::Channel *m_levelUpChannel;
        FMOD::Channel *m_musicChannel;
        FMOD::Channel *m_accelChannel;

        FMOD::System *system;

        FMOD::Sound *rotate;
        FMOD::Sound *move;
        FMOD::Sound *clear;
        FMOD::Sound *levelup;
        FMOD::Sound *music;
        FMOD::Sound *accel;

};

#endif

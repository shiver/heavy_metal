#include "soundcontrol.h"

#include <stdio.h>

SoundControl *sound;

SoundControl::SoundControl() {
    Setup();
    m_sfxEnabled = true;
    m_musicEnabled = true;
}

bool SoundControl::Setup() {
    FMOD_RESULT result;

    result = FMOD::System_Create(&system);
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, result);
        return false;
    }

    result = system->init(100, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, result);
        return false;
    }

    result = system->createSound("rotate.ogg", FMOD_DEFAULT, 0, &rotate);
    result = system->createSound("move.ogg", FMOD_DEFAULT, 0, &move);
    result = system->createSound("clear.ogg", FMOD_DEFAULT, 0, &clear);
    result = system->createSound("levelup.ogg", FMOD_DEFAULT, 0, &levelup);
    result = system->createSound("music.ogg", FMOD_LOOP_NORMAL, 0, &music);
    result = system->createSound("accel.ogg", FMOD_DEFAULT, 0, &accel);
    if (result != FMOD_OK)
        return false;


    return true;
}

void SoundControl::SetSFXState(bool State) {
    m_sfxEnabled = State;
}

bool SoundControl::IsMusicEnabled() {
    return m_musicEnabled;
}

bool SoundControl::IsSFXEnabled() {
    return m_sfxEnabled;
}

void SoundControl::Stop(int SoundID) {
    FMOD_RESULT result;

    switch (SoundID) {
        case SOUND_ROTATE:
            result = m_rotateChannel->stop();
            break;
        case SOUND_MOVE:
            result = m_moveChannel->stop();
            break;
        case SOUND_CLEAR_LINE:
            result = m_clearChannel->stop();
            break;
        case SOUND_LEVEL_UP:
            result = m_levelUpChannel->stop();
            break;
        case SOUND_MUSIC:
            result = m_musicChannel->stop();
            m_musicEnabled = false;
            break;
        case SOUND_ACCEL:
            result = m_accelChannel->stop();
            break;
        case SOUND_SFX:
            result = m_rotateChannel->stop();
            result = m_moveChannel->stop();
            result = m_clearChannel->stop();
            result = m_levelUpChannel->stop();
            result = m_accelChannel->stop();
            m_sfxEnabled = false;
            break;
        default:
            break;
    }
}

void SoundControl::Play(int SoundID) {
    FMOD_RESULT result;

    switch (SoundID) {
        case SOUND_ROTATE:
            if (m_sfxEnabled)
                result = system->playSound(FMOD_CHANNEL_REUSE, rotate, false, &m_rotateChannel);
            break;
        case SOUND_MOVE:
            if (m_sfxEnabled)
                result = system->playSound(FMOD_CHANNEL_REUSE, move, false, &m_moveChannel);
            break;
        case SOUND_CLEAR_LINE:
            if (m_sfxEnabled)
                result = system->playSound(FMOD_CHANNEL_REUSE, clear, false, &m_clearChannel);
            break;
        case SOUND_LEVEL_UP:
            if (m_sfxEnabled)
                result = system->playSound(FMOD_CHANNEL_REUSE, levelup, false, &m_levelUpChannel);
            break;
        case SOUND_MUSIC:
            if (m_sfxEnabled)
                result = system->playSound(FMOD_CHANNEL_REUSE, music, false, &m_musicChannel);
            m_musicEnabled = true;
            break;
        case SOUND_ACCEL:
            if (m_sfxEnabled)
                result = system->playSound(FMOD_CHANNEL_REUSE, accel, false, &m_accelChannel);
            break;
        default:
            break;
    }
}

#pragma once

#include "Daybreak/Audio/AudioClip.h"

#include "miniaudio.h"

namespace Daybreak
{
    class AudioEngine
    {
    public:
        static void Init();
        static void Shutdown();

        static void Play(const Ref<AudioClip>& clip);
    private:
        static ma_engine s_Engine;
    };
}
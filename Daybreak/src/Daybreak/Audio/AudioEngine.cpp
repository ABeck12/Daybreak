#include "dbpch.h"

#include "Daybreak/Audio/AudioEngine.h"

#include "miniaudio.h"

namespace Daybreak
{
    ma_engine AudioEngine::s_Engine = ma_engine();

    void AudioEngine::Init()
    {
        ma_result result = ma_engine_init(NULL, &AudioEngine::s_Engine);
        if (result != MA_SUCCESS) 
            DB_CORE_ASSERT(false, "Audio Engine initialization failed!");

        DB_CORE_INFO("Audio Info:");
        DB_CORE_INFO("  Device: {0}", s_Engine.pDevice->playback.name);
        // DB_LOG()
    }

    void AudioEngine::Shutdown()
    {

    }

    void AudioEngine::Play(const Ref<AudioClip>& clip)
    {
          ma_engine_play_sound(&s_Engine, clip->GetFilepath().c_str(), NULL);
    }
}
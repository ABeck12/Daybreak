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
    }

    void AudioEngine::Shutdown()
    {

    }

    void AudioEngine::Play(const Ref<AudioClip>& clip)
    {
          ma_engine_play_sound(&s_Engine, clip->m_Filepath.c_str(), NULL);
    }
}
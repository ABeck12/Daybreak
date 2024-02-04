#include "dbpch.h"

#include "Daybreak/Audio/AudioClip.h"

namespace Daybreak
{
    Ref<AudioClip> AudioClip::Create(const std::string& filepath)
	{
		return CreateRef<AudioClip>(filepath);
	}
}
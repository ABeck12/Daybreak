#pragma once

#include "dbpch.h"

namespace Daybreak
{
    class AudioClip
    {
    public:
        virtual ~AudioClip() = default;

        static Ref<AudioClip> Create(const std::string& filepath);

        //Temporary
        AudioClip(const std::string& filepath) : m_Filepath(filepath) {}
        std::string m_Filepath;

    private:
    };
}
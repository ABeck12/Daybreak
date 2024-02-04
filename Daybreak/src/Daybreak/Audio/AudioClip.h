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
        std::string GetFilepath() const { return m_Filepath; }
        AudioClip(const std::string& filepath) : m_Filepath(filepath) {}
    private:
        std::string m_Filepath;
    };
}
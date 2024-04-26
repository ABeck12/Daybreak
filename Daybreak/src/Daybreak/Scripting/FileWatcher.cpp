#include "Daybreak/Core/DeltaTime.h"
#include "Daybreak/Core/Log.h"
#include "dbpch.h"
#include <chrono>
#include <filesystem>
#include <format>
#include "Daybreak/Scripting/FileWatcher.h"

namespace Daybreak
{
	void FileWatcher::AddFile(const std::filesystem::path& path)
	{
		WatchedFile file = { path, std::filesystem::last_write_time(path).time_since_epoch() };
		m_WatchedFiles.emplace_back(file);
	}

	void FileWatcher::RemoveFile(const std::filesystem::path& path)
	{
	}

	void FileWatcher::OnUpdate(DeltaTime dt)
	{
		bool foundNewerFile = false;
		for (WatchedFile& file : m_WatchedFiles)
		{
			if (std::filesystem::last_write_time(file.Path).time_since_epoch() > file.LastUpdatedTime)
			{
				file.LastUpdatedTime = std::filesystem::last_write_time(file.Path).time_since_epoch();
				foundNewerFile = true;
			}
		}

		// if (foundNewerFile && m_CallbackFn)
		// {
		// 	// m_CallbackFn();
		// }
	}
}

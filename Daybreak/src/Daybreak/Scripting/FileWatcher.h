#pragma once

#include "dbpch.h"

#include "Daybreak/Core/DeltaTime.h"

namespace Daybreak
{
	class FileWatcher
	{
	public:
		void OnUpdate(DeltaTime dt);
		void AddFile(const std::filesystem::path& path);
		void RemoveFile(const std::filesystem::path& path);

		// void SetCallbackFunction((*fn)()) { m_CallbackFn = fn; }

	private:
		struct WatchedFile
		{
			std::filesystem::path Path;
			std::chrono::duration<long long, std::ratio<1, 10000000>> LastUpdatedTime;
		};

	private:
		std::vector<WatchedFile> m_WatchedFiles;
		// (*m_CallbackFn)();
	};
}

#pragma once

#include "dbpch.h"

namespace Daybreak
{
	class Font
	{
	public:
		Font(const std::filesystem::path& path);
		~Font() = default;
	};
}

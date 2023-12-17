#pragma once

namespace Daybreak
{
	class RenderAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual ~RenderAPI() = default;

		virtual void Init() = 0;
		virtual void Clear() = 0;

		static API GetAPI() { return s_API; }
		static Scope<RenderAPI> Create();
	private:
		static API s_API;
	};
}

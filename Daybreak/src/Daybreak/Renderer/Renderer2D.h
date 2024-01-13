#pragma once

#include "Daybreak/Renderer/Camera.h"
#include "Daybreak/Renderer/Texture.h"

namespace Daybreak
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();

		static void Flush();
		static void StartBatch();
		static void NextBatch();

		//static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		//static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& tintColor, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& tintColor);
	private:

		

		//static Scope<Renderer2DData> s_Renderer2DData;
	};
}

#pragma once

#include "Daybreak/Renderer/Camera.h"
#include "Daybreak/Renderer/Texture.h"
#include "Daybreak/Scene/Components.h"
#include "Daybreak/Scene/Entity.h"

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

		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tintColor = glm::vec4(1.0f), const float& tilingFactor = 1.0f, int entityID = -1);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, 
			const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, 
			const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawSprite(const glm::mat4 transform, const SpriteRendererComponent& spriteRenderer, int entityID);
		
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotDeg, 
			const Ref<Texture2D>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float& rotDeg,
			const Ref<Texture2D>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotDeg, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float& rotDeg, const glm::vec4& color);
	

		static void DrawLine(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec4& color);
		static void DrawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color);
	};

}
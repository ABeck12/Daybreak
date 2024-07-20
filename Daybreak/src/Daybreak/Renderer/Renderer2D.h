#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/Camera.h"
#include "Daybreak/Renderer/Texture.h"
#include "Daybreak/Renderer/Font.h"
#include "Daybreak/Renderer/SubTexture.h"

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

		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,
							 const glm::vec4& tintColor = glm::vec4(1.0f), const float tilingFactor = 1.0f,
							 int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture,
							 const glm::vec4& tintColor = glm::vec4(1.0f), const float tilingFactor = 1.0f,
							 int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& tintColor = glm::vec4(1.0f),
							 const float tilingFactor = 1.0f, int entityID = -1);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
							 const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
							 const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture,
							 const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture,
							 const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4,
							 const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4,
							 const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotDeg,
									const Ref<Texture2D>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotDeg,
									const Ref<Texture2D>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotDeg,
									const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotDeg,
									const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotDeg,
									const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotDeg,
									const glm::vec4& color);

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f),
							   const float fade = 0.0f, const float thickness = 1.0f);
		static void DrawCircle(const glm::vec3& position, const float radius, const glm::vec4& color = glm::vec4(1.0f),
							   const float fade = 0.0f, const float thickenss = 1.0f);
		static void DrawCircle(const glm::vec2& position, const float radius, const glm::vec4& color = glm::vec4(1.0f),
							   const float fade = 0.0f, const float thickenss = 1.0f);

		static void DrawLine(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec4& color);
		static void DrawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color);
		static void DrawLine(const glm::vec2& pos, float deg, float length, const glm::vec4& color);

		static void DrawString(const std::string& string, const Ref<Font>& font, const glm::mat4& transform,
							   const glm::vec4& color = glm::vec4(1.0f), float kerning = 0.0f, float lineSpacing = 0.0f,
							   int entityID = -1);
		static void DrawString(const std::string& string, const Ref<Font>& font, const glm::vec3& position,
							   const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), float kerning = 0.0f,
							   float lineSpacing = 0.0f, int entityID = -1);
	};

}

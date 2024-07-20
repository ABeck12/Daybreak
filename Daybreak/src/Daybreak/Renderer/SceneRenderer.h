#pragma once

#include "Daybreak/Renderer/FrameBuffer.h"
#include "Daybreak/Renderer/Shader.h"
#include "Daybreak/Scene/Scene.h"
#include "Daybreak/Scene/Components.h"
#include "Daybreak/Scene/Entity.h"

namespace Daybreak
{
	class SceneRenderer
	{
	public:
		SceneRenderer();

		void DrawSpriteRenderer(const glm::mat4& transform, const SpriteRendererComponent& spriteRenderer, int entityID);
		void DrawAnimator(const glm::mat4& transform, const AnimatorComponent& anim, int entityID);
		void DrawTextRenderer(const glm::mat4& transform, const TextRendererComponent& textRenderer, int entityID);

		void DrawPointLight2D(const glm::vec3& position, const PointLight2DComponent& pointLight);
		void DrawGlobalLight2D(const GlobalLight2DComponent& globalLight);

		const Ref<FrameBuffer>& GetFinalBuffer() const { return m_FinalBuffer; }
		const Ref<FrameBuffer>& GetDrawBuffer2D() const { return m_DrawBuffer2D; }

		void DrawToScreen();

		void StartDrawBuffer2D();
		void StartLightBuffer2D();

		void DoBloom();

		void CheckResize();

	private:
		uint32_t m_BufferWidth;
		uint32_t m_BufferHeight;

		Ref<FrameBuffer> m_DrawBuffer2D;
		Ref<FrameBuffer> m_LightBuffer2D;
		Ref<FrameBuffer> m_FinalBuffer;

		// Regular bloom stuff
		Ref<FrameBuffer> m_BloomBuffer;
		Ref<FrameBuffer> m_BloomBufferFinal;
		float m_BloomBlurDirections = 32.0f;
		float m_BloomSize = 32.0f;
		float m_BloomQuality = 8.0f;
		std::array<Ref<FrameBuffer>, 7> m_BloomDownscaleBuffers;
		std::array<Ref<FrameBuffer>, 7> m_BloomUpscaleBuffers;

		float m_Strength = 1;
		Ref<Shader> m_BloomShader;
		Ref<Shader> m_BloomBlurDownscaleShader;
		Ref<Shader> m_BloomBlurUpscaleShader;

		Ref<Shader> m_DefaultDrawShader;
		Ref<Shader> m_FinalShader;

		friend class RendererTesting; // TODO Temporary
	};
}

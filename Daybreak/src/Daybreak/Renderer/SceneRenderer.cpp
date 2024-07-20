#include "dbpch.h"

#include "Daybreak/Renderer/SceneRenderer.h"

#include "Daybreak/Renderer/Renderer2D.h"
#include "Daybreak/Renderer/Renderer.h"
#include "Daybreak/Core/Application.h"
#include "Daybreak/Assets/AssetManager/AssetManager.h"
#include "Daybreak/Renderer/RenderCommand.h"

namespace Daybreak
{
	SceneRenderer::SceneRenderer()
	{
		m_BufferWidth = Application::Get().GetWindow().GetWidth();
		m_BufferHeight = Application::Get().GetWindow().GetHeight();

		FrameBufferSpecifications lightBuffer2DSpec;
		lightBuffer2DSpec.Height = m_BufferHeight;
		lightBuffer2DSpec.Width = m_BufferWidth;
		lightBuffer2DSpec.ClearColor = { 1, 1, 1, 1 };
		lightBuffer2DSpec.AttachmentTypes = {
			FrameBufferAttachmentTypes::RGBA32F,
			FrameBufferAttachmentTypes::Depth
		};
		m_LightBuffer2D = FrameBuffer::Create(lightBuffer2DSpec);

		FrameBufferSpecifications drawBuffer2DSpec;
		drawBuffer2DSpec.Height = m_BufferHeight;
		drawBuffer2DSpec.Width = m_BufferWidth;
		drawBuffer2DSpec.ClearColor = { -1, 0, 0, 1 };
		drawBuffer2DSpec.AttachmentTypes = {
			FrameBufferAttachmentTypes::RGBA,
			FrameBufferAttachmentTypes::RedInteger,
			FrameBufferAttachmentTypes::Depth
		};
		m_DrawBuffer2D = FrameBuffer::Create(drawBuffer2DSpec);

		FrameBufferSpecifications bloomBufferSpec;
		bloomBufferSpec.Height = m_BufferHeight;
		bloomBufferSpec.Width = m_BufferWidth;
		bloomBufferSpec.ClearColor = { 0, 0, 0, 1 };
		bloomBufferSpec.AttachmentTypes = {
			FrameBufferAttachmentTypes::RGBA32F,
		};
		m_BloomBuffer = FrameBuffer::Create(bloomBufferSpec);
		// bloomBufferSpec.AttachmentTypes = { FrameBufferAttachmentTypes::RGBA };
		m_BloomBufferFinal = FrameBuffer::Create(bloomBufferSpec);

		for (size_t i = 0; i < m_BloomDownscaleBuffers.size(); i++)
		{
			bloomBufferSpec.Width = m_BufferWidth / (1 << (i + 1));
			bloomBufferSpec.Height = m_BufferHeight / (1 << (i + 1));
			// bloomBufferSpec.Width = uint32_t(m_BufferWidth / (1.8 * (i + 1)));
			// bloomBufferSpec.Height = uint32_t(m_BufferHeight / (1.8 * (i + 1)));
			m_BloomDownscaleBuffers[i] = FrameBuffer::Create(bloomBufferSpec);
			m_BloomUpscaleBuffers[i] = FrameBuffer::Create(bloomBufferSpec);
		}

		// for (size_t i = 0; i < m_BloomUpscaleBuffers.size(); i++)
		// {
		// 	bloomBufferSpec.Width = m_BufferWidth / (1 << (i + 1));
		// 	bloomBufferSpec.Height = m_BufferHeight / (1 << (i + 1));
		// 	m_BloomUpscaleBuffers[i] = FrameBuffer::Create(bloomBufferSpec);
		// }

		FrameBufferSpecifications finalBufferSpec;
		finalBufferSpec.Height = m_BufferHeight;
		finalBufferSpec.Width = m_BufferWidth;
		finalBufferSpec.ClearColor = { 0, 0, 0, 1 };
		finalBufferSpec.AttachmentTypes = {
			FrameBufferAttachmentTypes::RGBA,
			FrameBufferAttachmentTypes::Depth
		};
		m_FinalBuffer = FrameBuffer::Create(finalBufferSpec);

		m_DefaultDrawShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_DefaultFrameBufferDraw.glsl");
		m_BloomShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_BloomShader.glsl");
		m_BloomBlurDownscaleShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_BloomShader_BlurDownscale.glsl");
		m_BloomBlurUpscaleShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_BloomShader_BlurUpscale.glsl");
		m_FinalShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_FinalShader.glsl");
	}

	void SceneRenderer::DrawSpriteRenderer(const glm::mat4& transform, const SpriteRendererComponent& spriteRenderer, int entityID)
	{
		if (spriteRenderer.Sprite)
		{
			const glm::vec3 scaleVector = { (float)spriteRenderer.Sprite->GetWidth() / (float)spriteRenderer.Sprite->GetTexutreSpecifications().PixelsPerUnit,
											(float)spriteRenderer.Sprite->GetHeight() / (float)spriteRenderer.Sprite->GetTexutreSpecifications().PixelsPerUnit,
											1.0f };
			const glm::mat4 scaledTransform = glm::scale(transform, scaleVector);
			Renderer2D::DrawQuad(scaledTransform,
								 spriteRenderer.Sprite,
								 spriteRenderer.TintColor,
								 spriteRenderer.TilingFactor, entityID);
		}
		else
		{
			Renderer2D::DrawQuad(transform, spriteRenderer.TintColor, spriteRenderer.TilingFactor, entityID);
		}
	}

	void SceneRenderer::DrawAnimator(const glm::mat4& transform, const AnimatorComponent& anim, int entityID)
	{
		if (!anim.Controller)
		{
			return;
		}
		const KeyFrame& currentFrame = anim.Controller->GetActiveAnimation()->GetCurrentKeyFrame();
		const TextureSpecifications& spec = currentFrame.Sprite->GetTexture()->GetTexutreSpecifications();
		const glm::mat4 scaledTransform = glm::scale(transform,
													 { (float)currentFrame.Sprite->GetWidth() / (float)spec.PixelsPerUnit,
													   (float)currentFrame.Sprite->GetHeight() / (float)spec.PixelsPerUnit,
													   1.0f });
		Renderer2D::DrawQuad(scaledTransform, currentFrame.Sprite, anim.TintColor, 1.0f, entityID);
	}

	void SceneRenderer::DrawTextRenderer(const glm::mat4& transform, const TextRendererComponent& textRenderer, int entityID)
	{
		Renderer2D::DrawString(textRenderer.Text,
							   textRenderer.Font,
							   transform,
							   textRenderer.Color,
							   textRenderer.Kerning,
							   textRenderer.LineSpacing,
							   entityID);
	}

	void SceneRenderer::DrawPointLight2D(const glm::vec3& position, const PointLight2DComponent& pointLight)
	{
		Renderer2D::DrawCircle(position,
							   pointLight.OuterRadius,
							   glm::vec4(pointLight.Color.r, pointLight.Color.g, pointLight.Color.b, 1.0f) * pointLight.Intensity,
							   ((pointLight.OuterRadius - pointLight.InnerRadius) / pointLight.OuterRadius) / pointLight.Intensity,
							   1.0f);
	}

	void SceneRenderer::DrawGlobalLight2D(const GlobalLight2DComponent& globalLight)
	{
		// Drawing a globa light requires only setting the clear color of the lighting framebuffer
		RenderCommand::SetClearColor({
			globalLight.Color.r * globalLight.Intensity,
			globalLight.Color.g * globalLight.Intensity,
			globalLight.Color.b * globalLight.Intensity,
			1,
		});
		RenderCommand::Clear();
	}

	void SceneRenderer::CheckResize()
	{
		uint32_t width = Application::Get().GetWindow().GetWidth();
		uint32_t height = Application::Get().GetWindow().GetHeight();

		if (m_BufferHeight != height || m_BufferWidth != width)
		{
			m_DrawBuffer2D->Resize(width, height);
			m_LightBuffer2D->Resize(width, height);
			m_FinalBuffer->Resize(width, height);

			m_BufferWidth = width;
			m_BufferHeight = height;
		}
	}

	void SceneRenderer::StartDrawBuffer2D()
	{
		m_DrawBuffer2D->Bind();
		RenderCommand::SetClearColor(m_DrawBuffer2D->GetClearColor());
		RenderCommand::Clear();
	}

	void SceneRenderer::StartLightBuffer2D()
	{
		m_LightBuffer2D->Bind();
		RenderCommand::SetClearColor(m_LightBuffer2D->GetClearColor());
		RenderCommand::Clear();
	}

	void SceneRenderer::DoBloom()
	{
		m_BloomBufferFinal->Bind();
		RenderCommand::SetClearColor({ 0, 0, 0, 1 });
		RenderCommand::Clear();

		Renderer2D::NextBatch();
		m_BloomBuffer->Bind();
		RenderCommand::SetClearColor({ 0, 0, 0, 1 });
		RenderCommand::Clear();

		// Renderer2D::DrawQuad({ 5, 3 }, { 1, 1 }, { 0.005, 0.05, 0.05, 1 });
		// Renderer2D::DrawQuad({ 3, 3 }, { 1, 1 }, { 0.1, 1, 1, 1 });
		// Renderer2D::DrawQuad({ 0, 3 }, { 1, 1 }, { 1, 1, 1, 1 });
		// Renderer2D::DrawCircle({ -4, 4 }, 2, { 0.1, 0.91, 0.05, 1 });
		Renderer2D::DrawString("Bloom", Font::GetDefault(), { -6, -4, 0 }, { 7, 7 }, { m_Strength * 1, m_Strength * 0.1, m_Strength * 0.1, 1 });
		Renderer2D::EndScene();

		// Downsample
		m_BloomBlurDownscaleShader->Bind();
		m_BloomBlurDownscaleShader->SetFloat1("u_Directions", m_BloomBlurDirections);
		m_BloomBlurDownscaleShader->SetFloat1("u_Size", m_BloomSize);
		m_BloomBlurDownscaleShader->SetFloat1("u_Quality", m_BloomQuality);
		m_BloomBuffer->BindAttachmentAsTexture(0, 0);
		for (size_t i = 0; i < m_BloomDownscaleBuffers.size(); i++)
		{
			m_BloomBlurDownscaleShader->Bind();
			m_BloomBlurDownscaleShader->SetFloat2("u_Resolution", { m_BloomDownscaleBuffers[i]->GetSpecification().Width, m_BloomDownscaleBuffers[i]->GetSpecification().Height });
			m_BloomDownscaleBuffers[i]->Bind();
			RenderCommand::Clear();
			Renderer::DrawFrameBuffer(m_BloomDownscaleBuffers[i], m_BloomBlurDownscaleShader);
			m_BloomDownscaleBuffers[i]->BindAttachmentAsTexture(0, 0);
		}

		// Upsample
		std::vector<int> indices;
		indices.emplace_back(0);
		m_BloomBuffer->BindAttachmentAsTexture(0, 0);
		for (size_t i = 0; i < m_BloomDownscaleBuffers.size(); i++)
		{
			indices.emplace_back((int)i);
			m_BloomDownscaleBuffers[i]->BindAttachmentAsTexture(0, uint32_t(i) + 1);
		}

		int lastBufferIndex = int(m_BloomDownscaleBuffers.size() - 1);
		m_BloomDownscaleBuffers[lastBufferIndex]->Blit(m_BloomUpscaleBuffers[lastBufferIndex]);
		for (int i = int(m_BloomUpscaleBuffers.size() - 2); i >= 0; i--)
		{
			m_BloomDownscaleBuffers[i]->BindAttachmentAsTexture(0, 0);
			m_BloomUpscaleBuffers[i + i]->BindAttachmentAsTexture(0, 1);

			m_BloomUpscaleBuffers[i]->Bind();
			RenderCommand::Clear();
			Renderer::DrawFrameBuffer(m_BloomUpscaleBuffers[i], m_BloomBlurUpscaleShader);
		}

		// Final image
		m_BloomBufferFinal->Bind();
		RenderCommand::Clear();
		m_BloomBuffer->BindAttachmentAsTexture(0, 0);
		m_BloomUpscaleBuffers[0]->BindAttachmentAsTexture(0, 1);
		Renderer::DrawFrameBuffer(m_BloomBufferFinal, m_BloomShader, { 0, 1 });
	}

	void SceneRenderer::DrawToScreen()
	{
		m_FinalBuffer->Bind();
		RenderCommand::SetClearColor(m_FinalBuffer->GetClearColor());
		RenderCommand::Clear();

		m_DrawBuffer2D->BindAttachmentAsTexture(0, 0);	// RBGA32F
		m_DrawBuffer2D->BindAttachmentAsTexture(2, 1);	// Depth
		m_LightBuffer2D->BindAttachmentAsTexture(0, 2); // RGBA

		Renderer::DrawFrameBuffer(m_FinalBuffer, m_FinalShader, { 0, 1, 2 });

		// Final copy to screen
		m_FinalBuffer->Unbind();
		m_FinalBuffer->BindAttachmentAsTexture(0, 0); // RGBA
		m_FinalBuffer->BindAttachmentAsTexture(1, 1); // Depth
		Renderer::DrawFrameBuffer(nullptr, m_DefaultDrawShader, { 0, 1, 2 });
	}
}
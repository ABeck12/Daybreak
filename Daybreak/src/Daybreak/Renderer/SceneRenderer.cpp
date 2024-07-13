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

		FrameBufferSpecifications finalBufferSpec;
		finalBufferSpec.Height = m_BufferHeight;
		finalBufferSpec.Width = m_BufferWidth;
		finalBufferSpec.ClearColor = { 0, 0, 0, 1 };
		finalBufferSpec.AttachmentTypes = {
			FrameBufferAttachmentTypes::RGBA,
			FrameBufferAttachmentTypes::Depth
		};
		m_FinalBuffer = FrameBuffer::Create(finalBufferSpec);

		// m_LightingShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_LightingFrameBuffer.glsl");
		m_DefaultDrawShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_DefaultFrameBufferDraw.glsl");
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

	// void SceneRenderer::StopLightBuffer2D()
	// {
	// 	RenderCommand::SetBlendMode(RenderAPI::BlendModes::OneMinusSrcAlpha);
	// }

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

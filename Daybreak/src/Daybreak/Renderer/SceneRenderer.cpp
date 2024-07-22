#include "dbpch.h"

#include "Daybreak/Renderer/SceneRenderer.h"

#include "Daybreak/Renderer/Renderer2D.h"
#include "Daybreak/Renderer/Renderer.h"
#include "Daybreak/Core/Application.h"
#include "Daybreak/Assets/AssetManager/AssetManager.h"
#include "Daybreak/Renderer/RenderCommand.h"
#include "glm/geometric.hpp"

#include "glad/glad.h"

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

#if 0
		m_BloomMask = FrameBuffer::Create(bloomBufferSpec);
		// bloomBufferSpec.AttachmentTypes = { FrameBufferAttachmentTypes::RGBA };
		m_BloomBufferFinal = FrameBuffer::Create(bloomBufferSpec);

		for (size_t i = 0; i < m_BloomDownscaleBuffers.size(); i++)
		{
			bloomBufferSpec.Width = m_BufferWidth / (1 << (i + 1));
			bloomBufferSpec.Height = m_BufferHeight / (1 << (i + 1));
			m_BloomDownscaleBuffers[i] = FrameBuffer::Create(bloomBufferSpec);
			m_BloomUpscaleBuffers[i] = FrameBuffer::Create(bloomBufferSpec);
		}
		m_BloomShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_BloomShader.glsl");
		m_BloomBlurDownscaleShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_BloomShader_BlurDownscale.glsl");
		m_BloomBlurUpscaleShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_BloomShader_BlurUpscale.glsl");
#endif

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

#if 0
	void SceneRenderer::DoBloom()
	{
		Renderer2D::NextBatch();
		m_BloomMask->Bind();
		RenderCommand::SetClearColor({ 0, 0, 0, 1 });
		RenderCommand::Clear();

		Renderer2D::DrawQuad({ 8, 4 }, { 1, 1 }, { m_Strength * 0.005, m_Strength * 0.05, m_Strength * 0.05, 1 });
		Renderer2D::DrawQuad({ 4, 3 }, { 1, 1 }, { m_Strength * 0.1, m_Strength * 1, m_Strength * 1, 1 });
		Renderer2D::DrawQuad({ 0, 3 }, { 1, 1 }, { m_Strength * 1, m_Strength * 1, m_Strength * 1, 1 });
		Renderer2D::DrawCircle({ -6, 4 }, 1, { m_Strength * 0.1, m_Strength * 0.91, m_Strength * 0.05, 1 });
		Renderer2D::DrawString("Bloom", Font::GetDefault(), { -6, -4, 0 }, { 7, 7 }, { m_Strength * 1, m_Strength * 0.1, m_Strength * 0.1, 1 });
		Renderer2D::EndScene();

		// Downsample
		m_BloomBlurDownscaleShader->Bind();
		m_BloomMask->BindAttachmentAsTexture(0, 0);
		for (size_t i = 0; i < m_BloomDownscaleBuffers.size(); i++)
		{
			m_BloomBlurDownscaleShader->Bind();
			m_BloomBlurDownscaleShader->SetFloat2("u_Resolution", { m_BloomDownscaleBuffers[i]->GetSpecification().Width, m_BloomDownscaleBuffers[i]->GetSpecification().Height });
			m_BloomDownscaleBuffers[i]->Bind();
			RenderCommand::Clear();
			Renderer::DrawFrameBuffer(m_BloomDownscaleBuffers[i], m_BloomBlurDownscaleShader, { 0 });
			m_BloomDownscaleBuffers[i]->BindAttachmentAsTexture(0, 0);
		}

		// // Upsample
		int lastBufferIndex = int(m_BloomDownscaleBuffers.size() - 1);
		m_BloomDownscaleBuffers[lastBufferIndex]->Blit(m_BloomUpscaleBuffers[lastBufferIndex]);
		for (int i = int(m_BloomUpscaleBuffers.size() - 2); i >= 0; i--)
		{
			m_BloomDownscaleBuffers[i]->BindAttachmentAsTexture(0, 0);	 // Current downscale
			m_BloomUpscaleBuffers[i - 1]->BindAttachmentAsTexture(0, 1); // Previous upscale

			m_BloomUpscaleBuffers[i]->Bind();
			RenderCommand::Clear();
			Renderer::DrawFrameBuffer(m_BloomUpscaleBuffers[i], m_BloomBlurUpscaleShader, { 0, 1 });
		}

		// Final image
		m_BloomShader->Bind();
		m_BloomShader->SetFloat1("u_Gamma", m_Gamma);
		m_BloomShader->SetFloat1("u_Exposure", m_Exposure);

		m_BloomBufferFinal->Bind();
		RenderCommand::Clear();
		m_BloomMask->BindAttachmentAsTexture(0, 0);
		m_BloomUpscaleBuffers[0]->BindAttachmentAsTexture(0, 1);
		Renderer::DrawFrameBuffer(m_BloomBufferFinal, m_BloomShader, { 0, 1 });
	}
#endif

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
		Renderer::DrawFrameBuffer(nullptr, m_DefaultDrawShader, { 0, 1 });
	}

	glm::vec2 calcPointOnRadius(const glm::vec2 light, const float radius, const glm::vec2 point)
	{
		glm::vec2 direction = glm::normalize(point - light);
		return (radius - glm::length(light - point)) * 2 * direction + point;
		// return (radius - glm::length(light - point)) * direction + point;
	}

	std::pair<glm::vec2, glm::vec2> calcClosestPoints(const glm::vec2 light, const glm::vec2* castorVertices)
	{
		float distances[4] = {};
		for (int i = 0; i < 4; i++)
		{
			distances[i] = glm::length(light - castorVertices[i]);
		}

		int smallestIndex = 0;
		float smallestDistance = FLT_MAX;
		for (int i = 0; i < 4; i++)
		{
			if (distances[i] < smallestDistance)
			{
				smallestDistance = distances[i];
				smallestIndex = i;
			}
		}
		int secondSmallestIndex = 0;
		for (int i = 0; i < 4; i++)
		{
			if (distances[i] <= smallestDistance && i != smallestIndex)
			{
				secondSmallestIndex = i;
			}
		}
		return std::make_pair(castorVertices[smallestIndex], castorVertices[secondSmallestIndex]);
	}

	void SceneRenderer::TestShadowDrawing()
	{
		float dotSize = 0.1f;

		glm::vec2 castorVertices[4] = {
			{ m_CastorPos.x + m_CastorWidth / 2, m_CastorPos.y + m_CastorHeight / 2 },
			{ m_CastorPos.x - m_CastorWidth / 2, m_CastorPos.y + m_CastorHeight / 2 },
			{ m_CastorPos.x + m_CastorWidth / 2, m_CastorPos.y - m_CastorHeight / 2 },
			{ m_CastorPos.x - m_CastorWidth / 2, m_CastorPos.y - m_CastorHeight / 2 },
		};

		// Draw light and shadow caster points
		{
			Renderer2D::NextBatch();
			RenderCommand::SetClearColor({ 0.3, 0.3, 0.3, 1 });
			RenderCommand::Clear();
			Renderer2D::DrawCircle(m_LightPos, m_LightRadius, { 1, 1, 0, 0.3 });
			Renderer2D::DrawCircle(m_LightPos, dotSize, { 1, 1, 0, 1 }); // Light

			Renderer2D::DrawCircle(castorVertices[0], dotSize, { 1, 0, 0, 1 });
			Renderer2D::DrawCircle(castorVertices[1], dotSize, { 1, 0, 0, 1 });
			Renderer2D::DrawCircle(castorVertices[2], dotSize, { 1, 0, 0, 1 });
			Renderer2D::DrawCircle(castorVertices[3], dotSize, { 1, 0, 0, 1 });
			Renderer2D::EndScene();
			Renderer2D::NextBatch();
		}

		{
#if 0
		// TODO Get the 2 closest vertices automatically
		glm::vec2 closetVertices[2] = { castorVertices[1], castorVertices[3] };


		// TODO Add a check to see if the castor vertices are inside the lights radius. If not then we dont need to draw anything
		float distance0 = glm::length(closetVertices[0] - m_LightPos);
		float distance1 = glm::length(closetVertices[1] - m_LightPos);

		if (distance0 > m_LightRadius || distance1 > m_LightRadius)
			return;

		// Calculate and draw the endpoints on the circle
		glm::vec2 endpoints[2];
		// TODO Add chord offset such that both points lie outside the circle and the the line between them is tangent
		endpoints[0] = calcPointOnRadius(m_LightPos, m_LightRadius, closetVertices[0]);
		endpoints[1] = calcPointOnRadius(m_LightPos, m_LightRadius, closetVertices[1]);

		Renderer2D::DrawQuad(closetVertices[1], closetVertices[0], endpoints[0], endpoints[1], { 0, 0, 0, 0.3 });

		Renderer2D::DrawCircle(endpoints[0], dotSize, { 0, 1, 0, 1 });
		Renderer2D::DrawCircle(endpoints[1], dotSize, { 0, 1, 0, 1 });

		Renderer2D::EndScene();
#endif
		}

		const float lightRad2 = m_LightRadius * m_LightRadius;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++) // This double counts the draws. We only need 6 but were doing 12
			{
				if (i == j)
					continue;

				glm::vec2& p1 = castorVertices[i];
				glm::vec2& p2 = castorVertices[j];

				if (glm::length2(p1 - m_LightPos) > lightRad2 || glm::length2(p2 - m_LightPos) > lightRad2)
					continue;

				glm::vec2 q1 = calcPointOnRadius(m_LightPos, m_LightRadius, p1);
				glm::vec2 q2 = calcPointOnRadius(m_LightPos, m_LightRadius, p2);

				// Need to sort them to be in ccw order

				Renderer2D::DrawCircle(q1, dotSize, { 0, 1, 0, 1 });
				Renderer2D::DrawCircle(q2, dotSize, { 0, 1, 0, 1 });

				Renderer2D::DrawQuad(p2, p1, q1, q2, { 0, 0, 0, 0.3 });
			}
		}
		Renderer2D::EndScene();
	}

	void SceneRenderer::DrawPointLight2DWithShadows(const glm::vec3 position, const PointLight2DComponent& pointLight)
	{
		{
			Renderer2D::DrawCircle(position,
								   pointLight.OuterRadius,
								   glm::vec4(pointLight.Color.r, pointLight.Color.g, pointLight.Color.b, 1.0f) * pointLight.Intensity,
								   ((pointLight.OuterRadius - pointLight.InnerRadius) / pointLight.OuterRadius) / pointLight.Intensity,
								   1.0f);
			Renderer2D::EndScene();
			Renderer2D::NextBatch();
		}
		const size_t numberCastors = m_Castors.size();
		const glm::vec2 pos = glm::vec2(position);
		for (size_t i = 0; i < numberCastors; i++)
		{
			const ShadowCasterComponent& castor = m_Castors[i];
			const glm::vec2& castorPosition = m_CastorPositions[i];

			const glm::vec2 castorVertices[4] = {
				{ castorPosition.x + castor.Size.x / 2, castorPosition.y + castor.Size.y / 2 },
				{ castorPosition.x - castor.Size.x / 2, castorPosition.y + castor.Size.y / 2 },
				{ castorPosition.x + castor.Size.x / 2, castorPosition.y - castor.Size.y / 2 },
				{ castorPosition.x - castor.Size.x / 2, castorPosition.y - castor.Size.y / 2 },
			};
			glDisable(GL_BLEND);
			const float lightRad2 = pointLight.OuterRadius * pointLight.OuterRadius;
			for (int j = 0; i < 4; i++)
			{
				for (int k = 0; j < 4; j++) // This double counts the draws. We only need 6 but were doing 12
				{
					if (i == j)
						continue;

					const glm::vec2& p1 = castorVertices[j];
					const glm::vec2& p2 = castorVertices[k];

					if (glm::length2(p1 - pos) > lightRad2 || glm::length2(p2 - pos) > lightRad2)
						continue;

					glm::vec2 q1 = calcPointOnRadius(pos, m_LightRadius, p1);
					glm::vec2 q2 = calcPointOnRadius(pos, m_LightRadius, p2);

					Renderer2D::DrawCircle(p1, 0.1, { 1, 0, 0, 1 });
					Renderer2D::DrawCircle(p2, 0.1, { 1, 0, 0, 1 });

					Renderer2D::DrawCircle(q1, 0.1, { 0, 1, 0, 1 });
					Renderer2D::DrawCircle(q2, 0.1, { 0, 1, 0, 1 });
					Renderer2D::DrawQuad(p2, p1, q1, q2, { 0, 0, 0, 1 });
				}
			}
			Renderer2D::EndScene();
			Renderer2D::NextBatch();
			glEnable(GL_BLEND);
		}
	}
}

#include "dbpch.h"

#include "Daybreak/Physics/DebugDraw.h"

#include <box2d/box2d.h>

#include "Daybreak/Renderer/Renderer2D.h"

namespace Daybreak
{
	void DebugRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		// for (int i = 0; i < vertexCount - 1; i++)
		// {
		// 	glm::vec2 p1 = { vertices[i].x, vertices[i].y };
		// 	glm::vec2 p2 = { vertices[i + 1].x, vertices[i + 1].y };
		// 	Renderer2D::DrawLine(p1, p2, m_DrawColor);
		// }
		// Renderer2D::DrawLine({ vertices[0].x, vertices[0].y }, { vertices[vertexCount - 1].x, vertices[vertexCount - 1].y }, m_DrawColor);
	}

	void DebugRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		for (int i = 0; i < vertexCount - 1; i++)
		{
			glm::vec2 p1 = { vertices[i].x, vertices[i].y };
			glm::vec2 p2 = { vertices[i + 1].x, vertices[i + 1].y };
			Renderer2D::DrawLine(p1, p2, m_DrawColor);
		}
		Renderer2D::DrawLine({ vertices[0].x, vertices[0].y }, { vertices[vertexCount - 1].x, vertices[vertexCount - 1].y }, m_DrawColor);
	}

	void DebugRenderer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
	{
		// Renderer2D::DrawCircle({ center.x, center.y }, radius, m_DrawColor, 0.0f, 0.01f);
	}

	void DebugRenderer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
	{
		Renderer2D::DrawCircle({ center.x, center.y }, radius, m_DrawColor, 0.0f, 0.02f);
	}

	void DebugRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		Renderer2D::DrawLine({ p1.x, p1.y }, { p2.x, p2.y }, m_DrawColor);
	}

	void DebugRenderer::DrawTransform(const b2Transform& xf)
	{
	}

	void DebugRenderer::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
	{
	}
}
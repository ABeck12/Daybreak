#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/Buffer.h"

namespace Daybreak
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		static Ref<VertexArray> Create();

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
	};
}
#pragma once

#include "dbpch.h"

namespace Daybreak
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(const void* vertices, uint32_t size);

		//virtual void Bind() const = 0;
		//virtual void Unbind() const = 0;

		//virtual void SetData(const void* data, uint32_t size) const = 0;

	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		static Ref<IndexBuffer> Create(uint32_t size);
		static Ref<IndexBuffer> Create(const void* indices, uint32_t size);

		//virtual void Bind() const = 0;
		//virtual void Unbind() const = 0;

		//virtual void SetData(const void* data, uint32_t size) const = 0;

	};
}
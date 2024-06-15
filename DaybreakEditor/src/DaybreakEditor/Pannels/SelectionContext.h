#pragma once

#include <Daybreak.h>

namespace Daybreak
{
	enum class SelectionContextType
	{
		None,
		Entity,
		File
	};

	class SelectionContext
	{
	public:
		virtual ~SelectionContext() = default;

		virtual const SelectionContextType GetContextType() const { return SelectionContextType::None; }

		static void AddContextCallback(const std::function<void(void)>& fn) { s_Callbacks.emplace_back(fn); }
		static void RemoveContextCallback(const std::function<void(void)>& fn);
		static const Ref<SelectionContext>& GetContext() { return s_Context; }
		static void SetContext(const Ref<SelectionContext>& context);
		static void ResetContext() { s_Context = CreateRef<SelectionContext>(); }

	public:
		static Ref<SelectionContext> s_Context;
		static std::vector<std::function<void(void)>> s_Callbacks;
	};

	class EntityContext final : public SelectionContext
	{
	public:
		EntityContext(Entity entity);

		virtual const SelectionContextType GetContextType() const override { return SelectionContextType::Entity; }
		const Entity& GetEntity() const { return m_Entity; }

	private:
		Entity m_Entity;
	};

	class FileContext final : public SelectionContext
	{
	public:
		FileContext(const std::filesystem::path& path);

		virtual const SelectionContextType GetContextType() const override { return SelectionContextType::File; }
		const std::filesystem::path& GetPath() const { return m_Path; }

	private:
		std::filesystem::path m_Path;
	};
}
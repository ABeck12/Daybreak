#include "DaybreakEditor/Pannels/SelectionContext.h"

#include <Daybreak.h>

namespace Daybreak
{
	Ref<SelectionContext> SelectionContext::s_Context = CreateRef<SelectionContext>();
	std::vector<std::function<void(void)>> SelectionContext::s_Callbacks = std::vector<std::function<void(void)>>();

	void SelectionContext::SetContext(const Ref<SelectionContext>& context)
	{
		s_Context = context;
		for (auto fn : s_Callbacks)
		{
			fn();
		}
	}


	EntityContext::EntityContext(Entity entity)
		: m_Entity(entity)
	{
	}

	FileContext::FileContext(const std::filesystem::path& path)
		: m_Path(path)
	{
	}
}
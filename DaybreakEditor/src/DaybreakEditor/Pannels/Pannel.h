#pragma once

#include <Daybreak.h>

namespace Daybreak
{
	class Pannel
	{
	public:
		virtual ~Pannel() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(DeltaTime dt) = 0;
		virtual void OnEvent(Event& event) = 0;
		virtual void OnImGuiRender() = 0;

		virtual const std::string& GetName() const = 0;
	};

	class PannelOrganizer
	{
	public:
		~PannelOrganizer();

		std::vector<Pannel*>::iterator begin() { return m_Pannels.begin(); }
		std::vector<Pannel*>::iterator end() { return m_Pannels.end(); }
		std::vector<Pannel*>::reverse_iterator rbegin() { return m_Pannels.rbegin(); }
		std::vector<Pannel*>::reverse_iterator rend() { return m_Pannels.rend(); }

		std::vector<Pannel*>::const_iterator begin() const { return m_Pannels.begin(); }
		std::vector<Pannel*>::const_iterator end() const { return m_Pannels.end(); }
		std::vector<Pannel*>::const_reverse_iterator rbegin() const { return m_Pannels.rbegin(); }
		std::vector<Pannel*>::const_reverse_iterator rend() const { return m_Pannels.rend(); }

		template<typename T, typename... Args>
		void AddPannel(Args&&... args)
		{
			m_Pannels.emplace_back(new T(std::forward<Args>(args)...));
		}

		template<typename T>
		T* GetPannelByName(const std::string& name)
		{
			for (Pannel* pannel : m_Pannels)
			{
				if (pannel->GetName() == name)
				{
					return (T*)pannel;
				}
			}
			DB_CORE_ASSERT(false, "Could not find pannel {}!", name);
			return nullptr;
		}

	private:
		std::vector<Pannel*> m_Pannels;
	};
}
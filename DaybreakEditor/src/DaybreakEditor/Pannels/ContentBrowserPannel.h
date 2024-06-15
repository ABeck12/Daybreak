#pragma once

#include "DaybreakEditor/Pannels/Pannel.h"

namespace Daybreak
{
	class ContentBrowserPannel final : public Pannel
	{
	public:
		ContentBrowserPannel();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime dt) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;
		virtual const std::string& GetName() const override { return m_PannelName; }

		void SetResourceDirectory(const std::filesystem::path& path) { m_ResourceDirectory = path; }

	private:
		void RenderFileButton(const std::filesystem::directory_entry& entry);

	private:
		std::string m_PannelName = "ContentBrowserPannel";

		Ref<Texture2D> m_FileTexture;
		Ref<Texture2D> m_DirectoryTexture;

		std::filesystem::path m_ResourceDirectory;

		std::filesystem::path m_CurrentPath;
		float m_ItemSize = 90;
	};
}
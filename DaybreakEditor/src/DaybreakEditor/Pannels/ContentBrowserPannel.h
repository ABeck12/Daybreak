#pragma once

#include "DaybreakEditor/Pannels/Pannel.h"

namespace Daybreak
{
	class ContentBrowserPannel final : public Pannel
	{
	public:
		enum class FileType
		{
			Default,
			Directory,
			Cpp,
			Header,
			Sprite,
			Shader,
			Image,
			Scene,
			Animation,
			AnimationController,
			Font,
		};

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

		std::unordered_map<FileType, Ref<Texture2D>> m_TextureMap;
		Ref<Texture2D> m_BackButton;
		// Ref<Texture2D> m_DirectoryTexture;

		std::filesystem::path m_ResourceDirectory;

		std::filesystem::path m_CurrentPath;
		float m_ItemSize = 90;
	};
}
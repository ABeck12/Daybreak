#include "DaybreakEditor/Pannels/ContentBrowserPannel.h"
#include "DaybreakEditor/EditorLayer.h"

#include <imgui.h>

#include "DaybreakEditor/Pannels/SelectionContext.h"

namespace Daybreak
{
	ContentBrowserPannel::ContentBrowserPannel()
	{
	}

	void ContentBrowserPannel::OnUpdate(DeltaTime dt)
	{
	}

	void ContentBrowserPannel::OnAttach()
	{
		m_CurrentPath = AssetManager::Get()->GetAssetDir();
		TextureSpecifications spec = { 512, 512, ImageFormat::RGBA, TextureFilterType::Bilinear };
		m_FileTexture = Texture2D::Create(spec, m_ResourceDirectory / "file.png");
		m_DirectoryTexture = Texture2D::Create(spec, m_ResourceDirectory / "folder.png");
	}

	void ContentBrowserPannel::OnDetach()
	{
	}

	void ContentBrowserPannel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");
		std::filesystem::path assetDir = AssetManager::Get()->GetAssetDir();
		if (ImGui::Button("Back") && m_CurrentPath != assetDir)
		{
			m_CurrentPath = m_CurrentPath.parent_path();
		}
		ImGui::SameLine();
		ImGui::Text("%ls", std::filesystem::relative(m_CurrentPath, assetDir.parent_path()).c_str());
		ImGui::Separator();

		// ImGui::BeginTable("##ContentBrowserTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV);
		// ImGui::TableNextColumn();
		// ImGui::Button("Assets");
		// ImGui::Button("Scripts");

		// ImGui::TableNextColumn();

		int numColumns = ImGui::GetContentRegionAvail().x / (int)m_ItemSize;
		if (numColumns <= 0)
		{
			numColumns = 1;
		}
		ImGui::Columns(numColumns, NULL, false);
		for (auto& item : std::filesystem::directory_iterator(m_CurrentPath))
		{
			RenderFileButton(item);
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		// ImGui::EndTable();
		ImGui::End();
	}

	void ContentBrowserPannel::OnEvent(Event& event)
	{
	}

	void ContentBrowserPannel::RenderFileButton(const std::filesystem::directory_entry& entry)
	{
		std::filesystem::path path = entry.path();
		bool isDir = entry.is_directory();

		Ref<Texture2D> icon = isDir ? m_DirectoryTexture : m_FileTexture;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton(path.string().c_str(), (ImTextureID)icon->GetRendererID(), { m_ItemSize, m_ItemSize }, { 0, 1 }, { 1, 0 }))
		{
			if (!isDir)
			{
				SelectionContext::SetContext(CreateRef<FileContext>(path));
			}
		}

		if (ImGui::BeginDragDropSource())
		{
			std::filesystem::path relativePath = std::filesystem::relative(path, AssetManager::Get()->GetAssetDir());
			if (path.extension() == ".scene")
			{
				ImGui::SetDragDropPayload("FILE_SCENE", (void*)relativePath.string().c_str(), relativePath.string().length() * sizeof(char*));
			}

			ImGui::Text("%ls", relativePath.parent_path().c_str());
			ImGui::EndDragDropSource();
		}
		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (isDir)
			{
				m_CurrentPath = path;
			}
		}
		ImGui::Text("%ls", std::filesystem::relative(path, m_CurrentPath).c_str());
	}
}
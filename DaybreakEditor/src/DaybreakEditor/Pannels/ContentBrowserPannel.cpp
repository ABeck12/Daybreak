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
		TextureSpecifications spec = { 96, 96, ImageFormat::RGBA, TextureFilterType::Bilinear };

		m_TextureMap[FileType::Default] = Texture2D::Create(spec, m_ResourceDirectory / "Icons/file.png");
		m_TextureMap[FileType::Directory] = Texture2D::Create(spec, m_ResourceDirectory / "Icons/folder.png");
		m_TextureMap[FileType::Cpp] = Texture2D::Create(spec, m_ResourceDirectory / "Icons/cpp.png");
		m_TextureMap[FileType::Header] = Texture2D::Create(spec, m_ResourceDirectory / "Icons/header.png");
		m_TextureMap[FileType::Sprite] = Texture2D::Create(spec, m_ResourceDirectory / "Icons/image.png");
		m_TextureMap[FileType::Shader] = Texture2D::Create(spec, m_ResourceDirectory / "Icons/code_block.png");
		m_TextureMap[FileType::Image] = Texture2D::Create(spec, m_ResourceDirectory / "Icons/image.png");
		m_TextureMap[FileType::Scene] = Texture2D::Create(spec, m_ResourceDirectory / "Icons/code_block.png");
		m_TextureMap[FileType::Animation] = Texture2D::Create(spec, m_ResourceDirectory / "Icons/animation.png");
		m_TextureMap[FileType::AnimationController] = Texture2D::Create(spec, m_ResourceDirectory / "Icons/code_block.png");
		m_TextureMap[FileType::Font] = Texture2D::Create(spec, m_ResourceDirectory / "Icons/font.png");

		m_BackButton = Texture2D::Create(spec, m_ResourceDirectory / "Icons/back_arrow.png");
	}

	void ContentBrowserPannel::OnDetach()
	{
	}

	void ContentBrowserPannel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");
		std::filesystem::path assetDir = AssetManager::Get()->GetAssetDir();
		if (ImGui::ImageButton("Back", (ImTextureID)(intptr_t)m_BackButton->GetRendererID(), { 20, 20 }, { 0, 1 }, { 1, 0 }) && m_CurrentPath != assetDir.parent_path())
		{
			m_CurrentPath = m_CurrentPath.parent_path();
		}
		ImGui::SameLine();
		ImGui::Text("%ls", std::filesystem::relative(m_CurrentPath, assetDir.parent_path()).c_str());
		if (ImGui::Button("Assets"))
		{
			m_CurrentPath = assetDir;
		}
		ImGui::SameLine();
		if (ImGui::Button("Scripts"))
		{
			m_CurrentPath = assetDir.parent_path() / "src/scripts";
		}
		ImGui::Separator();

		int numColumns = int(ImGui::GetContentRegionAvail().x / m_ItemSize);
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

	static ContentBrowserPannel::FileType MatchExtensionToFileType(const std::filesystem::path& path)
	{
		std::filesystem::path fileExtension = path.extension();

#define MATCH(extension, type)                       \
	if (fileExtension == extension)                  \
	{                                                \
		return ContentBrowserPannel::FileType::type; \
	}

		MATCH(".h", Header);
		MATCH(".cpp", Cpp);
		MATCH(".scene", Scene);
		MATCH(".anim", Animation);
		MATCH(".controller", AnimationController);
		MATCH(".png", Image);
		MATCH(".jpg", Image);
		MATCH(".jpeg", Image);
		MATCH(".glsl", Shader);

		return ContentBrowserPannel::FileType::Default;
	}

	void ContentBrowserPannel::RenderFileButton(const std::filesystem::directory_entry& entry)
	{
		std::filesystem::path path = entry.path();
		bool isDir = entry.is_directory();

		ImTextureID iconID;
		if (isDir)
		{
			iconID = (ImTextureID)(intptr_t)m_TextureMap[FileType::Directory]->GetRendererID();
		}
		else
		{
			FileType type = MatchExtensionToFileType(path);
			iconID = (ImTextureID)(intptr_t)m_TextureMap[type]->GetRendererID();
		}

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
		if (ImGui::ImageButton(path.string().c_str(), iconID, { m_ItemSize, m_ItemSize }, { 0, 1 }, { 1, 0 }))
		{
			if (!isDir)
			{
				SelectionContext::SetContext(CreateRef<FileContext>(path));
			}
		}
		ImGui::PopStyleVar();

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
#pragma once

#include "DaybreakEditor/Pannels/ViewportPannel.h"

#include <imgui.h>

namespace Daybreak
{
	ViewportPannel::ViewportPannel()
	{
		FrameBufferSpecifications fbspec;
		fbspec.Width = Application::Get().GetWindow().GetWidth();
		fbspec.Height = Application::Get().GetWindow().GetHeight();
		m_Framebuffer = FrameBuffer::Create(fbspec);
	}

	// ViewportPannel::~ViewportPannel()
	// {
	// }

	void ViewportPannel::OnAttach()
	{
	}

	void ViewportPannel::OnDetach()
	{
	}

	void ViewportPannel::OnImGuiRender()
	{
		ImGui::Begin("Viewport");


		// std::string text = m_IsPlaying ? "Pause" : "Play";
		// ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
		// if (ImGui::Button(text.c_str()))
		// {
		// 	m_IsPlaying = !m_IsPlaying;
		// 	m_IsPlaying ? Start() : Stop();
		// }
		// ImGui::Separator();
		uint32_t textureID = m_Framebuffer->GetAttachmentRendererID(0);
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		float aspectRatio = 1920.0f / 1080.0f;
		ImVec2 imageSize = { viewportSize.x, viewportSize.x / aspectRatio };
		ImGui::SetCursorPos({ 0.5f * (viewportSize.x - imageSize.x), 0.5f * (viewportSize.y - imageSize.y) + 43 }); // FIXME: 43 is just a temporary guess on to get the viewport to line up vertically correctly
		ImGui::Image((void*)textureID, imageSize, { 0, 1 }, { 1, 0 });

		ImGui::End();
		// ImGui::PopStyleVar();
	}

}
#include "pspch.h"
#include "ModelsPanel.h"

#include "TempGui/imgui.h"
#include "TempGui/imgui_internal.h"

#include "TempGui/imgui_impl_glfw.h"
#include "TempGui/imgui_impl_opengl3.h"

#include "Entities/Entity.h"

namespace PhySim {

	ModelsPanel::ModelsPanel()
	{
		m_SquareTexture = std::make_shared<Texture>("../assets/Textures/1x/Square.png");
		m_CircleTexture = std::make_shared<Texture>("../assets/Textures/1x/Circle.png");
		m_TriangleTexture = std::make_shared<Texture>("../assets/Textures/1x/Triangle.png");

	}

	void ModelsPanel::OnImGuiRender()
	{
		ImGui::Begin("Models");

		static float padding = 16.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((ImTextureID)m_SquareTexture->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
		
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("MODELS_PANEL", new Quad("newQuad"), sizeof(Quad));
			ImGui::EndDragDropSource();
		}
		ImGui::NextColumn();

		ImGui::ImageButton((ImTextureID)m_CircleTexture->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("MODELS_PANEL", nullptr, 0);
			ImGui::EndDragDropSource();
		}
		ImGui::NextColumn();

		ImGui::ImageButton((ImTextureID)m_TriangleTexture->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("MODELS_PANEL", nullptr, 0);
			ImGui::EndDragDropSource();
		}
		ImGui::PopStyleColor();

		ImGui::End();
	}
}
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
			Entity* newQuad = new Entity("newQuad");
			newQuad->spriteComponent = new SpriteComponent();
			newQuad->m_Translation = { 0.0f, 0.0f, 0.0f };
			ImGui::SetDragDropPayload("MODELS_PANEL", newQuad, sizeof(Entity));
			ImGui::EndDragDropSource();
			delete newQuad;
		}
		ImGui::NextColumn();

		ImGui::ImageButton((ImTextureID)m_CircleTexture->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
		if (ImGui::BeginDragDropSource())
		{
			Entity* newCircle = new Entity("newCircle");
			newCircle->circleComponent = new CircleComponent();
			newCircle->m_Translation = { 0.0f, 0.0f, 0.0f };
			ImGui::SetDragDropPayload("MODELS_PANEL", newCircle, sizeof(Entity));
			ImGui::EndDragDropSource();
			delete newCircle;
		}
		ImGui::NextColumn();

		ImGui::ImageButton((ImTextureID)m_TriangleTexture->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
		if (ImGui::BeginDragDropSource())
		{
			Entity* newTriangle = new Entity("newTriangle");
			newTriangle->triangleComponent = new TriangleComponent();
			newTriangle->m_Translation = { 0.0f, 0.0f, 0.0f };
			ImGui::SetDragDropPayload("MODELS_PANEL", newTriangle, sizeof(Entity));
			ImGui::EndDragDropSource();
			delete newTriangle;
		}
		ImGui::PopStyleColor();

		ImGui::End();
	}
}
#include "pspch.h"
#include "SceneHierarchyPanel.h"

#include "TempGui/imgui.h"
#include "TempGui/imgui_internal.h"

#include "TempGui/imgui_impl_glfw.h"
#include "TempGui/imgui_impl_opengl3.h"

#include "Core/Application.h"

#include "Entities/SceneSerializer.h"
#include "Utils/PlatformUtils.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef _MSVC_LANG
	#define _CRT_SECURE_NO_WARNINGS
#endif

namespace PhySim {

	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const std::shared_ptr<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionIndex = -1;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{

		ImGui::Begin("Scene Hierarchy");

		for (uint32_t i = 0; i < m_Context->m_Entities.size(); ++i)
		{
			DrawEntityNode(i);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionIndex = -1;

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Quad"))
				m_Context->CreteQuad("Quad");

			if (ImGui::MenuItem("Create Circle"))
				m_Context->CreteCircle("Circle");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionIndex > -1 && m_SelectionIndex < m_Context->m_Entities.size())
			DrawComponents(m_SelectionIndex);

		ImGui::End();
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawEntityNode(int index)
	{
		Entity* entity = m_Context->m_Entities[index];
		std::string name = entity->m_Name;

		ImGuiTreeNodeFlags flags = ((m_SelectionIndex == index) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)entity, flags, name.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionIndex = index;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			delete m_Context->m_Entities[index];
			m_Context->m_Entities.erase(m_Context->m_Entities.begin() + index);
			if (m_SelectionIndex == index)
				m_SelectionIndex = -1;
		}
	}


	void SceneHierarchyPanel::DrawComponents(int index)
	{
		Entity* entity = m_Context->m_Entities[index];

		std::string name = entity->m_Name;

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), name.c_str());

		if (ImGui::InputText("Name", buffer, sizeof(buffer)))
		{
			entity->m_Name = std::string(buffer);
		}

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::TreeNodeEx((void*)entity, treeNodeFlags, "Transform"))
		{
			
			DrawVec3Control("Translation", entity->m_Translation);
			glm::vec3 rotation = glm::degrees(entity->m_Rotation);
			DrawVec3Control("Rotation", rotation);
			entity->m_Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", entity->m_Scale, 30.0f);

			ImGui::TreePop();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 50.0f, lineHeight };

		if (entity->m_HasSpriteRendererComponent)
		{
			Quad* quad = dynamic_cast<Quad*>(entity);
			if (ImGui::TreeNodeEx((void*)(entity + 1), treeNodeFlags, "Sprite"))
			{
				if (quad)
				{
					auto& color = quad->m_Color;
					ImGui::ColorEdit4("Color", glm::value_ptr(color));

					ImGui::TreePop();
				}
			}

			if (ImGui::TreeNodeEx((void*)(entity + 2), treeNodeFlags, "Texture"))
			{
				if (quad)
				{
					if (ImGui::Button("clear", buttonSize))
					{
						quad->m_Texture = nullptr;
					}

					ImGui::SameLine();

					if (ImGui::Button("...", buttonSize))
					{
						std::string filepath = FileDialogs::OpenFile("Texture (*.png)\0*.png\0");
						if (!filepath.empty())
							quad->m_Texture = std::make_shared<Texture>(filepath);
					}

					ImGui::DragFloat("Tiling Factor", &quad->m_TilingFactor, 0.1f, 0.0f, 100.0f);
				}
				ImGui::TreePop();
			}
		}
		if (entity->rb2d)
		{
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
			const char* currentBodyTypeString = bodyTypeStrings[(int)entity->rb2d->Type];
			if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
					{
						currentBodyTypeString = bodyTypeStrings[i];
						entity->rb2d->Type = (Rigidbody2DComponent::BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &entity->rb2d->FixedRotation);
		}

		if (entity->bc2d)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(entity->bc2d->Offset));
			ImGui::DragFloat2("Size", glm::value_ptr(entity->bc2d->Offset));
			ImGui::DragFloat("Density", &entity->bc2d->Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &entity->bc2d->Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &entity->bc2d->Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &entity->bc2d->RestitutionThreshold, 0.01f, 0.0f);
		}
	}
}
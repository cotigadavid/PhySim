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

// DELETE
#include "OpenGL/Renderer2D.h"
// DELETE

namespace PhySim {

	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const std::shared_ptr<Scene>& scene)
	{
		m_Context = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar;
		ImGui::SetNextWindowSize(ImVec2(500, 256));

		bool open;
		ImGui::Begin("Scene Hierarchy", &open, flags);

		
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		for (uint32_t i = 0; i < m_Context->m_Entities.size(); ++i)
		{
			DrawEntityNode(i);
			//PS_ERROR("{0}", m_Context->m_Entities.size());
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

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
			//m_Context->m_Entities[index]->m_Rotation.z += 45;
			delete m_Context->m_Entities[index];
			m_Context->m_Entities.erase(m_Context->m_Entities.begin() + index);
			//PS_ERROR("{0}", index);
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
			DrawVec3Control("Scale", entity->m_Scale, 1.0f);

			ImGui::TreePop();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		if (entity->m_HasSpriteRendererComponent)
		{
			if (ImGui::TreeNodeEx((void*)(entity + 1), treeNodeFlags, "Color"))
			{
				Quad* quad = dynamic_cast<Quad*>(entity);
				//Quad* quad = (Quad*)entity;
				if (quad)
				{
					auto& color = quad->m_Color;
					ImGui::ColorEdit4("Color", glm::value_ptr(color));

					ImGui::TreePop();
				}
			}
		}

	}

	void SceneHierarchyPanel::NewScene()
	{
		m_Context->m_Entities.clear();
		//m_Context = std::make_shared<Scene>();
		//m_Context->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		//SetContext(m_Context);
		//Application::Get().m_Scene = m_Context;
		//m_Context->AddEntity(new Quad("fake", m_Context.get()));
		
		/*m_Context->AddEntity(new Quad("fake", m_Context.get()));
		m_Context->AddEntity(new Quad("fake", m_Context.get()));
		m_Context->m_Entities.pop_back();
		m_Context->m_Entities.pop_back();*/
		//Renderer2D::DrawQuad(glm::mat4(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		/*for (uint32_t index = 0; index < m_Context->m_Entities.size(); index++)
		{
			m_Context->m_Entities[index]->m_Translation.x = 100;

			PS_INFO("{0}", index);
		}

		m_Context->m_Entities.clear();*/

		/*m_Context->OnUpdate(101);

		for (uint32_t index = 0; m_Context->m_Entities.size(); )
		{
			//m_Context->m_Entities[index]->m_Translation.z = -2;
			//PS_ERROR("{0}", m_Context->m_Entities[index]->m_Translation.z);
			//delete m_Context->m_Entities[index];
			m_Context->m_Entities.erase(m_Context->m_Entities.begin() + index);
			
		}*/

		

		/*for (int i = m_Context->m_Entities.size() - 2; i >= 0; --i)
		{
			delete m_Context->m_Entities[i];
			m_Context->m_Entities.pop_back();
		}*/
	}

	void SceneHierarchyPanel::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("PhySim Scene (*.physim)\0*.physim\0");
		if (!filepath.empty())
		{
			//m_Context = std::make_shared<Scene>();
			//m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			//SetContext(m_Context);
			m_Context->m_Entities.clear();

			SceneSerializer serializer(m_Context);
			serializer.Deserialize(filepath);
		}
	}

	void SceneHierarchyPanel::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("PhySim Scene (*.physim)\0*.physim\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_Context);
			serializer.Serialize(filepath);
		}
	}
}
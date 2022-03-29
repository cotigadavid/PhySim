#include "pspch.h"
#include "PhysicsPanel.h"

#include "TempGui/imgui.h"
#include "TempGui/imgui_internal.h"

#include "TempGui/imgui_impl_glfw.h"
#include "TempGui/imgui_impl_opengl3.h"



namespace PhySim {

	PhysicsPanel::PhysicsPanel()
	{
		
	}

	void PhysicsPanel::OnImGuiRender()
	{
		ImGui::Begin("Physics");

		//const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (m_SceneHierarchyPanel->GetSelectedEntity())
		{
			Entity* entity = m_SceneHierarchyPanel->GetSelectedEntity();
			if (entity->rb2d)
			{
				
				if (ImGui::Button("Apply") && entity->rb2d->RuntimeBody)
				{
					entity->rb2d->RuntimeBody->ApplyForce(b2Vec2(entity->rb2d->HorizontalForce, entity->rb2d->VerticalForce), entity->rb2d->RuntimeBody->GetWorldCenter(), 1);
					//m_Context->m_Entities[1]->rb2d->RuntimeBody->ApplyForce(b2Vec2(0, 150), m_Context->m_Entities[1]->rb2d->RuntimeBody->GetWorldCenter(), 0);
				}
				ImGui::PushItemWidth(200);
				ImGui::SameLine();
				ImGui::DragFloat("HorizontalForce", &(entity->rb2d->HorizontalForce), 0.025f);
				ImGui::SameLine();
				ImGui::SetCursorPosX(385);
				ImGui::DragFloat("VerticalForce", &(entity->rb2d->VerticalForce), 0.025f);
				ImGui::PopItemWidth();

				if (ImGui::Button("Apply") && entity->rb2d->RuntimeBody)
				{
					entity->rb2d->RuntimeBody->ApplyLinearImpulse(b2Vec2(entity->rb2d->HorizontalImpulse, entity->rb2d->VerticalImpulse), entity->rb2d->RuntimeBody->GetWorldCenter(), 1);
				}
				ImGui::PushItemWidth(200);
				ImGui::SameLine();
				ImGui::DragFloat("HorizontalImpulse", &(entity->rb2d->HorizontalImpulse), 0.025f);
				ImGui::SameLine();
				ImGui::DragFloat("VerticalImpulse", &(entity->rb2d->VerticalImpulse), 0.025f);
				ImGui::PopItemWidth();
				
			}
		}

		ImGui::End();
	}
}



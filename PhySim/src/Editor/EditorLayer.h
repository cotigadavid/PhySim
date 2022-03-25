#pragma once

#include "Core/Layer.h"
#include "SceneHierarchyPanel.h"
#include "ModelsPanel.h"

#include "OpenGL/FrameBuffer.h"

#include <filesystem>

namespace PhySim {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
			
		std::shared_ptr<Framebuffer> m_Framebuffer;

	private:
		void UI_Toolbar();

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void OnScenePlay();
		void OnSceneStop();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();
		void SaveScene();

		void OnDuplicateEntity();

		void OnOverlayRender();

	private:
		std::shared_ptr<Scene> m_ActiveScene; 
		std::shared_ptr<Scene> m_SavedScene;
		std::filesystem::path m_EditorScenePath;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		std::shared_ptr<Texture> m_PlayIcon, m_StopIcon;

		int m_GizmoType = 0;
		
		bool m_ShowPhyCol = false;

		enum class SceneState
		{
			Edit = 0, Play
		};

		SceneState m_SceneState = SceneState::Edit;

		int m_HoveredEntityIndex;
		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ModelsPanel m_ModelsPanel;
	};


}
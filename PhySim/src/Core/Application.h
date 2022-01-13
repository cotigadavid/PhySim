#pragma once

#include "Core.h"
#include "Window/Window.h"
#include "Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

#include "OpenGL/Shader.h"
#include "OpenGL/Buffer.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/Texture.h"

#include "ProjectionData.h"

#include "Entities/Entity.h"
#include "Entities/Scene.h"

#include "Editor/SceneHierarchyPanel.h"

#include "LayerStack.h"

namespace PhySim {


	class Application
	{
	public:

		Application();
		~Application();

		void Run();
		void OnEvent(Event& e);
		void OnUpdate(Timestep ts);

		Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

		//ResultData& GetResultData() { return m_ResultData; }

	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		ImGuiLayer* m_ImguiLayer;
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0;

		ProjectionData m_ProjectionData;

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Texture> m_CheckerboardTexture, m_LogoTexture;

		ShaderLibrary m_ShaderLibrary;

		static Application* s_Instance;

		SceneHierarchyPanel m_SceneHierarchyPanel;

	public:
		glm::vec3 Squarecolor;
		std::shared_ptr<Scene> m_Scene;


	};
}
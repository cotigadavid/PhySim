#include "pspch.h"

#include "Application.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/Renderer2D.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>


namespace PhySim {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_ProjectionData(0.0f, 1280.0f, 0.0f, 720.0f)
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImguiLayer = new ImGuiLayer();
		m_LayerStack.PushLayer(m_ImguiLayer);

		m_CheckerboardTexture = std::make_shared<Texture>("../assets/Checkerboard.png");

		m_Scene = std::make_shared<Scene>();

		m_Scene->AddEntity(new Quad("quad1", m_Scene.get()));

		Quad* quad2 = new Quad("quad2", m_Scene.get());
		quad2->m_Translation = { 200.0f, 200.0f, 1.0f };
		quad2->m_Color = { 0.5f, 1.0f, 0.5f, 1.0f };
		m_Scene->AddEntity(quad2);

		Quad* quad3 = new Quad("quad3", m_Scene.get());
		quad3->m_Translation = { 500.0f, 500.0f, 1.0f };
		quad3->m_Color = { 1.0f, 0.5f, 0.5f, 1.0f };
		m_Scene->AddEntity(quad3);

		m_SceneHierarchyPanel.SetContext(m_Scene);
	}

	Application::~Application()
	{
		
	}
	
	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
			this->OnUpdate(timestep);
			//m_imguiLayer->OnUpdate(timestep);
			m_SceneHierarchyPanel.OnImGuiRender();
			m_Window->OnUpdate();
		}
	}

	void Application::OnUpdate(Timestep ts)
	{
		m_ProjectionData.RecalculateViewMatrix();

		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		Renderer::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		Renderer::Clear();

		Renderer2D::BeginScene(m_ProjectionData);

		/*Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderer2D::DrawQuad({ 0.5f, -0.5f, 1.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
		Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);
		*/

		//Renderer2D::DrawQuad(*quad);

		m_Scene->OnUpdate(ts);

		Renderer2D::EndScene();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		//m_imguiLayer->OnEvent(e);

		for (std::vector<Layer*>::iterator it = m_LayerStack.begin(); it != m_LayerStack.end(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		float aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_ProjectionData.SetProjection(0, aspectRatio * 720, 0, 720);
		//Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		
		return false;
	}

}
#pragma once

#include "OpenGL/Texture.h"

namespace PhySim {

	class ModelsPanel
	{
	public:
		ModelsPanel();
		
		void OnImGuiRender();

	private:
		std::shared_ptr<Texture> m_SquareTexture;
		std::shared_ptr<Texture> m_CircleTexture;
		std::shared_ptr<Texture> m_TriangleTexture;
	};
}
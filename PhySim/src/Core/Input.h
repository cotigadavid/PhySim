#pragma once

#include "Core.h"

namespace PhySim {
	
	class Input
	{
	public:
		inline bool IsKeyPressed(int keycode);
		inline bool IsMouseButtonPressed(int mousebuttoncode);

		inline std::pair<float, float> GetMousePosition();
		inline float GetMouseX();
		inline float GetMouseY();


	private:
		static Input* s_Instance;
	};
}
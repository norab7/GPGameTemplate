#pragma once
#include <GLFW/glfw3.h>

namespace _I_Input {
	class I_Input {
	protected:
	public:
		virtual void process_keyboard(bool* KEYS) = 0;
		virtual void process_mouse_movement(double xpos, double ypos) = 0;
		virtual void process_mouse_scroll() = 0;
	};
}
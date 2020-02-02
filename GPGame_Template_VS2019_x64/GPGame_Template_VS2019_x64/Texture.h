#pragma once
#include <GL/glew.h>
#include <gl\GL.h>

#ifndef STB_IMAGE_H
#define STB_IMAGE_H
#include <stb_image.h>
#endif // !STB_IMAGE_H

namespace _Texture {
	class Texture {
		unsigned int texture;
	public:
		Texture(const char* file, GLint format);
		~Texture();

		unsigned int get();
		// TODO: add some adjustment functions for wrapping/repeat ect

	};
}
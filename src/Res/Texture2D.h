#pragma once

#include "glad/glad.h"
#include <string>

namespace Render{
	class Textrue2D
	{
	public:
		Textrue2D ( const GLuint width, 
					const GLuint height,
					const unsigned char* data,
					const unsigned int channels, 
					const GLenum filter, 
					const GLenum wrapMode);

		Textrue2D() = delete;
		Textrue2D(const Textrue2D&) = delete;

		Textrue2D& operator= (const Textrue2D&) = delete;
		Textrue2D& operator= (Textrue2D&& texture2d);
		Textrue2D(Textrue2D&& texture2d);

		~Textrue2D();

		void bind() const;
	private:
		GLuint m_ID;
		GLenum m_mode;
		unsigned int m_width;
		unsigned int m_height;
	};

	
}
#pragma once
#include <gl/glew.h>
#include "Globals.h"
#include<assert.h>
namespace bogong {
	class Texture
	{
	public:
		unsigned int m_TexID = 0;
		Texture() = default;
		Texture(unsigned int p_Width, unsigned int p_Height, unsigned char * p_TextureBufferData,int nrComponents = 4)
		{
			glGenTextures(1, &m_TexID);
			assert(!error());
			glBindTexture(GL_TEXTURE_2D, m_TexID);
			assert(!error());
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;
			assert(!error());
			glTexImage2D(GL_TEXTURE_2D, 0, format, p_Width, p_Height, 0, format, GL_UNSIGNED_BYTE, p_TextureBufferData);
			assert(!error());
			glGenerateMipmap(GL_TEXTURE_2D);
			assert(!error());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			assert(!error());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			assert(!error());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);			assert(!error());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			assert(!error());

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void Bind()
		{
			glBindTexture(GL_TEXTURE_2D, m_TexID);
		}
		void Unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};
}
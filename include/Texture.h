#pragma once
#include <gl/glew.h>
#include "Globals.h"
#include<assert.h>
namespace bogong {
	struct TextureParameters {
		/*
			Specifies the format of the pixel data. 
			The following symbolic values are accepted: GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL. 
		*/
		GLenum format                          = GL_NONE;
		GLenum internal_format                 = GL_NONE;
		
		GLenum TextureWrapSMode                = GL_REPEAT;
		
		//
		GLenum TextureWrapTMode				   = GL_REPEAT;
		GLenum TextureMinificationFilterMode   = GL_LINEAR;
		GLenum TextureMagnificationFilterMode  = GL_LINEAR;
		/*
			 target defines the target texture, like GL_TEXTURE_2D, GL_TEXTURE_2D_ARRAY,GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_2D_MULTISAMPLE_ARRAY
		*/
		GLenum Target                          = GL_TEXTURE_2D;

		/*
			Specifies the data type of the pixel data. like GL_UNSIGNED_BYTE,GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT
		*/
		GLenum type                            = GL_UNSIGNED_BYTE;
		bool generateMipMap = true;
	};
	class Texture
	{
	public:
		unsigned int m_TexID = 0;
		Texture() = default;
		TextureParameters tex_param;
		Texture(unsigned int p_Width, unsigned int p_Height, unsigned char * data, TextureParameters params = TextureParameters()) 
			:
			tex_param(params)
		{
			
			CHECK_GL_ERROR( glGenTextures(1, &m_TexID) );
			
			CHECK_GL_ERROR(glBindTexture(params.Target, m_TexID));
			CHECK_GL_ERROR(glTexImage2D(params.Target, 0, params.internal_format, p_Width, p_Height, 0, params.format, params.type, data));
			if(params.generateMipMap)
			CHECK_GL_ERROR(glGenerateMipmap(params.Target));
			if (params.TextureWrapSMode != GL_NONE)
				CHECK_GL_ERROR(glTexParameteri(params.Target, GL_TEXTURE_WRAP_S, params.TextureWrapSMode));
			if (params.TextureWrapTMode != GL_NONE)
				CHECK_GL_ERROR(glTexParameteri(params.Target, GL_TEXTURE_WRAP_T, params.TextureWrapTMode));
			if (params.TextureMinificationFilterMode != GL_NONE)
				CHECK_GL_ERROR(glTexParameteri(params.Target, GL_TEXTURE_MIN_FILTER, params.TextureMinificationFilterMode));
			if (params.TextureMagnificationFilterMode != GL_NONE)
				CHECK_GL_ERROR(glTexParameteri(params.Target, GL_TEXTURE_MAG_FILTER, params.TextureMagnificationFilterMode));
			CHECK_GL_ERROR(glBindTexture(params.Target, 0));
		}
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
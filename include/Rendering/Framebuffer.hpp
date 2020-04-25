#pragma once
#include "../Globals.h"
#include "../Texture.h"
namespace bogong {
	class Framebuffer {
	private:
		unsigned int id = 0;
		unsigned int width = 0;
		unsigned int height = 0;
		bool isMultiSampled = false;
		std::shared_ptr<Texture> colour_tex;
		std::shared_ptr<Texture> depth_tex;
	public:
		void Bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, id);
		}

		void Unbind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		void clear() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
		Framebuffer(unsigned int width, unsigned int height, bool isMultiSampled = false)
			:
			width(width),
			height(height),
			isMultiSampled(isMultiSampled)
		{
			glGenFramebuffers(1, &id);
			Bind();
			InitColourAttachment();
			InitDepthStencilAttachment();
			Unbind();
		}
		void InitDepthStencilAttachment() {
			TextureParameters tp;
			tp.format = GL_DEPTH_STENCIL;
			tp.internal_format = GL_DEPTH24_STENCIL8;
			tp.type = GL_UNSIGNED_INT_24_8;
			tp.TextureMinificationFilterMode = GL_NONE;
			tp.TextureMagnificationFilterMode = GL_NONE;
			tp.TextureWrapSMode = GL_NONE;
			tp.TextureWrapTMode = GL_NONE;
			tp.Target = GL_TEXTURE_2D;
			tp.generateMipMap = false;
			depth_tex = std::make_shared<Texture>(width, height, nullptr, tp);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_tex->m_TexID, 0);
		}
		void InitColourAttachment() {
			TextureParameters tp;
			tp.generateMipMap = false;
			tp.format = GL_RGBA;
			tp.Target = GL_TEXTURE_2D;
			tp.internal_format = GL_RGBA;
			tp.TextureWrapSMode = GL_NONE;
			tp.TextureWrapTMode = GL_NONE;
			colour_tex = std::make_shared<Texture>(width, height, nullptr, tp);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colour_tex->m_TexID, 0);

		}
		std::shared_ptr<Texture> GetColourTexture() {
			return colour_tex;
		}
		std::shared_ptr<Texture> GetDepthTexture() {
			return depth_tex;
		}

	};
}
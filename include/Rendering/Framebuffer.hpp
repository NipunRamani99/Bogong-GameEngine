#pragma once
#include "../Globals.h"
#include "../Texture.h"
namespace bogong {
	class Framebuffer {
	private:
		unsigned int id = 0;
		unsigned int width = 0;
		unsigned int height = 0;
		std::shared_ptr<Texture> colour_tex;
		std::shared_ptr<Texture> depth_tex;
	public:
		void Bind();
		void Unbind();
		Framebuffer(unsigned int width, unsigned int height) {

		}
		std::shared_ptr<Texture> GetColourTexture() {
			return colour_tex;
		}
		std::shared_ptr<Texture> GetDepthTexture() {
			return depth_tex;
		}

	};
}
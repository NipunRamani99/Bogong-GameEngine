#include "TextureFactory.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

namespace bogong {
	std::unordered_map<size_t, std::shared_ptr<Texture>> TextureManager::texmap;
	std::shared_ptr<bogong::Texture> TextureManager::make_texture(std::string path)
	{
		size_t h = HashPath(path);
		if (texmap.find(h) != texmap.end()) {
			return texmap[h];
		}
		else
		{
			int width = 0;
			int height = 0;
			int chnls = 0;
			unsigned char * data = stbi_load(path.c_str(), &width, &height, &chnls, 0);
			auto tex = std::make_shared<bogong::Texture>(width, height, data, chnls);
			delete data;
			texmap[h] = tex;
			return tex;
		}
	}
}

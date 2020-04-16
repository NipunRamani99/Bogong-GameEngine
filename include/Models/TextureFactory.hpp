#pragma once
#include<memory>
#include<string>
#include "../Texture.h"
#include <unordered_map>
#include "../stb_image.h"
namespace bogong {
	class TextureManager {
		static std::unordered_map<size_t, std::shared_ptr<Texture>> texmap;
	public:
		static size_t HashPath(std::string path) {
			std::hash<std::string> hasher;
			size_t h  = hasher(path);
			return h;
		}
		static std::shared_ptr<bogong::Texture> make_texture(std::string path);
		static void save_texture(std::string path) {
			size_t hash = HashPath(path);
			if (!(texmap.find(hash)!=texmap.end())) {
				int width = 0;
				int height = 0;
				int chnls = 0;
				unsigned char * data = stbi_load(path.c_str(), &width, &height, &chnls, 0);
				auto tex = std::make_shared<bogong::Texture>(width, height, data, chnls);
				delete data;
				texmap[hash] = tex;
			}
		}
	};
}
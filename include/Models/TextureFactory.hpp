#pragma once
#include<memory>
#include<string>
#include "../Texture.h"
#include <unordered_map>
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
	};
}
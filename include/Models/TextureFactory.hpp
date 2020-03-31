#pragma once
#include<memory>
#include<string>
#include "../Texture.h"
class TextureFactory {
public:
	static std::shared_ptr<bogong::Texture> make_texture(std::string path);
};
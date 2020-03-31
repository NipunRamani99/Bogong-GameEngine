#include "TextureFactory.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
std::shared_ptr<bogong::Texture> TextureFactory::make_texture(std::string path)
{
	int width = 0;
	int height = 0;
	int chnls = 0;
	unsigned char * data = stbi_load(path.c_str(),&width,&height,&chnls,0);
	auto tex = std::make_shared<bogong::Texture>(width, height, data,chnls);
	
	delete data;
	return tex;
}

#include "Texture.h"
#include "Engine/Dependencies/stb_image/stb_image.h"
#include <iostream>
#include "Graphics.h"

namespace Sapphire
{
	TextureManager textureManager_;

	Texture::Texture(const std::string newpath) : path_(newpath), width_(0), height_(0), channels_(0), textureId_(NULL)
	{

	}

	Texture::~Texture()
	{
		int i = 0;
	}

	const std::shared_ptr<Texture> TextureManager::AddNewTexture(const std::string newpath)
	{
		// load and generate the texture
		std::shared_ptr<Texture> newTexture(new Texture(newpath));
		//latestTexture = newTexture;
		char charPath[1024];
		strncpy_s(charPath, newpath.c_str(), sizeof(charPath));
		charPath[sizeof(charPath) - 1] = 0;
		unsigned char* tempStbiData = stbi_load(charPath, &newTexture->width_, &newTexture->height_, &newTexture->channels_, 0);
		//unsigned char* tempStbiData = stbi_load(path, &object.lock()->GetTexture().lock()->width_, &object.lock()->GetTexture().lock()->height_, &object.lock()->GetTexture().lock()->channels_, 0);

		if (tempStbiData)
		{
			graphics_.ProcessTexture(tempStbiData, newTexture);
			allTextures_.insert(std::pair<const std::string, std::shared_ptr<Texture>>(newpath, newTexture));
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
			//return false;
		}
		stbi_image_free(tempStbiData);
		//allLoadedTextures.push_back(std::make_shared<Texture>(newpath));
		return allTextures_.find(newpath)->second;
	}

	/*const std::shared_ptr<Texture> &TextureManager::GetNewestTexture()
	{
		return allTextures_.end()->second;
	}*/

	std::shared_ptr<Texture> TextureManager::GetTexture(const std::string path)
	{
		assert(this);
		if(allTextures_.find(path) == allTextures_.end())
		{
			//key not present in map, add new Texture
			return AddNewTexture(path);
		}
		return allTextures_.find(path)->second;
		
	}

}

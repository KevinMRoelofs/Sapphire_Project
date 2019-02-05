#include "Texture.h"
#include "Engine/Dependencies/stb_image/stb_image.h"
#include <iostream>
#include "Graphics.h"

namespace Sapphire
{
	TextureManager textureManager_;

	Texture::Texture(const char * newpath) : path_(newpath), width_(0), height_(0), channels_(0), textureId_(NULL)
	{

	}

	Texture::~Texture()
	{

	}

	const std::shared_ptr<Texture> &TextureManager::AddNewTexture(const char * newpath)
	{
		// load and generate the texture
		auto newTexture = std::make_shared<Texture>(Texture(newpath));
		//latestTexture = newTexture;
		unsigned char* tempStbiData = stbi_load(newpath, &newTexture->width_, &newTexture->height_, &newTexture->channels_, 0);
		//unsigned char* tempStbiData = stbi_load(path, &object.lock()->GetTexture().lock()->width_, &object.lock()->GetTexture().lock()->height_, &object.lock()->GetTexture().lock()->channels_, 0);

		if (tempStbiData)
		{
			graphics_.ProcessTexture(tempStbiData, newTexture);
			allTextures_.insert(std::pair<const char*, std::shared_ptr<Texture>>(newpath, newTexture));
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

	std::weak_ptr<Texture> TextureManager::GetTexture(const char * path)
	{
		if(allTextures_.find(path) == allTextures_.end())
		{
			//key not present in map, add new Texture
			return AddNewTexture(path);
		}
		return allTextures_.find(path)->second;
		
	}

}

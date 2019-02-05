#pragma once
#include <vector>
#include <memory>
#include <map>

namespace Sapphire
{
	class Texture
	{
	public:
		Texture(const char* newpath);
		~Texture();

		int width_;
		int height_;
		int channels_;

		//unsigned char* data = nullptr;
		unsigned int textureId_;
		const char* path_ = nullptr;
	};

	class TextureManager
	{
	public:
		const std::shared_ptr<Texture> &AddNewTexture(const char * newpath);
		//const std::shared_ptr<Texture> &GetNewestTexture();
		std::weak_ptr<Texture> GetTexture(const char* path);
	private:
		std::map<const char*, std::shared_ptr<Texture>> allTextures_;
		//std::vector<std::shared_ptr<Texture>> allLoadedTextures;
	};

	extern TextureManager textureManager_;
}


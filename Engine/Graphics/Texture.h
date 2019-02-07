#pragma once
#include <vector>
#include <memory>
#include <map>

namespace Sapphire
{
	class Texture
	{
	public:
		Texture(const std::string newpath);
		~Texture();

		int width_;
		int height_;
		int channels_;

		//unsigned char* data = nullptr;
		unsigned int textureId_;
		const std::string path_ = nullptr;
	};

	class TextureManager
	{
	public:
		//const std::shared_ptr<Texture> &GetNewestTexture();
		std::shared_ptr<Texture> GetTexture(const std::string path);
	private:
		const std::shared_ptr<Texture> AddNewTexture(const std::string newpath);
		std::map<const std::string, std::shared_ptr<Texture>> allTextures_;
		//std::vector<std::shared_ptr<Texture>> allLoadedTextures;
	};

	extern TextureManager textureManager_;
}


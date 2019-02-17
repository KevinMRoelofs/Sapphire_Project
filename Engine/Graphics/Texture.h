#pragma once
#include <vector>
#include <memory>
#include <map>

#include "Engine/Dependencies/cereal-master/include/cereal/cereal.hpp"
#include "Engine/Dependencies/cereal-master/include/cereal/archives/binary.hpp"
#include "Engine/Dependencies/cereal-master/include/cereal/types/string.hpp"
//#include "Engine/Dependencies/cereal-master/include/cereal/types/memory.hpp"

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
		const std::string path_ = NULL;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(path_); // serialize things by passing them to the archive
		}

		// We could define load_and_construct internally:
		template <class Archive>
		static void load_and_construct(Archive & archive, cereal::construct<Texture> & construct)
		{
			std::string path("not a path");
			archive(path);
			construct(path);
		}
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


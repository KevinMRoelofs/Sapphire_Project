#pragma once
#include <string>
#include <memory>

namespace Sapphire
{

	class Scene
	{
	public:
		Scene();
		~Scene();

		void New();
		void Save(const std::string &filePath);
		void Load(const std::string &filePath);

		std::string fullPath;

		std::string name;

		std::shared_ptr<class ObjectManager> objectManagerPtr_;
		std::shared_ptr<class ActorManager> actorManagerPtr_;
		std::shared_ptr<class PlayerManager> playerManagerPtr_;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(objectManagerPtr_, actorManagerPtr_, playerManagerPtr_);
		}

	};
	extern Scene scene_;
}
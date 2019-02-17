#include "Scene.h"

#include <fstream>
#include <istream>
#include "Engine/Engine.h"
#include "Engine/Dependencies/cereal-master/include/cereal/types/polymorphic.hpp"
#include "Engine/Resources/Player.h"
#include "Engine/Graphics/Graphics.h"

namespace Sapphire
{
	Scene scene_;

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::New()
	{
		playerManager_->DeleteAllPlayers();
		actorManager_->DeleteAllActors();
		objectManager_->DeleteAllObjects();
		graphics_.camera_.SetPositition(0, 0);
	}

	void Scene::Save(const std::string &filePath)
	{
		std::ofstream os(filePath, std::ios::binary | std::ios::out);
			cereal::BinaryOutputArchive setArchive(os);

			setArchive(
				objectManager_,
				actorManager_,
				playerManager_);
			os.close();
	}

	void Scene::Load(const std::string &filePath)
	{
		const std::string &fileName(filePath);
		std::ifstream is(fileName, std::ios::binary | std::ios::in);
		if (is.good())
		{
			cereal::BinaryInputArchive getArchive(is);

			objectManager_->DeleteAllObjects();
			actorManager_->DeleteAllActors();
			playerManager_->DeleteAllPlayers();

			getArchive(objectManager_, actorManager_, playerManager_);

			is.close();

			for (auto &object : objectManager_->allObjects_)
			{
				objectManager_->CreateObject(object->GetTexture()->path_, "Quad", object->GetPositionWorld(), object);
			}

			for (auto &actor : actorManager_->allActors_)
			{
				actorManager_->CreateActor(actor->GetTexture()->path_, actor->GetPositionWorld(), actor);
			}

			for (auto &player : playerManager_->GetAllPlayers())
			{
				playerManager_->CreatePlayer(player->GetTexture()->path_, player->GetPositionWorld(), player->playerId_, player);
			}

		}
	}
}

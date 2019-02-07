#pragma once

#include "Engine/Resources/Object.h"
#include "Engine/Resources/Tile.h"
#include <memory>

namespace Sapphire
{
	class Actor : public Object
	{
	public:
		Actor();
		~Actor();

		void UpdateActor();

		float health_;
		float maxHealth_;

		glm::vec2 movementDirection_;
		glm::vec2 velocity_;

		float movementSpeed_;
		float jumpStrength_;

	};

	class ActorManager
	{
	public:
		~ActorManager();
		std::vector<std::shared_ptr<Actor>> allActors_;
		std::weak_ptr<Actor> CreateActor(const std::string &texturePath, glm::vec2 newPos, std::shared_ptr<Actor> actorPtr);
		std::weak_ptr<Actor> GetActor(const int &elementIndex);
		bool DeleteActor(std::weak_ptr<Actor> actorToDelete);
		int actorCount = 0;
	};

	extern ActorManager actorManager_;
}

#pragma once

#include "Actor.h"
#include <vector>

namespace Sapphire
{
	class Player : public Actor
	{
	public:
		Player();
		~Player();

		void UpdateMovement();
	
		int playerId_ = 0;
	};

	class PlayerManager
	{
	public:
		PlayerManager();
		~PlayerManager();
		std::weak_ptr<Player> CreatePlayer(const std::string texturePath, glm::vec2 newPos, int playerId = 0);
		bool DeletePlayer(std::weak_ptr<Player> playerToDelete);
		void UpdatePlayers();

		std::weak_ptr<Player> GetPlayer(int index);
	private:
		std::vector<std::shared_ptr<Player>> players_;
	};

	extern PlayerManager playerManager_;
}

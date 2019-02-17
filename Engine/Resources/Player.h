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

		std::string test;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(cereal::base_class<Actor>(this), playerId_);
		}
	};

	class PlayerManager
	{
	public:
		PlayerManager();
		~PlayerManager();
		std::weak_ptr<Player> CreatePlayer(const std::string texturePath, glm::vec2 newPos, int playerId = 0, std::shared_ptr<Player> playerPtr = nullptr);
		bool DeletePlayer(std::weak_ptr<Player> playerToDelete);
		void DeleteAllPlayers();
		void UpdatePlayers();

		std::weak_ptr<Player> GetPlayer(int index);

		std::vector<std::shared_ptr<Player>> GetAllPlayers();

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(players_);
		}

		std::vector<std::shared_ptr<Player>> players_;
	private:
	};

	extern std::shared_ptr<PlayerManager> playerManager_;
}


CEREAL_REGISTER_TYPE(Sapphire::Player)

#include "Player.h"

#include "Engine/Engine.h"
#include "Engine/Utility/Random.h"
#include "Engine/Utility/Input.h"

namespace Sapphire
{
	PlayerManager playerManager_;

	Player::Player()
	{
		
	}

	Player::~Player()
	{

	}

	void Player::UpdateMovement()
	{
		movementDirection_ = glm::vec3(0);
		if (inputManager_.AllKeyStates_[Key_Space].isPressed)
		{
 			velocity_.y += jumpStrength_;
		}
		if (inputManager_.AllKeyStates_[Key_D].isHeld)
		{
			movementDirection_.x += 1.f;
		}
		if (inputManager_.AllKeyStates_[Key_A].isHeld)
		{
			movementDirection_.x -= 1.f;
		}
	}

	PlayerManager::PlayerManager()
	{
	}

	PlayerManager::~PlayerManager()
	{

	}

	std::weak_ptr<Player> PlayerManager::CreatePlayer(const char* texturePath, glm::vec2 newPos, int playerId)
	{
		for(auto &player: players_)
		{
			if(player->playerId_ == playerId)
			{
				playerManager_.DeletePlayer(player);
			}
		}
		std::shared_ptr<Player> newPlayer = std::make_shared<Player>();
		newPlayer->playerId_ = playerId;
		players_.push_back(newPlayer);
		actorManager_.CreateActor(texturePath, newPos, newPlayer);
		return players_.back();
		//return newPlayer;
	}

	bool PlayerManager::DeletePlayer(std::weak_ptr<Player> playerToDelete)
	{
		if (playerToDelete.expired()) return false;
		int currentElementIndex = 0;
		for (auto &player : players_)
		{
			if (player == playerToDelete.lock())
			{
				//printf(": Destroyed Player %i \n", deletePlayerId);
				// DeleteObject function here{
				std::rotate(players_.begin(), players_.begin() + currentElementIndex + 1, players_.end());
				players_.pop_back();
				std::rotate(players_.begin(), players_.end() - currentElementIndex, players_.end());
				
				actorManager_.DeleteActor(player);
				return true;
				//}
			}
			currentElementIndex++;
		}
		return false;
	}

	void PlayerManager::UpdatePlayers()
	{
		for(auto &player: players_)
		{
			player->UpdateMovement();
		}
	}

	std::weak_ptr<Player> PlayerManager::GetPlayer(int index)
	{
		return players_[index];
	}
}

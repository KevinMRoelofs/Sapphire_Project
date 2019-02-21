#include "Game.h"
#include "Engine/Engine.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/Utility/Input.h"
#include "Engine/Resources/Actor.h"
#include "Engine/Resources/Player.h"

namespace Sapphire
{
	Game game_;

	Game::Game()
	{
		gameState_ = GameState::Playing;
	}

	void Game::StartUp()
	{
		playerManager_->CreatePlayer("../Assets/Player/Nico.png", glm::vec3(0));
		//load start level/menu here
	}

	void  Game::GameLoop()
	{
		engine_.MainLoop();
		if (inputManager_.AllKeyStates_[Key_P].isPressed && gameState_ != Playing) Unpause();
		else if (inputManager_.AllKeyStates_[Key_P].isPressed && gameState_ != Paused) Pause();

		if(gameState_ == Playing)
		{
			playerManager_->UpdatePlayers();

			for (auto& actor : actorManager_->allActors_)
			{
				actor->UpdateActor();
				
			}
		}
	}

	void  Game::Unpause()
	{
		gameState_ = Playing;
		engine_.timeScale_ = 1.f;
	}

	void  Game::Pause()
	{
		gameState_ = Paused;
		engine_.timeScale_ = 0.f;
	}
}

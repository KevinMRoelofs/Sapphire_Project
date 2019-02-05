#pragma once
namespace Sapphire
{
	class Game
	{
	public:
		Game();

		void StartUp();

		void GameLoop();

		enum GameState
		{
			InMenu,
			Playing,
			Paused
		}gameState_;

		void Unpause();
		void Pause();
	};

	extern Game game_;
}

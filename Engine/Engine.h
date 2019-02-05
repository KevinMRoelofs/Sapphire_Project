#pragma once
#include <string>
#include "Engine/Dependencies/glm/glm.hpp"


namespace Sapphire
{
	class Engine
	{
	public:
		Engine();
		~Engine();
		void Start(int const& width, int const& height, const std::string& name, const bool& fullScreen);
		void MainLoop();
		
		const int & GetTileSize();
		void SetTileSize(const int & newTileSize);

		enum PlayState
		{
			PLAYING,
			EDITMODE
		} playState_;

		float timeScale_;
		float deltaTime_;


		glm::vec2 gravity_;

	private:
		int tileSize_;
		float Time_, oldTime_;

	};
	extern Engine engine_;

}

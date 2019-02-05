#include "Game/Game.h"

#include "Engine/Engine.h"
#include "Engine/Graphics/Graphics.h"

using namespace Sapphire;

int main()
{
	engine_.Start(1920, 1080, "Nico's Adventure", true);
	game_.StartUp();
	while (graphics_.PrepareRender())
	{
		graphics_.Render();
		game_.GameLoop();
	};

	return 0;
}
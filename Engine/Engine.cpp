#include "Engine.h"

#include "stdio.h"
#include <windows.h>

#include "Graphics/Graphics.h"
#include "Graphics/Mesh.h"

#include "Engine/Resources/Actor.h"
#include "Engine/Resources/Player.h"

#include "Utility/Random.h"
#include "Utility/Input.h"
#include "Utility/Action.h"

namespace Sapphire
{
	Engine engine_;

	Engine::Engine()
	{
		tileSize_ = 32;
		timeScale_ = 1.f;

		gravity_ = glm::vec2(0, -30);
		Time_ = float(glfwGetTime());
	}

	Engine::~Engine()
	{
	}

	void Engine::Start(int const& width, int const& height, const std::string& name, const bool& fullScreen)
	{
		//std::cout << "Startup" << std::endl;
		const std::string &nameString = name + " - Console";
		const LPCSTR &nameLP = nameString.c_str();

		SetConsoleTitle(nameLP);
		graphics_.SetUpWindow(width, height, name, fullScreen);
		meshManager_.Start();
		
	}

	void Engine::MainLoop()
	{
		inputManager_.UpdateInputKeyboardMouse();
		actionManager_.Update();

		oldTime_ = Time_;
		Time_ = float(glfwGetTime());
		deltaTime_ = Time_ - oldTime_;

	}

	const int& Engine::GetTileSize()
	{
		return tileSize_;
	}

	void Engine::SetTileSize(const int& newTileSize)
	{
		tileSize_ = newTileSize;
		for(auto &object : objectManager_.allObjects_)
		{
			object->UpdateTransform();
		}
	}
}
